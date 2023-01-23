#include <toumou/rendering.hpp>
#include <toumou/constants.hpp>

#include <spdlog/spdlog.h>

#include <chrono>


namespace toumou {

	RayTracer::RayTracer(int w, int h, int sampling, int bounce, int split) :
		image(w, h), normal_map(w, h), depth_map(w, h), index_map(w, h),
	pixel_sampling(sampling), max_bounce(bounce), rays_per_bounce(split),
	m_dis(0.f, 1.f)
{
	std::random_device rd;
	m_gen = std::mt19937(rd());
}

Ray RayTracer::cast(std::shared_ptr<Camera> camera, float x, float y, float aspect_ratio) const
{
	// Compute pixel position in 3D space
	Vec3 pixel_pos = camera->location()
		+ camera->forward() * camera->sensor_width / std::tan(.5f * camera->field_of_view)
		- camera->left() * x * camera->sensor_width
		+ camera->up() * y * camera->sensor_width * aspect_ratio;
	return trace(camera->location(), pixel_pos);
}

Ray RayTracer::cast(const Vec3& pos, const Vec3& normal, float theta, float phi) const
{
	// Local coordinate system
	Vec3 tz = (std::abs(normal.x) > std::abs(normal.y)) ? Vec3(normal.z, 0, -normal.x) : Vec3(0, -normal.z, normal.y);
	tz.normalize();
	Vec3 tx = normal.cross(tz);
	Vec3 dir = tx * std::sin(theta) * std::cos(phi) + normal * std::cos(theta) + tz * std::sin(theta) * std::sin(phi);
	return Ray(pos, dir);
}

std::shared_ptr<Surface> RayTracer::hit(const Ray& ray, const Scene& scene, float& t, Vec3& normal) const
{
	std::shared_ptr<Surface> surface = nullptr;

	// Go through all surfaces
	for (auto s : scene.surfaces()) {

		// Check if ray intersects surface
		float t_local = 0.f;
		Vec3 n_local;
		if (!s->hit(ray, t_local, n_local)) {
			continue;
		}

		// Make sure intersection point is not ray origin
		if (t_local < eps_ray_sep) {
			continue;
		}
		
		// Update closest hit
		if (!surface || (surface && t_local < t)) {
			surface = s;
			t = t_local;
			normal = n_local;
		}
	}

	return surface;
}

Color RayTracer::direct_lighting(std::shared_ptr<Surface> surface, const Scene& scene, const Vec3& pos, const Vec3& normal) const
{
	Color c_out(0);

	// Go through all light sources
	for (auto light : scene.lights()) {

		// Retrieve light contribution
		Vec3 dir_light;
		float dist_light = 0.f;
		float intensity = 0.f;
		light->sample(pos, dir_light, dist_light, intensity);

		// Check if light source is obstructed
		Ray r_light(pos, dir_light);
		float t_obstruct = 0.f;
		Vec3 n_obstruct;
		auto s_obstruct = hit(r_light, scene, t_obstruct, n_obstruct);
		if (s_obstruct && t_obstruct < dist_light) {
			continue;
		}

		// Add lighting contribution
		float diffuse = std::max(0.f, normal.dot(dir_light)) * (surface->material).albedo / 3.14f;
		c_out = (surface->material).color_at(pos) * (diffuse * intensity);
	}

	return c_out;
}

Color RayTracer::indirect_lighting(std::shared_ptr<Surface> surface, const Scene& scene, const Vec3& pos, const Vec3& normal, int n_bounce) const
{
	Color c_out(0);

	// End of recursion
	if (n_bounce == 0) {
		return c_out;
	}

	// Split ray at bouncing point
	for (int i = 0; i < rays_per_bounce; i++) {

		// Generate ray in random direction
		float r1 = m_dis(m_gen);
		float theta = std::acos(1 - r1);
		float r2 = m_dis(m_gen);
		float phi = r2 * 6.18f;
		Ray ray_bounce = cast(pos, normal, theta, phi);

		// Find first surface hit
		float t_hit = 0.f;
		Vec3 n_hit;
		auto surf_hit = hit(ray_bounce, scene, t_hit, n_hit);
		if (!surf_hit) {
			continue;
		}

		Vec3 p_hit = ray_bounce.at(t_hit);

		// Direct lighting
		Color c_direct = direct_lighting(surf_hit, scene, p_hit, n_hit);

		// Recursive indirect lighting
		Color c_indirect = indirect_lighting(surf_hit, scene, p_hit, n_hit, n_bounce - 1);

		// Add lighting contribution
		float diffuse = std::max(0.f, normal.dot(ray_bounce.dir)) * (surface->material).albedo / 3.14f;
		c_out += (c_direct + c_indirect) * diffuse / static_cast<float>(rays_per_bounce);
	}

	return c_out;
}

void RayTracer::render(const Scene& scene, std::function<void(int)> progress_callback)
{
	// Start timer
	spdlog::info("start rendering");
	auto time_start = std::chrono::steady_clock::now();

	// Dimensions
	const int width = image.width();
	const int height = image.height();
	spdlog::info("dimensions: {}x{}", width, height);

	// Aspect ratio
	const float f_width = static_cast<float>(width);
	const float f_height = static_cast<float>(height);
	const float aspect_ratio = f_height / f_width;

	// First progress callback
	int progress = 0;
	progress_callback(0);

	// Loop over pixels
	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			// Pixel color (to compute)
			Color c_out(0);

			Vec3 n_out(0);
			float depth_min = (scene.camera())->z_far;
			float uid_out = 0.f;

			// Pixel top-left coordinates
			const float x = (static_cast<float>(j) / f_width) - .5f;
			const float y = .5f - (static_cast<float>(i) / f_height);

			// Send rays randomly over the pixel's area
			for (int k = 0; k < pixel_sampling; k++) {

				// Generate ray with a random offset
				const float dx = m_dis(m_gen) / f_width;
				const float dy = m_dis(m_gen) / f_height;
				const Ray ray = cast(scene.camera(), x + dx, y + dy, aspect_ratio);

				// Find first surface hit by ray
				float t = 0.f;
				Vec3 normal;
				auto surface = hit(ray, scene, t, normal);
				if (!surface) {
					// No surface hit, send next ray
					continue;
				}

				n_out += normal;

				if (t < depth_min) {
					depth_min = t;
					uid_out = static_cast<float>(surface->uid());
				}

				// Hit position
				Vec3 pos = ray.at(t);

				// Surface color at hit point (to compute)
				Color c_sample(0);

				// Direct lighting
				c_sample += direct_lighting(surface, scene, pos, normal);

				// Indirect lighting
				c_sample += indirect_lighting(surface, scene, pos, normal, max_bounce);

				// Add sample contribution
				c_sample.x = std::min(c_sample.x, 1.f);
				c_sample.y = std::min(c_sample.y, 1.f);
				c_sample.z = std::min(c_sample.z, 1.f);
				c_out += c_sample / static_cast<float>(pixel_sampling);
			}

			// Store results
			image.set(i, j, c_out);

			n_out.normalize();
			normal_map.set(i, j, n_out);
			depth_map.set(i, j, depth_min);
			index_map.set(i, j, uid_out);

			// Every time progress reaches one percent more, call progress callback
			progress++;
			if((progress * 100) % (width * height) == 0) {
				progress_callback((progress * 100) / (width * height));
			}
		}
	}

	// Stop timer and compute elapsed time
	auto time_end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = time_end - time_start;
	spdlog::info("rendering done in {}s", elapsed_seconds.count());
}

}

#include <toumou/rendering.hpp>
#include <toumou/constants.hpp>

#include <spdlog/spdlog.h>

#include <chrono>


namespace toumou {

RayTracer::RayTracer(int w, int h) :
	image(w, h), normal_map(w, h), depth_map(w, h), index_map(w, h),
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
	for (const auto& s : scene.surfaces()) {

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

Color RayTracer::direct_lighting(std::shared_ptr<Surface> surface, const Scene& scene, const Vec3& pos, const Vec3& normal, const Vec3& dir_view) const
{
	Color c_out(0);

	// Go through all light sources
	for (const auto& light : scene.lights()) {

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

		// Diffuse
		float diffuse = std::max(0.f, normal.dot(dir_light)) * (surface->material).albedo / 3.14f;
		c_out += (surface->material).color_at(pos) * (diffuse * intensity);

		// Specular
		float specular = brdf(surface->material, dir_light, dir_view, normal);
		c_out += light->color * (specular * intensity);
	}

	// Environment lighting
	auto env_light = scene.env_light();
	if (!env_light) {
		return c_out;
	}

	Color c_env(0);

	// Diffuse
	for (int i = 0; i < env_sampling; ++i) {

		// Generate ray in random direction
		float r1 = m_dis(m_gen);
		float theta = std::acos(1 - r1);
		float r2 = m_dis(m_gen);
		float phi = r2 * 6.18f;
		Ray ray = cast(pos, normal, theta, phi);

		// Check for surface intersection
		float t_hit = 0.f;
		Vec3 n_hit;
		auto surf_hit = hit(ray, scene, t_hit, n_hit);
		if (surf_hit) {
			continue;
		}

		Color c_sample(0);
		float intensity;
		env_light->sample(ray.dir, c_sample, intensity);

		float diffuse = std::max(0.f, normal.dot(ray.dir)) * (surface->material).albedo / 3.14f;
		c_env += (surface->material).color_at(pos) * (diffuse * intensity) / static_cast<float>(env_sampling);
	}

	// Specular
	for (int i = 0; i < env_sampling; ++i) {

		// Generate ray in random direction using GGX PDF
		float r1 = m_dis(m_gen);
		float theta = std::atan(surface->material.roughness * std::sqrt(r1 / (1.f - r1)));
		float r2 = m_dis(m_gen);
		float phi = r2 * 6.18f;
		Vec3 dir_reflected = 2.f * dir_view.dot(normal) * normal - dir_view;
		Ray ray = cast(pos, dir_reflected, theta, phi);

		// Check for surface intersection
		float t_hit = 0.f;
		Vec3 n_hit;
		auto surf_hit = hit(ray, scene, t_hit, n_hit);
		if (surf_hit) {
			continue;
		}

		Color c_sample(0);
		float intensity;
		env_light->sample(ray.dir, c_sample, intensity);

		float specular = brdf(surface->material, ray.dir, dir_view, normal);
		c_env += c_sample * (specular * intensity) / static_cast<float>(env_sampling);
	}

	c_out += c_env;

	return c_out;
}

Color RayTracer::indirect_lighting(std::shared_ptr<Surface> surface, const Scene& scene, const Vec3& pos, const Vec3& normal, const Vec3& dir_view, int n_bounce) const
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
		Vec3 dir_view_hit = ray_bounce.dir * -1;

		// Direct lighting
		Color c_direct = direct_lighting(surf_hit, scene, p_hit, n_hit, dir_view_hit);

		// Recursive indirect lighting
		Color c_indirect = indirect_lighting(surf_hit, scene, p_hit, n_hit, dir_view_hit, n_bounce - 1);

		// Diffuse
		float diffuse = std::max(0.f, normal.dot(ray_bounce.dir)) * (surface->material).albedo / 3.14f;
		float intensity = (c_direct + c_indirect).dot(Vec3(1)) / 6.f;
		c_out += (surface->material).color_at(pos) * intensity * diffuse / static_cast<float>(rays_per_bounce);
	}

	for (int i = 0; i < rays_per_bounce; i++) {

		// Generate ray in random direction using GGX PDF
		float r1 = m_dis(m_gen);
		float theta = std::atan(surface->material.roughness * std::sqrt(r1 / (1.f - r1)));
		float r2 = m_dis(m_gen);
		float phi = r2 * 6.18f;
		Vec3 dir_reflected = 2.f * dir_view.dot(normal)* normal - dir_view;
		Ray ray_bounce = cast(pos, dir_reflected, theta, phi);

		// Find first surface hit
		float t_hit = 0.f;
		Vec3 n_hit;
		auto surf_hit = hit(ray_bounce, scene, t_hit, n_hit);
		if (!surf_hit) {
			continue;
		}

		Vec3 p_hit = ray_bounce.at(t_hit);
		Vec3 dir_view_hit = ray_bounce.dir * -1;

		// Direct lighting
		Color c_direct = direct_lighting(surf_hit, scene, p_hit, n_hit, dir_view_hit);

		// Recursive indirect lighting
		Color c_indirect = indirect_lighting(surf_hit, scene, p_hit, n_hit, dir_view_hit, n_bounce - 1);

		// Specular
		float specular = brdf(surface->material, ray_bounce.dir, dir_view, normal);
		c_out += (c_direct + c_indirect) * specular / static_cast<float>(rays_per_bounce);
	}

	return c_out;
}

float RayTracer::brdf(const Material& mat, const Vec3& dir_light, const Vec3& dir_view, const Vec3& normal) const
{
	// Half-angle vector
	const Vec3 h = (dir_light + dir_view).normalized();

	const float vn = dir_view.dot(normal);
	const float ln = dir_light.dot(normal);
	const float hn = h.dot(normal);
	const float vh = dir_view.dot(h);

	// GGX normal distribution function
	const float a2 = mat.roughness * mat.roughness;
	const float d = (a2 - 1.f) * hn * hn + 1.f;
	const float ggx = a2 / (d * d * 3.14f);

	// Fresnel factor - Schlick's approximation
	const float r0_sqrt = (1.f - mat.ior) / (1.f + mat.ior);
	const float r0 = r0_sqrt * r0_sqrt;
	const float fresnel = r0 + (1.f - r0) * std::pow(1.f - vh, 5.f);

	// Geometric attenuation
	const float shadowing_view = 2.f * hn * vn / vh;
	const float shadowing_light = 2.f * hn * ln / vh;
	const float shadowing = std::min(1.f, std::min(shadowing_view, shadowing_light));

	// Cook-Torrance model
	return (ggx * fresnel * shadowing) / (4.f * dir_view.dot(normal) * dir_light.dot(normal));
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

				// View direction
				Vec3 dir_view = ray.dir * -1;

				// Surface color at hit point (to compute)
				Color c_sample(0);

				// Direct lighting
				c_sample += direct_lighting(surface, scene, pos, normal, dir_view);

				// Indirect lighting
				c_sample += indirect_lighting(surface, scene, pos, normal, dir_view, max_bounce);

				// Add sample contribution
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

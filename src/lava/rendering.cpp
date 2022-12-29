#include <lava/rendering.hpp>

#include <spdlog/spdlog.h>

#include <chrono>
#include <random>


namespace lava {

RenderParams::RenderParams(int sampling) : 
	pixel_sampling(sampling)
{}

RenderLayers::RenderLayers(int w, int h) : 
	image(w, h)
{}

Ray cast(std::shared_ptr<Camera> camera, float x, float y, float aspect_ratio)
{
	Vec3 pixel_pos = camera->location
		+ camera->forward * camera->sensor_width / std::tan(.5f * camera->field_of_view)
		+ camera->right * x * camera->sensor_width
		+ camera->up * y * camera->sensor_width * aspect_ratio;
	return trace(camera->location, pixel_pos);
}

std::shared_ptr<Surface> hit(const Ray& ray, const Scene& scene,
							 float& t, Vec3& normal)
{
	std::shared_ptr<Surface> surface = nullptr;

	for (auto s : scene.surfaces()) {
		float t_local = 0.f;
		Vec3 n_local;
		if (!s->hit(ray, t_local, n_local)) {
			continue;
		}
		
		if (!surface || (surface && t_local < t)) {
			surface = s;
			t = t_local;
			normal = n_local;
		}
	}

	return surface;
}

void render(const Scene& scene,
			const RenderParams& params, 
			RenderLayers& layers, 
			std::function<void(float)> progress_callback)
{
	spdlog::info("start rendering");
	auto time_start = std::chrono::steady_clock::now();

	const int width = layers.image.width();
	const int height = layers.image.height();
	spdlog::info("dimensions: {}x{}", width, height);

	const float f_width = static_cast<float>(width);
	const float f_height = static_cast<float>(height);
	const float aspect_ratio = f_height / f_width;

	int progress_counter = 0;
	progress_callback(0);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.f, 1.f);

	for (int j = 0; j < width; j++) {
		for (int i = 0; i < height; i++) {
			const float x = (static_cast<float>(j) / f_width) - .5f;
			const float y = .5f - (static_cast<float>(i) / f_height);

			Color c_out;

			for (int k = 0; k < params.pixel_sampling; k++) {
				const float dx = dis(gen) / f_width;
				const float dy = dis(gen) / f_height;

				const Ray ray = cast(scene.camera(), x + dx, y + dy, aspect_ratio);

				float t = 0.f;
				Vec3 normal;
				auto surface = hit(ray, scene, t, normal);
				if (!surface) {
					continue;
				}

				Vec3 pos = ray.at(t);
				Vec3 dir_view = ray.dir * -1;

				Color c_sample;

				for (auto light : scene.lights()) {
					Vec3 dir_light;
					float dist_light = 0.f;
					float intensity = 0.f;
					light->sample(pos, dir_light, dist_light, intensity);

					Ray r_light(pos, dir_light);
					float t_obstruct = 0.f;
					Vec3 n_obstruct;
					auto s_obstruct = hit(r_light, scene, t_obstruct, n_obstruct);
					if (s_obstruct && t_obstruct > 1e-3 && t_obstruct < dist_light) {
						continue;
					}

					float diffuse = std::max(0.f, dot(normal, dir_light)) * (surface->material).k_diffuse / 3.14f;

					Vec3 dir_reflected = normal * dot(normal, dir_light) * 2 - dir_light;
					float alpha = 1.f / std::max((surface->material).roughness, 1e-3f);
					float specular = std::pow(std::max(0.f, dot(dir_view, dir_reflected)), alpha) * (surface->material).k_specular;

					c_sample = (surface->material).base_color * ((surface->material).k_ambient + diffuse * intensity)
							 + light->color * specular * intensity;
					c_sample.r = std::min(c_sample.r, 1.f);
					c_sample.g = std::min(c_sample.g, 1.f);
					c_sample.b = std::min(c_sample.b, 1.f);
				}

				c_out += c_sample;
			}

			c_out /= static_cast<float>(params.pixel_sampling);

			layers.image.set(i, j, c_out);

			progress_counter++;
			if((progress_counter * 10) % (width * height) == 0) {
				float progress = static_cast<float>(progress_counter) / static_cast<float>(width * height);
				progress_callback(progress);
			}
		}
	}

	auto time_end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = time_end - time_start;
	spdlog::info("rendering done in {}s", elapsed_seconds.count());
}

}

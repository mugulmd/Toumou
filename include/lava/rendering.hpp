#pragma once

#include <lava/scene.hpp>
#include <lava/image.hpp>
#include <lava/geometry.hpp>
#include <lava/color.hpp>

#include <functional>
#include <memory>
#include <random>


namespace lava {

class RayTracer {
public:
	
	/// Number of rays per pixel.
	int pixel_sampling;

	/// Maximum number of bounces for each light path.
	int max_bounce;

	/// Number of rays emitted at each bounce.
	int rays_per_bounce;

	/// RGB layer.
	Image<Color> image;

	RayTracer(int w, int h, int sampling = 16, int bounce = 4, int split = 16);

	void render(const Scene& scene, std::function<void(int)> progress_callback);

private:

	mutable std::mt19937 m_gen;

	mutable std::uniform_real_distribution<float> m_dis;
	
	Ray cast(std::shared_ptr<Camera> camera, float x, float y, float aspect_ratio) const;

	Ray cast(const Vec3& pos, const Vec3& normal, float theta, float phi) const;

	std::shared_ptr<Surface> hit(const Ray& ray, const Scene& scene, float& t, Vec3& normal) const;

	Color direct_lighting(std::shared_ptr<Surface> surface, const Scene& scene, const Vec3& pos, const Vec3& normal) const;

	Color indirect_lighting(std::shared_ptr<Surface> surface, const Scene& scene, const Vec3& pos, const Vec3& normal, int n_bounce) const;


};

}

#pragma once

#include <toumou/scene.hpp>
#include <toumou/image.hpp>
#include <toumou/geometry.hpp>
#include <toumou/color.hpp>

#include <functional>
#include <memory>
#include <random>


namespace toumou {

/**
 * @brief Ray tracing engine. Currently supports only diffuse lighting (direct and indirect).
 */
class RayTracer {
public:
	
	/// Number of rays per pixel.
	int pixel_sampling;

	/// Maximum number of bounces for each light path.
	int max_bounce;

	/// Number of rays emitted at each bounce.
	int rays_per_bounce;

	/// Color pass.
	Image<Color> image;

	// Normal pass.
	Image<Vec3> normal_map;

	// Depth pass.
	Image<float> depth_map;

	// Surface UID pass.
	Image<float> index_map;

	/**
	 * @brief Create a ray tracer object for the given output dimensions.
	 * @param[in] w Output image width.
	 * @param[in] h Output image height.
	 * @param[in] sampling Pixel sampling.
	 * @param[in] bounce Maximum number of bounces.
	 * @param[in] split Number of rays per bounce.
	 */
	RayTracer(int w, int h, int sampling = 16, int bounce = 4, int split = 16);

	/**
	 * @brief Ray trace a given 3D scene.
	 * @param[in] scene Scene to render.
	 * @param[in] progress_callback Function called everytime the computations progress by one percent of the total workload.
	 */
	void render(const Scene& scene, std::function<void(int)> progress_callback);

private:

	/// Pseudo-random number generation.
	mutable std::mt19937 m_gen;
	mutable std::uniform_real_distribution<float> m_dis;
	
	/// Trace a ray from a camera's origin to a position on the image plane.
	Ray cast(std::shared_ptr<Camera> camera, float x, float y, float aspect_ratio) const;

	/// Trace a ray from a surface point using spherical coordinates in the hemisphere oriented by the surface normal.
	Ray cast(const Vec3& pos, const Vec3& normal, float theta, float phi) const;

	/// Find first surface in the scene hit by a given ray. 
	std::shared_ptr<Surface> hit(const Ray& ray, const Scene& scene, float& t, Vec3& normal) const;

	/// Compute direct lighting at a given surface point.
	Color direct_lighting(std::shared_ptr<Surface> surface, const Scene& scene, const Vec3& pos, const Vec3& normal) const;

	/// Compute indirect lighting at a given surface point.
	Color indirect_lighting(std::shared_ptr<Surface> surface, const Scene& scene, const Vec3& pos, const Vec3& normal, int n_bounce) const;


};

}

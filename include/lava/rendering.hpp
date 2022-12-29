#pragma once

#include <lava/scene.hpp>
#include <lava/image.hpp>

#include <functional>
#include <memory>


namespace lava {

/**
 * @brief A struct the specify the parameters of a render operation.
 */
struct RenderParams {

	/// Number of rays per pixel.
	int pixel_sampling;

	/// Utility constructor with parameter initialization.
	RenderParams(int sampling = 16);

};

/**
 * @brief A struct to initialize and store the layers in which the render operation will write.
 */
struct RenderLayers {

	/// RGB layer.
	Image<Color> image;

	/// Utility constructor to create all layers at once with the same dimensions.
	RenderLayers(int w, int h);

};

/**
 * @brief Cast a ray from a camera's eye to a given point on its sensor.
 * @see Camera
 * @see Ray
 * @param[in] camera The camera providing all the parameters (eye position, sensor width, field of view, etc.).
 * @param[in] x The X-coordinate of the point on the sensor.
 * @param[in] y the Y-coordinate of the point on the sensor.
 * @param[in] aspect_ratio The sensor's aspect ratio.
 * @return A ray starting at the camera's eye and going through the given point on the sensor.
 */
Ray cast(std::shared_ptr<Camera> camera, float x, float y, float aspect_ratio);

/**
 * @brief Find the first intersection point (hit) between a ray and the surfaces of a scene.
 * @see Scene
 * @see Surface
 * @see Ray
 * @param[in] ray The ray to check for intersections.
 * @param[in] scene The scene containing the surfaces.
 * @param[out] t The distance between the ray's origin and the hit (if a hit has been found).
 * @param[out] normal The surface normal at the hit (if a hit has been found).
 * @return A shared pointer to the surface first intersected by the ray if it exists, otherwise nullptr.
 */
std::shared_ptr<Surface> hit(const Ray& ray, const Scene& scene, 
							 float& t, Vec3& normal);

/**
 * @brief Render a given scene.
 * @see Scene
 * @see RenderParams
 * @see RenderLayers
 * @param[in] scene The scene to render.
 * @param[in] params The parameters of the render operation.
 * @param[out] layers The 2D layers where the rendering results are stored.
 * @param progress_callback A function called regularly as computations progress.
 */
void render(const Scene& scene, 
			const RenderParams& params, 
			RenderLayers& layers, 
			std::function<void(float)> progress_callback);

}

#pragma once

#include <lava/color.hpp>
#include <lava/geometry.hpp>

#include <memory>


namespace lava {

/**
 * @brief Abstract class for light source models.
 */
class Light {
public:

	/// Amount of energy provided by the light source.
	float brightness;

	/// Light source color.
	/// For now light sources only provide one color, 
	/// this will change when we add environment lights.
	Color color;

	/**
	 * @brief Sample the light source contribution at a given position in 3D space.
	 * @param[in] pos Position where the light source contribution is to be sampled.
	 * @param[out] dir Light direction at the given position.
	 * @param[out] dist Distance to the light source at the given position.
	 * @param[out] intensity Light intensity at the given position.
	 */
	virtual void sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const = 0;

};

/**
 * @brief Point light source model (a.k.a delta light).
 */
class PointLight : public Light {
public:
	
	/// Position of the light source in 3D space.
	Vec3 location;

	void sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const override;

};

/**
 * @brief Initialize a point light source.
 * @param[in] location Position where light is emitted from.
 * @param[in] brightness Brightness of the light source.
 * @param[in] color Color of the light source.
 * @return Shared pointer to a point light source model initialized with the given parameters.
 */
std::shared_ptr<PointLight> make_point_light(const Vec3& location, float brightness, const Color& color);

/**
 * @brief Directional light source model.
 */
class DirectionalLight : public Light {
public:
	
	/// Direction of the light source.
	Vec3 direction;

	void sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const override;

};

/**
 * @brief Constructs and initializes a directional light source.
 * @param[in] direction Direction of the light rays.
 * @param[in] brightness Brightness of the light source.
 * @param[in] color Color of the light source.
 * @return Shared pointer to a directional light source model initialized with the given parameters.
 */
std::shared_ptr<DirectionalLight> make_directional_light(const Vec3& direction, float brightness, const Color& color);

}

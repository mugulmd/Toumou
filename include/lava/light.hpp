#pragma once

#include <lava/color.hpp>
#include <lava/geometry.hpp>

#include <memory>


namespace lava {

/**
 * @brief An abstract class to define and manipulate light source models.
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
	 * @param[in] pos The position where the light source contribution is to be sampled.
	 * @param[out] dir The light direction at the given position.
	 * @param[out] dist The distance to the light source at the given position.
	 * @param[out] intensity The light intensity at the given position.
	 */
	virtual void sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const = 0;

};

/**
 * @brief A class to define and manipulate a point light source model (a.k.a delta light).
 */
class PointLight : public Light {
public:
	
	/// Position of the light source in 3D space.
	Vec3 location;

	void sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const override;

};

/**
 * @brief Constructs and initializes a point light source.
 * @param[in] location The position from where light is emitted.
 * @param[in] brightness The brightness of the light source.
 * @param[in] color The color of the light source.
 * @return A shared pointer to a point light source model initialized with the given parameters.
 */
std::shared_ptr<PointLight> make_point_light(const Vec3& location, 
											 float brightness, const Color& color);

/**
 * @brief A class to define and manipulate a directional light source model.
 */
class SunLight : public Light {
public:
	
	/// Direction of the light source.
	Vec3 direction;

	void sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const override;

};

/**
 * @brief Constructs and initializes a directional light source.
 * @param[in] direction The direction of the light rays.
 * @param[in] brightness The brightness of the light source.
 * @param[in] color The color of the light source.
 * @return A shared pointer to a directional light source model initialized with the given parameters.
 */
std::shared_ptr<SunLight> make_sun_light(const Vec3& direction, 
										 float brightness, const Color& color);

}

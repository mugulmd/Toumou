#pragma once

#include <toumou/color.hpp>
#include <toumou/geometry.hpp>


namespace toumou {

/**
 * @brief Abstract class for light source models.
 */
class Light {
public:

	/**
	 * @brief TODO
	 */
	Light(const Color& _color, float _brightness);

	/// Light source color.
	/// For now light sources only provide one color, 
	/// this will change when we add environment lights.
	Color color;

	/// Amount of energy provided by the light source.
	float brightness;

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

	/**
	 * @brief TODO
	 */
	PointLight(const Color& _color, float _brightness, const Vec3& _location);
	
	/// Position of the light source in 3D space.
	Vec3 location;

	void sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const override;

};

/**
 * @brief Directional light source model.
 */
class DirectionalLight : public Light {
public:

	/**
	 * @brief TODO
	 */
	DirectionalLight(const Color& _color, float _brightness, const Vec3& _direction);
	
	/// Direction of the light source.
	Vec3 direction;

	void sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const override;

};

}

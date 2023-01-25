#pragma once

#include <toumou/color.hpp>
#include <toumou/field.hpp>
#include <toumou/geometry.hpp>
#include <toumou/macros.hpp>

#include <memory>


namespace toumou {

/**
 * @brief Simple diffuse material.
 */
struct Material {

	/// Amount of light transmitted by diffuse lighting.
	float albedo = .18f;

	/**
	 * @brief TODO
	 */
	Color color_at(const Vec3& pos) const;

	/**
	 * @brief TODO
	 */
	void set_solid_color(const Color& color);

	/// TODO
	std::shared_ptr<Field> red = tmks(Constant, 1.f);

	/// TODO
	std::shared_ptr<Field> green = tmks(Constant, 1.f);

	/// TODO
	std::shared_ptr<Field> blue = tmks(Constant, 1.f);

};

}

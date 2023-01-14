#pragma once

#include <lava/color.hpp>


namespace lava {

/**
 * @brief A struct to define and manipulate the paramaters of a material (using the Phong reflection model).
 */
struct Material {

	/// Material color.
	Color base_color = Color(.3f, .3f, .3f);

	/// Amount of light transmitted by diffuse lighting.
	float albedo = .18f;

};

}

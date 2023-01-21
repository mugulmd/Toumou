#pragma once

#include <toumou/color.hpp>


namespace toumou {

/**
 * @brief Simple diffuse material.
 */
struct Material {

	/// Material color.
	Color base_color = Color(.3f, .3f, .3f);

	/// Amount of light transmitted by diffuse lighting.
	float albedo = .18f;

};

}
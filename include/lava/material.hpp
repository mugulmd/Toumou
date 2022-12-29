#pragma once

#include <lava/color.hpp>


namespace lava {

/**
 * @brief A struct to define and manipulate the paramaters of a material (using the Phong reflection model).
 */
struct Material {

	/// Material color.
	Color base_color = Color(.3f, .3f, .3f);

	/// Ambient contribution term.
	float k_ambient = 0.1f;

	/// Diffuse contribution term.
	float k_diffuse = .18f;

	/// Specular contribution term.
	float k_specular = .1f;

	/// Roughness term, interpreted here as the inverse of shininess.
	float roughness = .5f;

};

}

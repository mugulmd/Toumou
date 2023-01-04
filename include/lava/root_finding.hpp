#pragma once

#include <lava/geometry.hpp>

#include <functional>


namespace lava {

/**
 * @brief A struct that implements a root finding algorithm for detecting the zeros of a 3D field along a ray.
 * 
 * To compute the intersection between a ray and an implicit surface in the general case (when there is no close formula)
 * we use the 3D field that defines the given implicit surface and we apply the following root finding algorithm: 
 * 1. sample the field along the ray with a fixed step to find an interval on which the field sign changes
 * 2. apply Newton's algorithm on this interval to refine the root value
 */
struct RootFinder {

	/// Start point of the search range.
	float t_min = 0.f;

	/// End point of the search range.
	float t_max = 10.f;

	/// Step used in the 1st pass to sample the field along the ray.
	float sampling_step = .1f;

	/// Criteria for determining if we are close enough to a solution.
	float threshold = 1e-3f;

	/// Maximum number of iterations for the refinement pass.
	int max_iterations = 10;

	/**
	 * @brief Find the first point along a ray at which a field evaluates to zero.
	 * @param[in] ray The ray on which we are looking for a root.
	 * @param[in] field 3D field describing an implicit surface.
	 * @param[in] ray_derivative The derivative of the field restricted to a ray.
	 * @param[out] t_root The estimated root position along the ray.
	 * @return Whether or not a root was found.
	 */
	bool find_first_root(const Ray& ray,
						 std::function<float(const Vec3&)> field,
						 std::function<float(const Ray&, float)> ray_derivative,
						 float& t_root) const;

};

}

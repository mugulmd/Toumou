#pragma once

#include <Imath/ImathVec.h>


namespace toumou {

/**
 * @brief 3D vector with float precision.
 */
using Vec3 = Imath::V3f;

/**
 * @brief Half-line in 3D space.
 */
struct Ray {

	/// Starting point of the ray.
	Vec3 origin;

	/// Direction of the ray (normalized).
	Vec3 dir;

	/**
	 * @brief Constructor with parameter initilization.
	 * @param[in] _origin Starting point of the ray.
	 * @param[in] _dir Direction of the ray (must be normalized).
	 */
	Ray(const Vec3& _origin, const Vec3& _dir);

	/**
	 * @brief Retrieve the position of a point on this ray.
	 * @param[in] t Distance between between the ray's origin and the position to compute (must be positive).
	 * @return Position of the queried point along the ray.
	 */
	Vec3 at(float t) const;

};

/**
 * @brief Trace a ray going through two points in 3D space.
 * @param[in] from Starting point of the ray to trace.
 * @param[in] to Destination point which the ray must go through.
 * @return A ray starting at the first given point and passing through the second one.
 */
Ray trace(const Vec3& from, const Vec3& to);

}

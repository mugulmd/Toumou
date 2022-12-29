#pragma once

#include <lava/geometry.hpp>

#include <memory>


namespace lava {

/**
 * @brief A struct to store and manipulate the parameters of a pinhole camera model.
 * @see make_camera
 */
struct Camera {

	/// Position in 3D space.
	Vec3 location;

	/// Local orthonormal coordinate system.
	Vec3 forward, up, right;

	/// Distance between the left and right side of the sensor (in metres).
	float sensor_width;

	/// Angle between the eye of the camera and the left and right side of the sensor (in radians).
	float field_of_view;

	/// Depth range in which objects are visible (in metres).
	float z_near, z_far;
	
};

/**
 * @brief Constructs and initializes a camera.
 * @param[in] location Camera position in 3D space.
 * @param[in] forward Direction in which the camera is looking (must be normalized).
 * @param[in] up Camera orientation along the viewing axis (must be normalized).
 * @param[in] sensor_width Width of the camera sensor (in millimeters).
 * @param[in] field_of_view Angle of view (in degrees).
 * @param[in] z_near Start point of visibility range (in meters).
 * @param[in] z_far End point of visibility range (in meters).
 * @return A shared pointer to a camera model initialized with the given parameters.
 */
std::shared_ptr<Camera> make_camera(const Vec3& location, 
									const Vec3& forward, const Vec3& up, 
									float sensor_width, float field_of_view, 
									float z_near, float z_far);

}

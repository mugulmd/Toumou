#pragma once

#include <toumou/geometry.hpp>


namespace toumou {

/**
 * @brief Pinhole camera model.
 */
class Camera {
public:

	/**
	 * @brief TODO
	 */
	Camera(float sw, float fov, float zn, float zf);

	/// Distance between the left and right side of the sensor (in metres).
	float sensor_width;

	/// Angle between the eye of the camera and the left and right side of the sensor (in radians).
	float field_of_view;

	/// Depth range in which objects are visible (in metres).
	float z_near, z_far;

	/**
	 * @brief TODO
	 */
	void move_to(const Vec3& pos);

	/**
	 * @brief TODO
	 */
	Vec3 location() const;

	/**
	 * @brief TODO
	 */
	Vec3 forward() const;

	/**
	 * @brief TODO
	 */
	Vec3 up() const;

	/**
	 * @brief TODO
	 */
	Vec3 left() const;

private:

	// TODO: use a 4x4 transform matrix instead
	Vec3 m_location;
	Vec3 m_forward = Vec3(0, 0, -1);
	Vec3 m_up = Vec3(0, 1, 0);
	Vec3 m_left = Vec3(-1, 0, 0);
	
};

}

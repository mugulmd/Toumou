#pragma once

#include <toumou/geometry.hpp>

#include <Imath/ImathMatrix.h>


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
	void rotate_to(const Vec3& angles);

	/**
	 * @brief TODO
	 */
	const Vec3& location() const;

	/**
	 * @brief TODO
	 */
	const Vec3& forward() const;

	/**
	 * @brief TODO
	 */
	const Vec3& up() const;

	/**
	 * @brief TODO
	 */
	const Vec3& left() const;

private:

	/// TODO
	Vec3 m_location;

	/// TODO
	Vec3 m_forward;

	/// TODO
	Vec3 m_up;

	/// TODO
	Vec3 m_left;

	/// TODO
	Imath::M44f m_transform;

	/**
	 * @brief TODO
	 */
	void set_transform(const Imath::M44f& tr);
	
};

}

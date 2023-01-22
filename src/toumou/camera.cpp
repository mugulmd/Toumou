#include <toumou/camera.hpp>


namespace toumou {

Camera::Camera(float sw, float fov, float zn, float zf) :
	sensor_width(sw * 1e-3f),
	field_of_view(fov * 3.14f / 180.f),
	z_near(zn), z_far(zf)
{
}

void Camera::move_to(const Vec3& pos)
{
	m_location = pos;
}

Vec3 Camera::location() const
{
	return m_location;
}

Vec3 Camera::forward() const
{
	return m_forward;
}

Vec3 Camera::up() const
{
	return m_up;
}

Vec3 Camera::left() const
{
	return m_left;
}

}

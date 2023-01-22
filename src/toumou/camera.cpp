#include <toumou/camera.hpp>

#include <Imath/ImathMatrixAlgo.h>


namespace toumou {

Camera::Camera(float sw, float fov, float zn, float zf) :
	sensor_width(sw * 1e-3f),
	field_of_view(fov * 3.14f / 180.f),
	z_near(zn), z_far(zf)
{
	set_transform(Imath::M44f());
}

void Camera::move_to(const Vec3& pos)
{
	Imath::M44f tr = m_transform;
	tr.translate(pos - m_location);
	set_transform(tr);
}

void Camera::rotate_to(const Vec3& angles)
{
	Vec3 current_angles;
	Imath::extractEulerXYZ(m_transform, current_angles);
	Imath::M44f tr = m_transform;
	tr.rotate(angles - current_angles);
	set_transform(tr);
}

const Vec3& Camera::location() const
{
	return m_location;
}

const Vec3& Camera::forward() const
{
	return m_forward;
}

const Vec3& Camera::up() const
{
	return m_up;
}

const Vec3& Camera::left() const
{
	return m_left;
}

void Camera::set_transform(const Imath::M44f& tr)
{
	m_transform = tr;
	tr.multVecMatrix(m_location, m_location);
	tr.multDirMatrix(m_forward, m_forward);
	tr.multDirMatrix(m_up, m_up);
	tr.multDirMatrix(m_left, m_left);
}

}

#include <toumou/geometry.hpp>

#include <cmath>


namespace toumou {

Vec3::Vec3() : x(0), y(0), z(0) 
{}

Vec3::Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
{}

bool Vec3::operator==(const Vec3& other) const
{
	return x == other.x && y == other.y && z == other.z;
}

Vec3 Vec3::operator+(const Vec3& other) const
{
	return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3& other) const
{
	return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(const float f) const
{
	return Vec3(x * f, y * f, z  * f);
}

Vec3 Vec3::operator/(const float f) const
{
	return Vec3(x / f, y / f, z  / f);
}

Vec3& Vec3::operator=(const Vec3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

Vec3& Vec3::operator+=(const Vec3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vec3& Vec3::operator-=(const Vec3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vec3& Vec3::operator*=(const float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Vec3& Vec3::operator/=(const float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

float Vec3::norm2() const
{
	return x*x + y*y + z*z;
}

float Vec3::norm() const
{
	return std::sqrt(norm2());
}

void Vec3::normalize()
{
	*this /= norm();
}

Vec3 Vec3::normalized() const
{
	return *this / norm();
}

float distance(const Vec3& lhs, const Vec3& rhs)
{
	return (rhs - lhs).norm();
}

float dot(const Vec3& lhs, const Vec3& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vec3 cross(const Vec3& lhs, const Vec3& rhs)
{
	return Vec3(
		lhs.y * rhs.z - lhs.z * rhs.y, 
		lhs.z * rhs.x - lhs.x * rhs.z, 
		lhs.x * rhs.y - lhs.y * rhs.x
	);
}

Ray::Ray(const Vec3& _origin, const Vec3& _dir) : 
	origin(_origin), dir(_dir)
{}

Vec3 Ray::at(float t) const
{
	return origin + dir * t;
}

Ray trace(const Vec3& from, const Vec3& to)
{
	return Ray(from, (to - from).normalized());
}

}

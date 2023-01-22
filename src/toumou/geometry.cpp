#include <toumou/geometry.hpp>

#include <cmath>


namespace toumou {

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

#include <toumou/light.hpp>

#include <limits>


namespace toumou {

Light::Light(const Color& _color, float _brightness) : 
	color(_color), brightness(_brightness)
{
}

PointLight::PointLight(const Color& _color, float _brightness, const Vec3& _location) : 
	Light(_color, _brightness),
	location(_location)
{
}

void PointLight::sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const
{
	dir = (location - pos).normalized();
	dist = distance(pos, location);
	intensity = brightness / (dist * dist);
}

DirectionalLight::DirectionalLight(const Color& _color, float _brightness, const Vec3& _direction) : 
	Light(_color, _brightness),
	direction(_direction)
{
}

void DirectionalLight::sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const
{
	dir = direction;
	dist = std::numeric_limits<float>::max();
	intensity = brightness;
}

}
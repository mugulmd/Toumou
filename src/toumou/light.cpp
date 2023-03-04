#include <toumou/light.hpp>
#include <toumou/constants.hpp>

#include <limits>
#include <cmath>
#include <algorithm>


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
	dist = (pos - location).length();
	float dist2 = std::max(dist * dist, eps_div_by_zero);
	intensity = brightness / dist2;
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

EnvironmentLight::EnvironmentLight(const Color& _color, float _brightness) :
	color(_color), brightness(_brightness)
{
}

void EnvironmentLight::sample(const Vec3& dir, Color& c_sample, float& intensity) const
{
	c_sample = color;
	intensity = brightness;
}

PanoramaLight::PanoramaLight(const Image<Color>& _image, float _brightness) :
	EnvironmentLight(Color(0), _brightness),
	image(_image)
{
}

void PanoramaLight::sample(const Vec3& dir, Color& c_sample, float& intensity) const
{
	// Equirectangular projection
	const float theta = std::acos(dir.y / dir.length());
	const float sgn = dir.z < 0 ? -1.f : 1.f;
	const float phi = sgn * std::acos(dir.x / std::sqrt(dir.x * dir.x + dir.z * dir.z));
	const float x = ((phi / k_pi) + 1.f) * .5f * static_cast<float>(image.width());
	const float y = (theta / k_pi) * static_cast<float>(image.height());
	const int i = std::clamp(static_cast<int>(y), 0, image.height() - 1);
	const int j = std::clamp(static_cast<int>(x), 0, image.width() - 1);
	c_sample = image.at(i, j);
	intensity = brightness;
}

}

#include <lava/light.hpp>

#include <limits>


namespace lava {

void PointLight::sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const
{
	dir = (location - pos).normalized();
	dist = distance(pos, location);
	intensity = brightness / (dist * dist);
}

std::shared_ptr<PointLight> make_point_light(const Vec3& location, 
											 float brightness, const Color& color)
{
	auto light = std::make_shared<PointLight>();
	light->location = location;
	light->brightness = brightness;
	light->color = color;
	return light;
}

void SunLight::sample(const Vec3& pos, Vec3& dir, float& dist, float& intensity) const
{
	dir = direction;
	dist = std::numeric_limits<float>::max();
	intensity = brightness;
}

std::shared_ptr<SunLight> make_sun_light(const Vec3& direction, 
										 float brightness, const Color& color)
{
	auto light = std::make_shared<SunLight>();
	light->direction = direction;
	light->brightness = brightness;
	light->color = color;
	return light;
}

}

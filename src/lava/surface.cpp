#include <lava/surface.hpp>

#include <cmath>


namespace lava {

unsigned int Surface::uid_counter = 0;

Surface::Surface()
{
	m_uid = ++Surface::uid_counter;
}

unsigned int Surface::uid() const
{
	return m_uid;
}

bool Sphere::hit(const Ray& ray, float& t, Vec3& n) const
{
	float b = 2 * dot(ray.dir, ray.origin - center);
	float c = (ray.origin - center).norm2() - radius*radius;

	float delta = b*b - 4*c;
	if (delta < 0) {
		return false;
	}

	t = (-b - std::sqrt(delta)) * .5f;
	if (t < 0) {
		return false;
	}

	n = (ray.at(t) - center).normalized();
	return true;
}

std::shared_ptr<Sphere> make_sphere(const Vec3& center, float radius)
{
	auto surface = std::make_shared<Sphere>();
	surface->center = center;
	surface->radius = radius;
	return surface;
}

bool Plane::hit(const Ray& ray, float& t, Vec3& n) const
{
	float alpha = dot(origin - ray.origin, normal);
	float beta = dot(ray.dir, normal);

	if (std::abs(beta) < 1e-5) {
		return false;
	}

	t = alpha / beta;
	if (t < 0) {
		return false;
	}

	n = normal;
	return true;
}

std::shared_ptr<Plane> make_plane(const Vec3& origin, const Vec3& normal)
{
	auto surface = std::make_shared<Plane>();
	surface->origin = origin;
	surface->normal = normal;
	return surface;
}

}

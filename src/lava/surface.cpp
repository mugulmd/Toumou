#include <lava/surface.hpp>

#include <cmath>
#include <algorithm>


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

void Metaball::add_sphere(std::shared_ptr<Sphere> sphere)
{
	m_spheres.push_back(sphere);
}

bool Metaball::hit(const Ray& ray, float& t, Vec3& n) const
{
	bool found_root = root_finder.find_first_root(ray,
		[this](const Vec3& pos) -> float {
			return value_at(pos) - 1.f;
		},
		[this](const Ray& ray, float t) -> float {
			return ray_derivative_at(ray, t);
		},
		t);

	if (!found_root) {
		return false;
	}

	n = normal_at(ray.at(t));

	return true;
}

float Metaball::value_at(const Vec3& pos) const
{
	float value = 0.f;
	for (auto sphere : m_spheres) {
		float dist2 = (pos - sphere->center).norm2();
		dist2 = std::max(dist2, 1e-3f);
		value += (sphere->radius * sphere->radius) / dist2;
	}
	return value;
}

float Metaball::ray_derivative_at(const Ray& ray, float t) const
{
	float derivative = 0.f;
	for (auto sphere : m_spheres) {
		Vec3 pos = ray.at(t);
		Vec3 delta = pos - sphere->center;
		float dist2 = delta.norm2();
		dist2 = std::max(dist2, 1e-3f);
		derivative += (sphere->radius * sphere->radius) * (t + dot(ray.origin - sphere->center, ray.dir)) / (dist2 * dist2);
	}
	derivative *= -2.f;
	return derivative;
}

Vec3 Metaball::normal_at(const Vec3& pos) const
{
	Vec3 normal;
	for (auto sphere : m_spheres) {
		Vec3 delta = pos - sphere->center;
		float dist2 = delta.norm2();
		dist2 = std::max(dist2, 1e-3f);
		normal += delta * (sphere->radius * sphere->radius) / (dist2 * dist2);
	}
	normal.normalize();
	return normal;
}

std::shared_ptr<Metaball> make_metaball()
{
	auto metaball = std::make_shared<Metaball>();
	return metaball;
}

}

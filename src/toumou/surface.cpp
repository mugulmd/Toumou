#include <toumou/surface.hpp>
#include <toumou/constants.hpp>
#include <toumou/macros.hpp>

#include <cmath>
#include <algorithm>


namespace toumou {

unsigned int Surface::uid_counter = 0;

Surface::Surface()
{
	m_uid = ++Surface::uid_counter;
}

unsigned int Surface::uid() const
{
	return m_uid;
}

ImplicitSurface::ImplicitSurface(std::shared_ptr<Field> _field) :
	Surface(),
	field(_field)
{
}

bool ImplicitSurface::hit(const Ray& ray, float& t, Vec3& n) const
{
	bool found_root = root_estimator.find_first_root(ray,
		[this](const Vec3& pos) -> float {
			return field->value(pos) - 1.f;
		},
		[this](const Ray& ray, float t) -> float {
			return field->ray_derivative(ray, t);
		},
		t);

	if (!found_root) {
		return false;
	}

	n = field->gradient(ray.at(t)) * -1;
	n.normalize();

	return true;
}

Sphere::Sphere(const Vec3& _center, float _radius) :
	center(_center), radius(_radius)
{
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

Plane::Plane(const Vec3& _origin, const Vec3& _normal) :
	origin(_origin), normal(_normal)
{
}

bool Plane::hit(const Ray& ray, float& t, Vec3& n) const
{
	float alpha = dot(origin - ray.origin, normal);
	float beta = dot(ray.dir, normal);

	if (std::abs(beta) < eps_div_by_zero) {
		return false;
	}

	t = alpha / beta;
	if (t < 0) {
		return false;
	}

	n = normal;
	return true;
}

Tube::Tube(const Vec3& _origin, const Vec3& _direction, float _radius) : 
	origin(_origin), direction(_direction), radius(_radius)
{
}

bool Tube::hit(const Ray& ray, float& t, Vec3& n) const
{
	Vec3 v1 = ray.dir - direction * dot(ray.dir, direction);
	Vec3 v2 = ray.origin - origin - direction * dot(ray.origin - origin, direction);
	float a = v1.norm2();
	float b = 2.f * dot(v1, v2);
	float c = v2.norm2() - (radius * radius);
	float delta = b * b - 4.f * a * c;
	if (delta < 0) {
		return false;
	}

	float t1 = (-b - std::sqrt(delta)) / (2.f * a);
	float t2 = (-b + std::sqrt(delta)) / (2.f * a);
	t = std::min(t1, t2);
	if (t < 0) {
		return false;
	}

	Vec3 p = ray.at(t);
	Vec3 q = origin + direction * dot(p - origin, direction);
	n = (p - q).normalized();

	return true;
}

}

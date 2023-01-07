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

float ImplicitSurface::ray_derivative(const Ray& ray, float t) const
{
	return (field(ray.at(t + 1e-5f)) - field(ray.at(t - 1e-5f))) * .5f * 1e5f;
}

Vec3 ImplicitSurface::normal(const Vec3& pos) const
{
	Vec3 dx(1e-5f, 0, 0);
	Vec3 dy(0, 1e-5f, 0);
	Vec3 dz(0, 0, 1e-5f);
	float grad_x = (field(pos + dx) - field(pos - dx)) * .5f * 1e5f;
	float grad_y = (field(pos + dy) - field(pos - dy)) * .5f * 1e5f;
	float grad_z = (field(pos + dz) - field(pos - dz)) * .5f * 1e5f;
	return Vec3(-grad_x, -grad_y, -grad_z).normalized();
}

bool ImplicitSurface::hit(const Ray& ray, float& t, Vec3& n) const
{
	bool found_root = root_finder.find_first_root(ray,
		[this](const Vec3& pos) -> float {
			return field(pos) - 1.f;
		},
		[this](const Ray& ray, float t) -> float {
			return ray_derivative(ray, t);
		},
		t);

	if (!found_root) {
		return false;
	}

	n = normal(ray.at(t));

	return true;
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

float Sphere::field(const Vec3& pos) const
{
	float dist2 = (pos - center).norm2();
	dist2 = std::max(dist2, 1e-5f);
	return (radius * radius) / dist2;
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

float Plane::field(const Vec3& pos) const
{
	float signed_dist = dot(pos - origin, normal);
	return std::exp(-signed_dist);
}

std::shared_ptr<Plane> make_plane(const Vec3& origin, const Vec3& normal)
{
	auto surface = std::make_shared<Plane>();
	surface->origin = origin;
	surface->normal = normal;
	return surface;
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

float Tube::field(const Vec3& pos) const
{
	Vec3 delta = pos - origin;
	float lambda = dot(delta, direction);
	float dist2 = delta.norm2() - lambda * lambda;
	dist2 = std::max(dist2, 1e-5f);
	return (radius * radius) / dist2;
}

std::shared_ptr<Tube> make_tube(const Vec3& origin, const Vec3& direction, float radius)
{
	auto surface = std::make_shared<Tube>();
	surface->origin = origin;
	surface->direction = direction;
	surface->radius = radius;
	return surface;
}

void Fusion::add(std::shared_ptr<ImplicitSurface> surface, float coef)
{
	m_surfaces[surface] = coef;
}

float Fusion::field(const Vec3& pos) const
{
	float value = 0.f;
	for (const auto& [surface, coef] : m_surfaces) {
		value += surface->field(pos) * coef;
	}
	return value;
}

std::shared_ptr<Fusion> make_fusion()
{
	auto surface = std::make_shared<Fusion>();
	return surface;
}

}

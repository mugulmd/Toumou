#include <toumou/field.hpp>
#include <toumou/constants.hpp>

#include <algorithm>
#include <cmath>


namespace toumou {

Field::Field()
{
}

Vec3 Field::gradient(const Vec3& pos) const
{
	Vec3 dx(derivation_step, 0, 0);
	Vec3 dy(0, derivation_step, 0);
	Vec3 dz(0, 0, derivation_step);
	float grad_x = (value(pos + dx) - value(pos - dx)) / (2.f * derivation_step);
	float grad_y = (value(pos + dy) - value(pos - dy)) / (2.f * derivation_step);
	float grad_z = (value(pos + dz) - value(pos - dz)) / (2.f * derivation_step);
	return Vec3(grad_x, grad_y, grad_z);
}

float Field::ray_derivative(const Ray& ray, float t) const
{
	return (value(ray.at(t + derivation_step)) - value(ray.at(t - derivation_step))) / (2.f * derivation_step);
}

Fusion::Fusion() : 
	Field()
{
}

void Fusion::add(std::shared_ptr<Field> field, float coef)
{
	m_fields[field] = coef;
}

float Fusion::value(const Vec3& pos) const
{
	float sum = 0.f;
	for (const auto& [field, coef] : m_fields) {
		sum += field->value(pos) * coef;
	}
	return sum;
}

Vec3 Fusion::gradient(const Vec3& pos) const
{
	Vec3 sum;
	for (const auto& [field, coef] : m_fields) {
		sum += field->gradient(pos) * coef;
	}
	return sum;
}

float Fusion::ray_derivative(const Ray& ray, float t) const
{
	float sum = 0.f;
	for (const auto& [field, coef] : m_fields) {
		sum += field->ray_derivative(ray, t) * coef;
	}
	return sum;
}

DistToPoint::DistToPoint(const Vec3& _center) : 
	Field(),
	center(_center)
{
}

float DistToPoint::value(const Vec3& pos) const
{
	return (pos - center).norm2();
}

Vec3 DistToPoint::gradient(const Vec3& pos) const
{
	return (pos - center) * 2.f;
}

float DistToPoint::ray_derivative(const Ray& ray, float t) const
{
	return 2.f * (t + dot(ray.dir, ray.origin - center));
}

DistToLine::DistToLine(const Vec3& _origin, const Vec3& _direction) : 
	Field(),
	origin(_origin), direction(_direction)
{
}

float DistToLine::value(const Vec3& pos) const
{
	Vec3 delta = pos - origin;
	float lambda = dot(delta, direction);
	return delta.norm2() - lambda * lambda;
}

Vec3 DistToLine::gradient(const Vec3& pos) const
{
	Vec3 delta = pos - origin;
	float lambda = dot(delta, direction);
	return (delta - direction * lambda) * 2.f;
}

float DistToLine::ray_derivative(const Ray& ray, float t) const
{
	Vec3 delta = ray.at(t) - origin;
	return (dot(delta, ray.dir) - dot(ray.dir, direction) * dot(delta, direction)) * 2.f;
}

SignedDistToPlane::SignedDistToPlane(const Vec3& _origin, const Vec3& _normal) :
	Field(),
	origin(_origin), normal(_normal)
{
}

float SignedDistToPlane::value(const Vec3& pos) const
{
	return dot(pos - origin, normal);
}

Vec3 SignedDistToPlane::gradient(const Vec3& pos) const
{
	return normal;
}

float SignedDistToPlane::ray_derivative(const Ray& ray, float t) const
{
	return dot(ray.dir, normal);
}

Remapping::Remapping(std::shared_ptr<Field> _field) : 
	Field(),
	input_field(_field)
{
}

float Remapping::derivative(float t) const
{
	return (remap(t + derivation_step) - remap(t - derivation_step)) / (2.f * derivation_step);
}

float Remapping::value(const Vec3& pos) const
{
	return remap(input_field->value(pos));
}

Vec3 Remapping::gradient(const Vec3& pos) const
{
	return input_field->gradient(pos) * derivative(input_field->value(pos));
}

float Remapping::ray_derivative(const Ray& ray, float t) const
{
	return input_field->ray_derivative(ray, t) * derivative(input_field->value(ray.at(t)));
}

Inverse::Inverse(std::shared_ptr<Field> _field, float _radius) : 
	Remapping(_field),
	radius(_radius)
{
}

float Inverse::remap(float t) const
{
	float d = std::max(eps_div_by_zero, t);
	return radius / d;
}

float Inverse::derivative(float t) const
{
	float d = std::max(eps_div_by_zero, t * t);
	return -radius / d;
}

Exponential::Exponential(std::shared_ptr<Field> _field, float _factor) : 
	Remapping(_field),
	factor(_factor)
{
}

float Exponential::remap(float t) const
{
	return std::exp(t * factor);
}

float Exponential::derivative(float t) const
{
	return std::exp(t * factor) * factor;
}

}

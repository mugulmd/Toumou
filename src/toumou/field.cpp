#include <toumou/field.hpp>
#include <toumou/constants.hpp>

#include <spdlog/spdlog.h>

#include <algorithm>
#include <cmath>
#include <random>
#include <limits>


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

Dist2ToPoint::Dist2ToPoint(const Vec3& _center) : 
	Field(),
	center(_center)
{
}

float Dist2ToPoint::value(const Vec3& pos) const
{
	return (pos - center).length2();
}

Vec3 Dist2ToPoint::gradient(const Vec3& pos) const
{
	return (pos - center) * 2.f;
}

float Dist2ToPoint::ray_derivative(const Ray& ray, float t) const
{
	return 2.f * (t + ray.dir.dot(ray.origin - center));
}

Dist2ToLine::Dist2ToLine(const Vec3& _origin, const Vec3& _direction) : 
	Field(),
	origin(_origin), direction(_direction)
{
}

float Dist2ToLine::value(const Vec3& pos) const
{
	Vec3 delta = pos - origin;
	float lambda = delta.dot(direction);
	return delta.length2() - lambda * lambda;
}

Vec3 Dist2ToLine::gradient(const Vec3& pos) const
{
	Vec3 delta = pos - origin;
	float lambda = delta.dot(direction);
	return (delta - direction * lambda) * 2.f;
}

float Dist2ToLine::ray_derivative(const Ray& ray, float t) const
{
	Vec3 delta = ray.at(t) - origin;
	return (delta.dot(ray.dir) - ray.dir.dot(direction) * delta.dot(direction)) * 2.f;
}

SignedDistToPlane::SignedDistToPlane(const Vec3& _origin, const Vec3& _normal) :
	Field(),
	origin(_origin), normal(_normal)
{
}

float SignedDistToPlane::value(const Vec3& pos) const
{
	return normal.dot(pos - origin);
}

Vec3 SignedDistToPlane::gradient(const Vec3& pos) const
{
	return normal;
}

float SignedDistToPlane::ray_derivative(const Ray& ray, float t) const
{
	return ray.dir.dot(normal);
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

Constant::Constant(float cst) :
	Field(),
	m_cst(cst)
{
}

float Constant::value(const Vec3& pos) const
{
	return m_cst;
}

Vec3 Constant::gradient(const Vec3& pos) const
{
	return Vec3();
}

float Constant::ray_derivative(const Ray& ray, float t) const
{
	return 0.0f;
}

Smoothstep::Smoothstep(std::shared_ptr<Field> _field, float in_min, float in_max) :
	Remapping(_field),
	m_in_min(in_min), m_in_max(in_max)
{
}

float Smoothstep::remap(float t) const
{
	// Clamping
	if (t < m_in_min) {
		return 0.f;
	}
	else if (t > m_in_max) {
		return 1.f;
	}

	// Cubic Hermite interpolation
	float u = (t - m_in_min) / (m_in_max - m_in_min);
	return u * u * (3.f - 2.f * u);
}

float Smoothstep::derivative(float t) const
{
	if (t < m_in_min || t > m_in_max) {
		return 0.f;
	}

	float u = (t - m_in_min) / (m_in_max - m_in_min);
	return 6.f * u * (1.f - u);
}

CellNoise::CellNoise(float grid_size, int grid_resolution) :
	Field(),
	m_grid_size(grid_size), m_grid_resolution(grid_resolution)
{
	m_points.resize(m_grid_resolution * m_grid_resolution * m_grid_resolution);

	std::random_device rd;
	std::mt19937 gen = std::mt19937(rd());
	std::uniform_real_distribution<float> dis(0.f, 1.f);

	for (std::size_t i = 0; i < m_points.size(); ++i) {
		m_points[i].x = dis(gen);
		m_points[i].y = dis(gen);
		m_points[i].z = dis(gen);
	}
}

float CellNoise::value(const Vec3& pos) const
{
	const float voxel_size = m_grid_size / static_cast<float>(m_grid_resolution);

	Vec3 pos_in_grid = pos / voxel_size;
	const int pos_i = static_cast<int>(std::floor(pos_in_grid.x));
	const int pos_j = static_cast<int>(std::floor(pos_in_grid.y));
	const int pos_k = static_cast<int>(std::floor(pos_in_grid.z));
	
	float min_dist = std::numeric_limits<float>::max();
	for (int di = -1; di <= 1; ++di) {
		const int corner_i = pos_i + di;
		const int i = ((corner_i % m_grid_resolution) + m_grid_resolution) % m_grid_resolution;

		for (int dj = -1; dj <= 1; ++dj) {
			const int corner_j = pos_j + dj;
			const int j = ((corner_j % m_grid_resolution) + m_grid_resolution) % m_grid_resolution;

			for (int dk = -1; dk <= 1; ++dk) {
				const int corner_k = pos_k + dk;
				const int k = ((corner_k % m_grid_resolution) + m_grid_resolution) % m_grid_resolution;

				Vec3 corner(static_cast<float>(corner_i), static_cast<float>(corner_j), static_cast<float>(corner_k));

				const int idx = i + (j + k * m_grid_resolution) * m_grid_resolution;
				Vec3 point = (corner + m_points[idx]) * voxel_size;

				const float dist = (pos - point).length();
				min_dist = std::min(min_dist, dist);
			}
		}
	}

	min_dist /= voxel_size * std::sqrt(3.f);

	return min_dist;
}

}

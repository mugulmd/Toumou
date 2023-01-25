#pragma once

#include <toumou/geometry.hpp>

#include <memory>
#include <unordered_map>
#include <vector>


namespace toumou {

/**
 * @brief TODO
 */
class Field {
public:

	/**
	 * @brief TODO
	 */
	Field();

	/**
	 * @brief TODO
	 */
	virtual float value(const Vec3& pos) const = 0;

	/**
	 * @brief TODO
	 */
	virtual Vec3 gradient(const Vec3& pos) const;

	/**
	 * @brief TODO
	 */
	virtual float ray_derivative(const Ray& ray, float t) const;

};

/**
 * @brief TODO
 */
class Constant : public Field {
public:

	/**
	 * @brief TODO
	 */
	Constant(float cst);

	/**
	 * @brief TODO
	 */
	float value(const Vec3& pos) const override;

	/**
	 * @brief TODO
	 */
	Vec3 gradient(const Vec3& pos) const override;

	/**
	 * @brief TODO
	 */
	float ray_derivative(const Ray& ray, float t) const override;

private:

	/// TODO
	float m_cst;

};

/**
 * @brief TODO
 */
class Fusion : public Field {
public:

	/**
	 * @brief TODO
	 */
	Fusion();

	/**
	 * @brief TODO
	 */
	void add(std::shared_ptr<Field> field, float coef = 1.f);

	float value(const Vec3& pos) const override;

	Vec3 gradient(const Vec3& pos) const override;

	float ray_derivative(const Ray& ray, float t) const override;

private:

	/// TODO
	std::unordered_map<std::shared_ptr<Field>, float> m_fields;

};

/**
 * @brief TODO
 */
class Dist2ToPoint : public Field {
public:

	/**
	 * @brief TODO
	 */
	Dist2ToPoint(const Vec3& _center);

	/// TODO
	Vec3 center;

	float value(const Vec3& pos) const override;

	Vec3 gradient(const Vec3& pos) const override;

	float ray_derivative(const Ray& ray, float t) const override;

};

/**
 * @brief TODO
 */
class Dist2ToLine : public Field {
public:

	/**
	 * @brief TODO
	 */
	Dist2ToLine(const Vec3& _origin, const Vec3& _direction);

	/// TODO
	Vec3 origin;

	/// TODO
	Vec3 direction;

	float value(const Vec3& pos) const override;

	Vec3 gradient(const Vec3& pos) const override;

	float ray_derivative(const Ray& ray, float t) const override;

};

/**
 * @brief TODO
 */
class SignedDistToPlane : public Field {
public:

	/**
	 * @brief TODO
	 */
	SignedDistToPlane(const Vec3& _origin, const Vec3& _normal);

	/// TODO
	Vec3 origin;

	/// TODO
	Vec3 normal;

	float value(const Vec3& pos) const override;

	Vec3 gradient(const Vec3& pos) const override;

	float ray_derivative(const Ray& ray, float t) const override;

};

/**
 * @brief TODO
 */
class Remapping : public Field {
public:

	/**
	 * @brief TODO
	 */
	Remapping(std::shared_ptr<Field> _field);

	/// TODO
	std::shared_ptr<Field> input_field;

	/**
	 * @brief TODO
	 */
	virtual float remap(float t) const = 0;

	/**
	 * @brief TODO
	 */
	virtual float derivative(float t) const;

	float value(const Vec3& pos) const override;

	Vec3 gradient(const Vec3& pos) const override;

	float ray_derivative(const Ray& ray, float t) const override;

};

/**
 * @brief TODO
 */
class Inverse : public Remapping {
public:

	/**
	 * @brief TODO
	 */
	Inverse(std::shared_ptr<Field> _field, float _radius);

	/// TODO
	float radius;

	float remap(float t) const override;

	float derivative(float t) const;

};

/**
 * @brief TODO
 */
class Exponential : public Remapping {
public:

	/**
	 * @brief TODO
	 */
	Exponential(std::shared_ptr<Field> _field, float _factor);

	/// TODO
	float factor;

	float remap(float t) const override;

	float derivative(float t) const;

};

/**
 * @brief TODO
 */
class Smoothstep : public Remapping {
public:

	/**
	 * @brief TODO
	 */
	Smoothstep(std::shared_ptr<Field> _field, float in_min, float in_max);

	float remap(float t) const override;

	float derivative(float t) const;

private:

	/// TODO
	float m_in_min;

	/// TODO
	float m_in_max;

};

/**
 * @brief TODO
 */
class CellNoise : public Field {
public:

	/**
	 * @brief TODO
	 */
	CellNoise(float grid_size, int grid_resolution);

	float value(const Vec3& pos) const override;

private:

	/// TODO
	float m_grid_size;

	/// TODO
	int m_grid_resolution;

	/// TODO
	std::vector<Vec3> m_points;

};

}

#pragma once

#include <lava/geometry.hpp>

#include <memory>
#include <unordered_map>


namespace lava {

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
class Fusion : public Field {
public:

	/**
	 * @brief TODO
	 */
	Fusion();

	/**
	 * @brief TODO
	 */
	void add(std::shared_ptr<Field> field, float coef);

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
class DistToPoint : public Field {
public:

	/**
	 * @brief TODO
	 */
	DistToPoint(const Vec3& _center);

	/// TODO
	Vec3 center;

	float value(const Vec3& pos) const override;

	Vec3 gradient(const Vec3& pos) const override;

	float ray_derivative(const Ray& ray, float t) const override;

};

/**
 * @brief TODO
 */
class DistToLine : public Field {
public:

	/**
	 * @brief TODO
	 */
	DistToLine(const Vec3& _origin, const Vec3& _direction);

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

}

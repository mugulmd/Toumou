#pragma once

#include <lava/geometry.hpp>
#include <lava/material.hpp>
#include <lava/root_estimation.hpp>
#include <lava/field.hpp>

#include <memory>


namespace lava {

/**
 * @brief Abstract class for surface models.
 */
class Surface {

	/// Counter used to assign a unique ID to each instance.
	static unsigned int uid_counter;

protected:

	/// Unique identifier.
	unsigned int m_uid;

public:

	/// Surface material.
	Material material;

	/// Default constructor.
	Surface();

	/// Access the surface's unique identifier.
	unsigned int uid() const;

	/**
	 * @brief Check if the given ray intersects this surface.
	 * @param[in] ray Ray to check for intersection.
	 * @param[out] t Distance between the ray's origin and the hit (if a hit has been found).
	 * @param[out] n Surface normal at the hit (if a hit has been found).
	 * @return Whether or not an intersection point was found.
	 */
	virtual bool hit(const Ray& ray, float& t, Vec3& n) const = 0;

};

/**
 * @brief Define an implicit surface using a field function.
 */
class ImplicitSurface : public Surface {
public:

	/**
	 * @brief TODO
	 */
	ImplicitSurface(std::shared_ptr<Field> _field);

	/// TODO
	std::shared_ptr<Field> field;

	/// Provide access to the root estimation algorithm parameters.
	RootEstimator root_estimator;

	virtual bool hit(const Ray& ray, float& t, Vec3& n) const override;

};

/**
 * @brief Sphere surface model.
 */
class Sphere : public ImplicitSurface {
public:

	/**
	 * @brief TODO
	 */
	Sphere(const Vec3& _center, float _radius);

	/// Sphere center.
	Vec3 center;

	/// Sphere radius.
	float radius;

	bool hit(const Ray& ray, float& t, Vec3& n) const override;

};

/**
 * @brief Plane surface model.
 */
class Plane : public ImplicitSurface {
public:

	/**
	 * @brief TODO
	 */
	Plane(const Vec3& _origin, const Vec3& _normal);

	/// Plane origin.
	Vec3 origin;

	/// Plane normal (normalized).
	Vec3 normal;

	bool hit(const Ray& ray, float& t, Vec3& n) const override;

};

/**
 * @brief Tube surface model.
 */
class Tube : public ImplicitSurface {
public:

	/**
	 * @brief TODO
	 */
	Tube(const Vec3& _origin, const Vec3& _direction, float _radius);

	/// Tube origin.
	Vec3 origin;

	/// Tube direction (normalized).
	Vec3 direction;

	/// Tube radius.
	float radius;

	bool hit(const Ray& ray, float& t, Vec3& n) const override;

};

}

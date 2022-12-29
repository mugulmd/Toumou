#pragma once

#include <lava/geometry.hpp>
#include <lava/material.hpp>

#include <memory>


namespace lava {

/**
 * @brief An abstract class to define and manipulate surface models.
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
	 * @param[in] ray The ray to check for intersection.
	 * @param[out] t The distance between the ray's origin and the hit (if a hit has been found).
	 * @param[out] n The surface normal at the hit (if a hit has been found).
	 * @return Whether or not an intersection point was found.
	 */
	virtual bool hit(const Ray& ray, float& t, Vec3& n) const = 0;

};

/**
 * @brief A class to define and manipulate a sphere surface model.
 */
class Sphere : public Surface {
public:

	/// Sphere center.
	Vec3 center;

	/// Sphere radius.
	float radius;

	bool hit(const Ray& ray, float& t, Vec3& n) const override;

};

/**
 * @brief Constructs and initializes a sphere surface.
 * @param[in] center The sphere center.
 * @param[in] radius The sphere radius.
 * @return A shared pointer to a sphere surface model initialized with the given parameters.
 */
std::shared_ptr<Sphere> make_sphere(const Vec3& center, float radius);

/**
 * @brief A class to define and manipulate a plane surface model.
 */
class Plane : public Surface {
public:

	/// Plane origin.
	Vec3 origin;

	/// Plane normal (normalized).
	Vec3 normal;

	bool hit(const Ray& ray, float& t, Vec3& n) const override;

};

/**
 * @brief Constructs and initializes a plane surface.
 * @param[in] origin The plane origin.
 * @param[in] normal The plane normal (must be normalized).
 * @return A shared pointer to a plane surface model initialized with the given parameters.
 */
std::shared_ptr<Plane> make_plane(const Vec3& origin, const Vec3& normal);

}

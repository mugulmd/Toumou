#pragma once

#include <lava/geometry.hpp>
#include <lava/material.hpp>
#include <lava/root_finding.hpp>

#include <memory>
#include <vector>


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

/**
 * @brief A class to define and manipulate a tube surface model.
 */
class Tube : public Surface {
public:

	/// Tube origin.
	Vec3 origin;

	/// Tube direction (normalized).
	Vec3 direction;

	/// Tube radius.
	float radius;

	bool hit(const Ray& ray, float& t, Vec3& n) const override;

};

/**
 * @brief Constructs and initializes a tube surface.
 * @param[in] origin The tube origin.
 * @param[in] direction The tube direction.
 * @param[in] radius The tube radius.
 * @return A shared pointer to a tube surface model initialized with the given parameters.
 */
std::shared_ptr<Tube> make_tube(const Vec3& origin, const Vec3& direction, float radius);

/**
 * @brief A class to create a metaball from several spheres.
 * @see Sphere
 */
class Metaball : public Surface {
public:

	/**
	 * @brief Add a sphere to this metaball.
	 * @param[in] sphere A shared pointer to a sphere.
	 */
	void add_sphere(std::shared_ptr<Sphere> sphere);

	/// Provide access to the root finding algorithm parameters.
	RootFinder root_finder;

	bool hit(const Ray& ray, float& t, Vec3& n) const override;

private:

	/// Metaball spheres.
	std::vector<std::shared_ptr<Sphere>> m_spheres;

	/**
	 * @brief Sample the metaball field.
	 * @param[in] pos The sampling position.
	 * @return The field value at the given position.
	 */
	float value_at(const Vec3& pos) const;

	/**
	 * @brief Compute the derivative of the metaball field restricted to a ray.
	 * @param[in] ray The ray on which to compute the derivative.
	 * @param[in] t A position along the ray.
	 * @return The metaball field derivative at the given position along the ray.
	 */
	float ray_derivative_at(const Ray& ray, float t) const;

	/**
	 * @brief Compute the metaball's normal vector.
	 * @param[in] pos The position where to compute the normal.
	 * @return The normal at the given position, i.e the normalized opposite of the gradient.
	 */
	Vec3 normal_at(const Vec3& pos) const;

};

/**
 * @brief Constructs an empty metaball.
 * @return A shared pointer to an empty metaball.
 */
std::shared_ptr<Metaball> make_metaball();

}

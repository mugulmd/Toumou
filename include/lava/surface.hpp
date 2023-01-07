#pragma once

#include <lava/geometry.hpp>
#include <lava/material.hpp>
#include <lava/root_finding.hpp>

#include <memory>
#include <unordered_map>


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
 * @brief An abstract class to define an implicit surface given its corresponding field function.
 */
class ImplicitSurface : public Surface {
public:

	/// Provide access to the root estimation algorithm parameters.
	RootFinder root_finder;

	virtual bool hit(const Ray& ray, float& t, Vec3& n) const override;

	/**
	 * @brief Field function that defines the surface as the solution to { field = 1 }.
	 * @param[in] pos Position where the field is evaluated.
	 * @return The field value at that position.
	 */
	virtual float field(const Vec3& pos) const = 0;

	/**
	 * @brief The field derivative along a ray.
	 * @param[in] ray Input ray.
	 * @param[in] t The parameters giving the position along the ray for evaluating the derivative.
	 * @return The derivative along the ray at the given position.
	 */
	virtual float ray_derivative(const Ray& ray, float t) const;

	/**
	 * @brief The surface normal, i.e. the normalized opposite of the field gradient.
	 * @param[in] pos Position where the normal is evaluated.
	 * @return The surface normal at that position.
	 */
	virtual Vec3 normal(const Vec3& pos) const;

};

/**
 * @brief A class to define and manipulate a sphere surface model.
 */
class Sphere : public ImplicitSurface {
public:

	/// Sphere center.
	Vec3 center;

	/// Sphere radius.
	float radius;

	bool hit(const Ray& ray, float& t, Vec3& n) const override;

	float field(const Vec3& pos) const override;

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
class Plane : public ImplicitSurface {
public:

	/// Plane origin.
	Vec3 origin;

	/// Plane normal (normalized).
	Vec3 normal;

	bool hit(const Ray& ray, float& t, Vec3& n) const override;

	float field(const Vec3& pos) const override;

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
class Tube : public ImplicitSurface {
public:

	/// Tube origin.
	Vec3 origin;

	/// Tube direction (normalized).
	Vec3 direction;

	/// Tube radius.
	float radius;

	bool hit(const Ray& ray, float& t, Vec3& n) const override;

	float field(const Vec3& pos) const override;

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
 * @brief A class to merge several implicit surfaces together.
 */
class Fusion : public ImplicitSurface {
public:

	/**
	 * @brief Add an implicit surface.
	 * @param[in] surface A shared pointer to an implicit surface.
	 * @param[in] coef The surface ponderation in the fusion.
	 */
	void add(std::shared_ptr<ImplicitSurface> surface, float coef);

	float field(const Vec3& pos) const override;

private:

	/// Surfaces in the fusion with their ponderation.
	std::unordered_map<std::shared_ptr<ImplicitSurface>, float> m_surfaces;

};

/**
 * @brief Constructs an empty fusion surface.
 * @return A shared pointer to an empty fusion surface.
 */
std::shared_ptr<Fusion> make_fusion();

}

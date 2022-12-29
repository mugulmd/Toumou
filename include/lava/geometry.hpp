#pragma once


namespace lava {

/**
 * @brief A struct to define and manipulate 3D vectors with float precision.
 */
struct Vec3 {

	/// (X, Y, Z) coordinates.
	float x, y, z;

	/// Default constructor, initialize all coordinates to 0.
	Vec3();
	
	/// Utility constructor with coordinates initialization.
	Vec3(float _x, float _y, float _z);

	/// Utility operators for calculus.
	bool operator==(const Vec3& other) const;
	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	Vec3 operator*(const float f) const;
	Vec3 operator/(const float f) const;
	Vec3& operator=(const Vec3& other);
	Vec3& operator+=(const Vec3& other);
	Vec3& operator-=(const Vec3& other);
	Vec3& operator*=(const float f);
	Vec3& operator/=(const float f);

	/// Compute the squared L2-norm of this vector.
	float norm2() const;

	/// Compute the L2-norm of this vector.
	float norm() const;

	/// In-place vector normalization.
	void normalize();

	/// Compute a normalized version of this vector.
	Vec3 normalized() const;

};

/**
 * @brief Compute the Euclidean distance between two vectors.
 * @param[in] lhs Left-hand-side vector.
 * @param[in] rhs Right-hand-side vector.
 * @return The L2-norm of difference between lhs and rhs.
 */
float distance(const Vec3& lhs, const Vec3& rhs);

/**
 * @brief Compute the dot product between two vectors.
 * @param[in] lhs Left-hand-side vector.
 * @param[in] rhs Right-hand-side vector.
 * @return The dot product between lhs and rhs.
 */
float dot(const Vec3& lhs, const Vec3& rhs);

/**
 * @brief Compute the cross product between two vectors.
 * @param[in] lhs Left-hand-side vector.
 * @param[in] rhs Right-hand-side vector.
 * @return The cross product between lhs and rhs.
 */
Vec3 cross(const Vec3& lhs, const Vec3& rhs);

/**
 * @brief A struct to define and manipulate a ray, i.e a half-line in 3D space.
 * @see trace
 */
struct Ray {

	/// Starting point of the ray.
	Vec3 origin;

	/// Direction of the ray (normalized).
	Vec3 dir;

	/**
	 * @brief Constructor with parameter initilization.
	 * @param[in] _origin Starting point of the ray.
	 * @param[in] _dir Direction of the ray (must be normalized).
	 */
	Ray(const Vec3& _origin, const Vec3& _dir);

	/**
	 * @brief Retrieve the position of a point on this ray.
	 * @param[in] t The distance between between the ray's origin and the position to compute (must be positive).
	 * @return The position of the queried point along the ray.
	 */
	Vec3 at(float t) const;

};

/**
 * @brief Trace a ray going through two points in 3D space.
 * @param[in] from The starting point of the ray to trace.
 * @param[in] to The destination point which the ray must go through.
 * @return A ray starting at the first given point and passing through the second one.
 */
Ray trace(const Vec3& from, const Vec3& to);

}

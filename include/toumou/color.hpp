#pragma once


namespace toumou {

/**
 * @brief RGB colors with float precision.
 */
struct Color {

	/// Red-Green-Blue channels.
	float r, g, b;

	/// Default constructor, initialize all channels to 0.
	Color();

	/// Utility constructor with channels initialization.
	Color(float _r, float _g, float _b);

	/// Utility operators for calculus.
	bool operator==(const Color& other) const;
	Color operator+(const Color& other) const;
	Color operator-(const Color& other) const;
	Color operator*(const float f) const;
	Color operator/(const float f) const;
	Color& operator=(const Color& other);
	Color& operator+=(const Color& other);
	Color& operator-=(const Color& other);
	Color& operator*=(const float f);
	Color& operator/=(const float f);

};

}

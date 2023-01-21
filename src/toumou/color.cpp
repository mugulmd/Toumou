#include <toumou/color.hpp>


namespace toumou {

Color::Color() : r(0), g(0), b(0) 
{}

Color::Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b)
{}

bool Color::operator==(const Color& other) const
{
	return r == other.r && g == other.g && b == other.b;
}

Color Color::operator+(const Color& other) const
{
	return Color(r + other.r, g + other.g, b + other.b);
}

Color Color::operator-(const Color& other) const
{
	return Color(r - other.r, g - other.g, b - other.b);
}

Color Color::operator*(const float f) const
{
	return Color(r * f, g * f, b  * f);
}

Color Color::operator/(const float f) const
{
	return Color(r / f, g / f, b  / f);
}

Color& Color::operator=(const Color& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	return *this;
}

Color& Color::operator+=(const Color& other)
{
	r += other.r;
	g += other.g;
	b += other.b;
	return *this;
}

Color& Color::operator-=(const Color& other)
{
	r -= other.r;
	g -= other.g;
	b -= other.b;
	return *this;
}

Color& Color::operator*=(const float f)
{
	r *= f;
	g *= f;
	b *= f;
	return *this;
}

Color& Color::operator/=(const float f)
{
	r /= f;
	g /= f;
	b /= f;
	return *this;
}

}

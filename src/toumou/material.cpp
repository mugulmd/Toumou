#include <toumou/material.hpp>


namespace toumou {

Color Material::color_at(const Vec3& pos) const
{
	return Color(
		red->value(pos),
		green->value(pos),
		blue->value(pos)
	);
}

void Material::set_solid_color(const Color& color)
{
	red = tmks(Constant, color.x);
	green = tmks(Constant, color.y);
	blue = tmks(Constant, color.z);
}

}

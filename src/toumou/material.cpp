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

}

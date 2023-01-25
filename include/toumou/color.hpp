#pragma once

#include <Imath/ImathColor.h>


namespace toumou {

/**
 * @brief RGB colors with float precision.
 */
using Color = Imath::Color3f;

inline const Color BLACK = Color(0, 0, 0);
inline const Color RED = Color(1, 0, 0);
inline const Color GREEN = Color(0, 1, 0);
inline const Color BLUE = Color(0, 0, 1);
inline const Color YELLOW = Color(1, 1, 0);
inline const Color MAGENTA = Color(1, 0, 1);
inline const Color CYAN = Color(0, 1, 1);
inline const Color WHITE = Color(1, 1, 1);

}

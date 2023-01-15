#pragma once


namespace lava {

/// Minimal separation between ray origin and hit point.
inline const float eps_ray_sep = 1e-5f;

/// Minimal value for divisions to avoid division-by-zero errors.
inline const float eps_div_by_zero = 1e-10f;

}

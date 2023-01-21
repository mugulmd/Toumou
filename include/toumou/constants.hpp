#pragma once


namespace toumou {

/// Minimal separation between ray origin and hit point.
inline const float eps_ray_sep = 1e-5f;

/// Minimal value for divisions to avoid division-by-zero errors.
inline const float eps_div_by_zero = 1e-10f;

/// Step for discretized derivation.
inline const float derivation_step = 1e-5f;

}

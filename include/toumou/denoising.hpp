#pragma once

#include <toumou/rendering.hpp>
#include <toumou/image.hpp>
#include <toumou/color.hpp>


namespace toumou {

/**
 * @brief TODO
 */
class Denoiser {
public:

	/// TODO
	virtual Image<Color> denoise(const RayTracer& rt) const = 0;

};

/**
 * @brief Denoising using a Vector Median Filter (VMF).
 */
class VMFDenoiser : public Denoiser {
public:

	Image<Color> denoise(const RayTracer& rt) const override;

	/// TODO
	int window_half_size = 2;

};

}

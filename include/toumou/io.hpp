#pragma once

#include <toumou/rendering.hpp>
#include <toumou/image.hpp>
#include <toumou/color.hpp>

#include <string>


namespace toumou {

/**
 * @brief Write render passes on disk in EXR format.
 * @param[in] rt Ray tracer with computed render passes.
 * @param[in] path Filepath to save the render passes on disk (must have the .exr extension).
 */
void write_EXR(const RayTracer& rt, const std::string& path);

/**
 * @brief TODO
 */
Image<Color> read_EXR(const std::string& path);

}

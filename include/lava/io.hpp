#pragma once

#include <lava/rendering.hpp>

#include <string>


namespace lava {

/**
 * @brief Write render passes on disk in EXR format.
 * @param[in] rt Ray tracer with computed render passes.
 * @param[in] path Filepath to save the render passes on disk (must have the .exr extension).
 */
void write_EXR(const RayTracer& rt, const std::string& path);

}

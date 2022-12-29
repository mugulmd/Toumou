#pragma once

#include <lava/rendering.hpp>

#include <string>


namespace lava {

/**
 * @brief Write the given render layers on disk using the OpenEXR format.
 * @see RenderLayers
 * @param[in] layers The render layers to save.
 * @param[in] path The path indicating where to save the layers on disk (must have the .exr extension).
 */
void write_EXR(const RenderLayers& layers, const std::string& path);

}

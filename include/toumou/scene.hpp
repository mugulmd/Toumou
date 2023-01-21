#pragma once

#include <toumou/camera.hpp>
#include <toumou/light.hpp>
#include <toumou/surface.hpp>

#include <vector>


namespace toumou {

/**
 * @brief Reference and access the objects in a 3D scene.
 */
class Scene {
public:

	void set_camera(std::shared_ptr<Camera> cam);

	void add_light(std::shared_ptr<Light> light);

	void add_surface(std::shared_ptr<Surface> surface);

	std::shared_ptr<Camera> camera() const;

	const std::vector<std::shared_ptr<Light>>& lights() const;

	const std::vector<std::shared_ptr<Surface>>& surfaces() const;

private:

	std::shared_ptr<Camera> m_camera = nullptr;

	std::vector<std::shared_ptr<Light>> m_lights;

	std::vector<std::shared_ptr<Surface>> m_surfaces;

};

}

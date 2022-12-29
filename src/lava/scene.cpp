#include <lava/scene.hpp>


namespace lava {

void Scene::set_camera(std::shared_ptr<Camera> cam)
{
	m_camera = cam;
}

void Scene::add_light(std::shared_ptr<Light> light)
{
	m_lights.push_back(light);
}

void Scene::add_surface(std::shared_ptr<Surface> surface)
{
	m_surfaces.push_back(surface);
}

std::shared_ptr<Camera> Scene::camera() const
{
	return m_camera;
}

const std::vector<std::shared_ptr<Light>>& Scene::lights() const
{
	return m_lights;
}

const std::vector<std::shared_ptr<Surface>>& Scene::surfaces() const
{
	return m_surfaces;
}

}

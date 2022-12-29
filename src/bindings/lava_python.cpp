#include <lava/all.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include <memory>

namespace py = pybind11;

using namespace lava;


PYBIND11_MODULE(lava_python, m) 
{
	m.doc() = "Python bindings for Lava";

	// Camera

	py::class_<Camera, std::shared_ptr<Camera>>(m, "Camera")
		.def_readonly("location", &Camera::location)
		.def_readonly("forward", &Camera::forward)
		.def_readonly("up", &Camera::up)
		.def_readonly("right", &Camera::right)
		.def_readonly("sensor_width", &Camera::sensor_width)
		.def_readonly("field_of_view", &Camera::field_of_view)
		.def_readonly("z_near", &Camera::z_near)
		.def_readonly("z_far", &Camera::z_far);

	m.def("make_camera", &make_camera, 
		py::arg("location"), 
		py::arg("forward"), 
		py::arg("up"), 
		py::arg("sensor_width"), 
		py::arg("field_of_view"), 
		py::arg("z_near"), 
		py::arg("z_far"));

	// Color

	py::class_<Color>(m, "Color")
		.def(py::init<float, float, float>())
		.def_readwrite("r", &Color::r)
		.def_readwrite("g", &Color::g)
		.def_readwrite("b", &Color::b);

	// Geometry

	py::class_<Vec3>(m, "Vec3")
		.def(py::init<float, float, float>())
		.def_readwrite("x", &Vec3::x)
		.def_readwrite("y", &Vec3::y)
		.def_readwrite("z", &Vec3::z)
		.def("norm2", &Vec3::norm2)
		.def("norm", &Vec3::norm)
		.def("normalize", &Vec3::normalize)
		.def("normalized", &Vec3::normalized);

	m.def("distance", &distance);

	m.def("dot", &dot);

	m.def("cross", &cross);

	py::class_<Ray>(m, "Ray")
		.def_readonly("origin", &Ray::origin)
		.def_readonly("dir", &Ray::dir)
		.def("at", &Ray::at);

	m.def("trace", &trace);

	// Image

	// IO

	m.def("write_EXR", &write_EXR, 
		py::arg("layers"), 
		py::arg("path"));

	// Light

	py::class_<Light, std::shared_ptr<Light>>(m, "Light")
		.def_readwrite("brightness", &Light::brightness)
		.def_readwrite("color", &Light::color);

	py::class_<PointLight, std::shared_ptr<PointLight>, Light>(m, "PointLight")
		.def_readwrite("location", &PointLight::location);

	m.def("make_point_light", &make_point_light, 
		py::arg("location"), 
		py::arg("brightness"), 
		py::arg("color"));

	py::class_<SunLight, std::shared_ptr<SunLight>, Light>(m, "SunLight")
		.def_readwrite("direction", &SunLight::direction);

	m.def("make_sun_light", &make_sun_light, 
		py::arg("direction"), 
		py::arg("brightness"), 
		py::arg("color"));

	// Material

	py::class_<Material>(m, "Material")
		.def_readwrite("base_color", &Material::base_color)
		.def_readwrite("k_ambient", &Material::k_ambient)
		.def_readwrite("k_diffuse", &Material::k_diffuse)
		.def_readwrite("k_specular", &Material::k_specular)
		.def_readwrite("roughness", &Material::roughness);

	// Rendering

	py::class_<RenderParams>(m, "RenderParams")
		.def(py::init<int>());

	py::class_<RenderLayers>(m, "RenderLayers")
		.def(py::init<int, int>());

	m.def("render", &render, 
		py::arg("scene"), 
		py::arg("params"), 
		py::arg("layers"), 
		py::arg("progress_callback"));

	// Scene

	py::class_<Scene>(m, "Scene")
		.def(py::init<>())
		.def("set_camera", &Scene::set_camera)
		.def("add_light", &Scene::add_light)
		.def("add_surface", &Scene::add_surface)
		.def("camera", &Scene::camera)
		.def("lights", &Scene::lights)
		.def("surfaces", &Scene::surfaces);

	// Surface

	py::class_<Surface, std::shared_ptr<Surface>>(m, "Surface")
		.def_readwrite("material", &Surface::material);

	py::class_<Sphere, std::shared_ptr<Sphere>, Surface>(m, "Sphere")
		.def_readwrite("center", &Sphere::center)
		.def_readwrite("radius", &Sphere::radius);

	m.def("make_sphere", &make_sphere, 
		py::arg("center"), 
		py::arg("radius"));

	py::class_<Plane, std::shared_ptr<Plane>, Surface>(m, "Plane")
		.def_readwrite("origin", &Plane::origin)
		.def_readwrite("normal", &Plane::normal);

	m.def("make_plane", &make_plane,
		py::arg("origin"),
		py::arg("normal"));
}

#include <lava/all.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <memory>

namespace py = pybind11;

using namespace lava;


PYBIND11_MODULE(lava_python, m) 
{
	m.doc() = "Python bindings for Lava";

	// Color

	py::class_<Color>(m, "Color")
		.def(py::init<>())
		.def(py::init<float, float, float>())
		.def_readwrite("r", &Color::r)
		.def_readwrite("g", &Color::g)
		.def_readwrite("b", &Color::b);

	// Geometry

	py::class_<Vec3>(m, "Vec3")
		.def(py::init<>())
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
		py::arg("location") = Vec3(0, 0, 0),
		py::arg("forward") = Vec3(0, 0, -1),
		py::arg("up") = Vec3(0, 1, 0),
		py::arg("sensor_width") = 36.f,
		py::arg("field_of_view") = 90.f,
		py::arg("z_near") = .1f,
		py::arg("z_far") = 100.f);

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
		py::arg("brightness") = 1.f, 
		py::arg("color") = Color(1, 1, 1));

	py::class_<DirectionalLight, std::shared_ptr<DirectionalLight>, Light>(m, "DirectionalLight")
		.def_readwrite("direction", &DirectionalLight::direction);

	m.def("make_directional_light", &make_directional_light, 
		py::arg("direction"), 
		py::arg("brightness") = 1.f, 
		py::arg("color") = Color(1, 1, 1));

	// Material

	py::class_<Material>(m, "Material")
		.def_readwrite("base_color", &Material::base_color)
		.def_readwrite("albedo", &Material::albedo);

	// Rendering

	py::class_<RayTracer>(m, "RayTracer")
		.def(py::init<int, int, int, int, int>())
		.def("render", &RayTracer::render);

	// Root finding

	py::class_<RootFinder>(m, "RootFinder")
		.def_readwrite("t_min", &RootFinder::t_min)
		.def_readwrite("t_max", &RootFinder::t_max)
		.def_readwrite("sampling_step", &RootFinder::sampling_step)
		.def_readwrite("threshold", &RootFinder::threshold)
		.def_readwrite("max_iterations", &RootFinder::max_iterations);

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

	py::class_<ImplicitSurface, std::shared_ptr<ImplicitSurface>, Surface>(m, "ImplicitSurface")
		.def_readwrite("root_finder", &ImplicitSurface::root_finder);

	py::class_<Sphere, std::shared_ptr<Sphere>, ImplicitSurface>(m, "Sphere")
		.def_readwrite("center", &Sphere::center)
		.def_readwrite("radius", &Sphere::radius);

	m.def("make_sphere", &make_sphere, 
		py::arg("center") = Vec3(0, 0, 0),
		py::arg("radius") = 1.f);

	py::class_<Plane, std::shared_ptr<Plane>, ImplicitSurface>(m, "Plane")
		.def_readwrite("origin", &Plane::origin)
		.def_readwrite("normal", &Plane::normal);

	m.def("make_plane", &make_plane,
		py::arg("origin") = Vec3(0, 0, 0),
		py::arg("normal") = Vec3(0, 1, 0));

	py::class_<Tube, std::shared_ptr<Tube>, ImplicitSurface>(m, "Tube")
		.def_readwrite("origin", &Tube::origin)
		.def_readwrite("direction", &Tube::direction)
		.def_readwrite("radius", &Tube::radius);

	m.def("make_tube", &make_tube, 
		py::arg("origin") = Vec3(0, 0, 0), 
		py::arg("direction") = Vec3(0, 1, 0), 
		py::arg("radius") = .5f);

	py::class_<Fusion, std::shared_ptr<Fusion>, ImplicitSurface>(m, "Fusion")
		.def("add", &Fusion::add);

	m.def("make_fusion", &make_fusion);
}

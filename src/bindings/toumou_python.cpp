#include <toumou/all.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <memory>

namespace py = pybind11;

using namespace toumou;


template<typename Class, typename... Args>
std::shared_ptr<Class> TMKS(Args... args)
{
	return tmks(Class, args...);
}

#define PYTMKS(Class, ...) py::init(&TMKS<Class, __VA_ARGS__>)


PYBIND11_MODULE(toumou, m) 
{
	m.doc() = "Python bindings for Toumou";

	// Geometry

	py::class_<Vec3>(m, "Vec3")
		.def(py::init<float, float, float>())
		.def_readwrite("x", &Vec3::x)
		.def_readwrite("y", &Vec3::y)
		.def_readwrite("z", &Vec3::z)
		.def("norm2", &Vec3::length2)
		.def("norm", &Vec3::length)
		.def("normalize", &Vec3::normalize)
		.def("normalized", &Vec3::normalized)
		.def("dot", &Vec3::dot)
		.def("cross", &Vec3::cross);

	py::class_<Ray>(m, "Ray")
		.def_readonly("origin", &Ray::origin)
		.def_readonly("dir", &Ray::dir)
		.def("at", &Ray::at);

	m.def("trace", &trace);

	// Color

	py::class_<Color>(m, "Color")
		.def(py::init<float, float, float>())
		.def_readwrite("r", &Color::x)
		.def_readwrite("g", &Color::y)
		.def_readwrite("b", &Color::z)
		.def_readonly_static("BLACK", &BLACK)
		.def_readonly_static("RED", &RED)
		.def_readonly_static("GREEN", &GREEN)
		.def_readonly_static("BLUE", &BLUE)
		.def_readonly_static("YELLOW", &YELLOW)
		.def_readonly_static("MAGENTA", &MAGENTA)
		.def_readonly_static("CYAN", &CYAN)
		.def_readonly_static("WHITE", &WHITE);

	// Camera

	py::class_<Camera, std::shared_ptr<Camera>>(m, "Camera")
		.def(PYTMKS(Camera, float, float, float, float))
		.def("move_to", &Camera::move_to)
		.def("rotate_to", &Camera::rotate_to);

	// Image

	py::class_<Image<Color>>(m, "Image")
		.def(py::init<int, int>())
		.def("at", &Image<Color>::at)
		.def("set", &Image<Color>::set);

	// Light

	py::class_<Light, std::shared_ptr<Light>>(m, "Light")
		.def_readwrite("brightness", &Light::brightness)
		.def_readwrite("color", &Light::color);

	py::class_<PointLight, std::shared_ptr<PointLight>, Light>(m, "PointLight")
		.def(PYTMKS(PointLight, const Color&, float, const Vec3&));

	py::class_<DirectionalLight, std::shared_ptr<DirectionalLight>, Light>(m, "DirectionalLight")
		.def(PYTMKS(DirectionalLight, const Color&, float, const Vec3&));

	py::class_<EnvironmentLight, std::shared_ptr<EnvironmentLight>>(m, "EnvironmentLight")
		.def(PYTMKS(EnvironmentLight, const Color&, float));

	py::class_<PanoramaLight, std::shared_ptr<PanoramaLight>, EnvironmentLight>(m, "PanoramaLight")
		.def(PYTMKS(PanoramaLight, const Image<Color>&, float));

	// Field

	py::class_<Field, std::shared_ptr<Field>>(m, "Field")
		.def("value", &Field::value);

	py::class_<Constant, std::shared_ptr<Constant>, Field>(m, "Constant")
		.def(PYTMKS(Constant, float));

	py::class_<Fusion, std::shared_ptr<Fusion>, Field>(m, "Fusion")
		.def(PYTMKS(Fusion))
		.def("add", &Fusion::add);

	py::class_<Dist2ToPoint, std::shared_ptr<Dist2ToPoint>, Field>(m, "Dist2ToPoint")
		.def(PYTMKS(Dist2ToPoint, const Vec3&));

	py::class_<Dist2ToLine, std::shared_ptr<Dist2ToLine>, Field>(m, "Dist2ToLine")
		.def(PYTMKS(Dist2ToLine, const Vec3&, const Vec3&));

	py::class_<SignedDistToPlane, std::shared_ptr<SignedDistToPlane>, Field>(m, "SignedDistToPlane")
		.def(PYTMKS(SignedDistToPlane, const Vec3&, const Vec3&));

	py::class_<Remapping, std::shared_ptr<Remapping>, Field>(m, "Remapping")
		.def("remap", &Remapping::remap);

	py::class_<Inverse, std::shared_ptr<Inverse>, Remapping>(m, "Inverse")
		.def(PYTMKS(Inverse, std::shared_ptr<Field>, float));

	py::class_<Exponential, std::shared_ptr<Exponential>, Remapping>(m, "Exponential")
		.def(PYTMKS(Exponential, std::shared_ptr<Field>, float));

	py::class_<Smoothstep, std::shared_ptr<Smoothstep>, Remapping>(m, "Smoothstep")
		.def(PYTMKS(Smoothstep, std::shared_ptr<Field>, float, float));

	py::class_<CellNoise, std::shared_ptr<CellNoise>, Field>(m, "CellNoise")
		.def(PYTMKS(CellNoise, float, int));

	// Material

	py::class_<Material>(m, "Material")
		.def_readwrite("red", &Material::red)
		.def_readwrite("green", &Material::green)
		.def_readwrite("blue", &Material::blue)
		.def_readwrite("albedo", &Material::albedo)
		.def_readwrite("roughness", &Material::roughness)
		.def_readwrite("ior", &Material::ior)
		.def("set_solid_color", &Material::set_solid_color);

	// Root estimation

	py::class_<RootEstimator>(m, "RootEstimator")
		.def_readwrite("t_min", &RootEstimator::t_min)
		.def_readwrite("t_max", &RootEstimator::t_max)
		.def_readwrite("sampling_step", &RootEstimator::sampling_step)
		.def_readwrite("threshold", &RootEstimator::threshold)
		.def_readwrite("max_iterations", &RootEstimator::max_iterations);

	// Surface

	py::class_<Surface, std::shared_ptr<Surface>>(m, "Surface")
		.def_readwrite("material", &Surface::material);

	py::class_<Sphere, std::shared_ptr<Sphere>, Surface>(m, "Sphere")
		.def(PYTMKS(Sphere, const Vec3&, float));

	py::class_<Plane, std::shared_ptr<Plane>, Surface>(m, "Plane")
		.def(PYTMKS(Plane, const Vec3&, const Vec3&));

	py::class_<Tube, std::shared_ptr<Tube>, Surface>(m, "Tube")
		.def(PYTMKS(Tube, const Vec3&, const Vec3&, float));

	py::class_<ImplicitSurface, std::shared_ptr<ImplicitSurface>, Surface>(m, "ImplicitSurface")
		.def(PYTMKS(ImplicitSurface, std::shared_ptr<Field>))
		.def_readwrite("root_estimator", &ImplicitSurface::root_estimator);

	// Scene

	py::class_<Scene>(m, "Scene")
		.def(py::init<>())
		.def("set_camera", &Scene::set_camera)
		.def("add_light", &Scene::add_light)
		.def("add_surface", &Scene::add_surface)
		.def("set_env_light", &Scene::set_env_light);

	// Rendering

	py::class_<RayTracer>(m, "RayTracer")
		.def(py::init<int, int>())
		.def_readwrite("pixel_sampling", &RayTracer::pixel_sampling)
		.def_readwrite("max_bounce", &RayTracer::max_bounce)
		.def_readwrite("rays_per_bounce", &RayTracer::rays_per_bounce)
		.def_readwrite("env_sampling", &RayTracer::env_sampling)
		.def("render", &RayTracer::render);

	// IO

	m.def("write_EXR", &write_EXR, 
		py::arg("layers"), 
		py::arg("path"));

	m.def("read_EXR", &read_EXR,
		py::arg("path"));
}

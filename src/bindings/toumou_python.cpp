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

	py::class_<Ray>(m, "Ray")
		.def_readonly("origin", &Ray::origin)
		.def_readonly("dir", &Ray::dir)
		.def("at", &Ray::at);

	m.def("distance", &distance);
	m.def("dot", &dot);
	m.def("cross", &cross);
	m.def("trace", &trace);

	// Camera

	py::class_<Camera, std::shared_ptr<Camera>>(m, "Camera")
		.def(PYTMKS(Camera, float, float, float, float))
		.def("move_to", &Camera::move_to);

	// Light

	py::class_<Light, std::shared_ptr<Light>>(m, "Light")
		.def_readwrite("brightness", &Light::brightness)
		.def_readwrite("color", &Light::color);

	py::class_<PointLight, std::shared_ptr<PointLight>, Light>(m, "PointLight")
		.def(PYTMKS(PointLight, const Color&, float, const Vec3&));

	py::class_<DirectionalLight, std::shared_ptr<DirectionalLight>, Light>(m, "DirectionalLight")
		.def(PYTMKS(DirectionalLight, const Color&, float, const Vec3&));

	// Field

	py::class_<Field, std::shared_ptr<Field>>(m, "Field")
		.def("value", &Field::value);

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

	// Material

	py::class_<Material>(m, "Material")
		.def_readwrite("base_color", &Material::base_color)
		.def_readwrite("albedo", &Material::albedo);

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
		.def("add_surface", &Scene::add_surface);

	// Image

	// Rendering

	py::class_<RayTracer>(m, "RayTracer")
		.def(py::init<int, int, int, int, int>())
		.def("render", &RayTracer::render);

	// IO

	m.def("write_EXR", &write_EXR, 
		py::arg("layers"), 
		py::arg("path"));
}

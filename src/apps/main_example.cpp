#include <lava/all.hpp>

#include <spdlog/spdlog.h>

#include <string>

using namespace lava;


int main(int argc, char* argv[])
{
	spdlog::info("*** Lava example program ***");

	std::string path;

	if (argc == 1) {
		spdlog::error("Missing path argument");
		return EXIT_FAILURE;
	}
	else if (argc == 2) {
		path = argv[1];
	}
	else {
		spdlog::error("Too many arguments");
		return EXIT_FAILURE;
	}

	Scene scene;

	auto camera = make_camera(Vec3(0, 0, 10), 
							  Vec3(0, 0, -1), Vec3(0, 1, 0), 
							  36.f, 90.f, 
							  0.1f, 100.f);
	scene.set_camera(camera);

	auto light = make_sun_light(Vec3(1, 1, 1).normalized(), 1.f, Color(1, 1, 1));
	scene.add_light(light);

	auto sphere_left = make_sphere(Vec3(-3, 0, 0), 1.f);
	(sphere_left->material).base_color = Color(1, 0, 0);
	(sphere_left->material).roughness = 0.1f;
	scene.add_surface(sphere_left);

	auto sphere_center = make_sphere(Vec3(0, 0, 0), 1.f);
	(sphere_center->material).base_color = Color(0, 1, 0);
	(sphere_center->material).roughness = .4f;
	scene.add_surface(sphere_center);

	auto sphere_right = make_sphere(Vec3(3, 0, 0), 1.f);
	(sphere_right->material).base_color = Color(0, 0, 1);
	(sphere_right->material).roughness = .8f;
	scene.add_surface(sphere_right);

	auto floor = make_plane(Vec3(0, -1, 0), Vec3(0, 1, 0));
	(floor->material).base_color = Color(.8f, .8f, .8f);
	(floor->material).k_specular = 0.f;
	scene.add_surface(floor);

	RenderParams params;
	RenderLayers layers(1280, 720);
	render(scene, params, layers, [](float progress){
		spdlog::info("progress: {}", progress);
	});

	write_EXR(layers, path);

	return EXIT_SUCCESS;
}

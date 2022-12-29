#include <lava/camera.hpp>


namespace lava {

std::shared_ptr<Camera> make_camera(const Vec3& location, 
									const Vec3& forward, const Vec3& up, 
									float sensor_width, float field_of_view, 
									float z_near, float z_far)
{
	auto cam = std::make_shared<Camera>();
	cam->location = location;
	cam->forward = forward;
	cam->up = up;
	cam->right = cross(forward, up);
	cam->sensor_width = sensor_width * 1e-3;
	cam->field_of_view = field_of_view * 3.14f / 180.f;
	cam->z_near = z_near;
	cam->z_far = z_far;
	return cam;
}

}

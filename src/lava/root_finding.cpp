#include <lava/root_finding.hpp>


namespace lava {

bool RootFinder::find_first_root(const Ray& ray,
								 std::function<float(const Vec3&)> field,
								 std::function<float(const Ray&, float)> ray_derivative,
								 float& t_root) const
{
	float t = t_min;

	// 1st step: Linear sampling
	bool has_root = false;
	while (t < t_max) {
		Vec3 pos = ray.at(t);
		if (field(pos) > 0) {
			return false;
		}
		Vec3 next_pos = ray.at(t + sampling_step);
		if (field(next_pos) > 0) {
			has_root = true;
			break;
		}
		t += sampling_step;
	}
	if (!has_root) {
		return false;
	}

	// 2nd step: Refinement with Newton's method
	int iter = 0;
	float value = field(ray.at(t));
	while (std::abs(value) > 1e-3f && iter < max_iterations) {
		float derivative = ray_derivative(ray, t);
		t -= value / derivative;
		value = field(ray.at(t));
		iter++;
	}

	t_root = t;
	return true;
}

}

#include <toumou/denoising.hpp>

#include <utility>
#include <limits>


namespace toumou {

Image<Color> VMFDenoiser::denoise(const RayTracer& rt) const
{
	// Noisy color output of ray tracing
	const Image<Color>& noisy_map = rt.image;

	// Dimensions
	const int width = noisy_map.width();
	const int height = noisy_map.height();
	const int window_size = 2 * window_half_size + 1;
	const int window_area = window_size * window_size;

	// Filtered image
	Image<Color> img(width, height);

	for (int j = 0; j < width; ++j) {
		for (int i = 0; i < height; ++i) {
			// Vector median calculation in local window

			// Retrieve coordinates of pixels in local window
			std::vector<std::pair<int, int>> coordinates;
			for (int delta_j = -window_half_size; delta_j <= window_half_size; ++delta_j) {
				int j_local = j + delta_j;
				if (j_local < 0 || j_local >= width) continue;

				for (int delta_i = -window_half_size; delta_i <= window_half_size; ++delta_i) {
					int i_local = i + delta_i;
					if (i_local < 0 || i_local >= height) continue;

					coordinates.push_back(std::make_pair(i_local, j_local));
				}
			}
			
			// Compute distances between pixel values
			// and retrieve the pixel that minimizes that distance
			std::pair<int, int> coords_min(i, j);
			float dist_min = std::numeric_limits<float>::max();
			for (const auto& p : coordinates) {
				float dist = 0.f;

				for (const auto& q : coordinates) {
					dist += (noisy_map.at(p.first, p.second) - noisy_map.at(q.first, q.second)).length();
				}

				if (dist < dist_min) {
					coords_min = p;
					dist_min = dist;
				}
			}

			// Store result
			img.set(i, j, noisy_map.at(coords_min.first, coords_min.second));
		}
	}

	return img;
}

}

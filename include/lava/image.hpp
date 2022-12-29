#pragma once

#include <vector>


namespace lava {

/**
 * @brief A template class to store and manipulate a 2D pixel grid.
 * @tparam T The type of the pixels.
 */
template<typename T>
class Image {
public:

	/**
	 * @brief Initialize a 2D pixel grid.
	 * @param[in] w The grid's width.
	 * @param[in] h The grid's height.
	 */
	Image(int w, int h);

	/// Access the grid's width.
	int width() const;

	/// Access the grid's height.
	int height() const;

	/**
	 * @brief Retrieve the pixel value at the given grid coordinates.
	 * @param[in] i The pixel's row.
	 * @param[in] j The pixel's column.
	 * @return The pixel value at (i, j).
	 */
	T at(int i, int j) const;

	/**
	 * @brief Set the pixel value at the given grid coordinates.
	 * @param[in] i The pixel's row.
	 * @param[in] j The pixel's column.
	 * @param[in] value The new pixel value.
	 */
	void set(int i, int j, const T& value);

	/// Retrieve the underlying data as a linear array.
	const T* data() const;

private:

	/// Pixel grid dimensions.
	int m_width, m_height;

	/// Linear array storing the pixel values.
	std::vector<T> m_data;

	/// Retrieve the index in the data array corresponding to the given pixel coordinates.
	int index(int i, int j) const;

};

}

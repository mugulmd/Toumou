#include <lava/image.hpp>
#include <lava/color.hpp>


namespace lava {

template<typename T>
Image<T>::Image(int w, int h) : 
	m_width(w), m_height(h), m_data(w * h)
{}

template<typename T>
int Image<T>::width() const
{
	return m_width;
}

template<typename T>
int Image<T>::height() const
{
	return m_height;
}

template<typename T>
int Image<T>::index(int i, int j) const
{
	return i * m_width + j;
}

template<typename T>
T Image<T>::at(int i, int j) const
{
	return m_data[index(i, j)];
}

template<typename T>
void Image<T>::set(int i, int j, const T& value)
{
	m_data[index(i, j)] = value;
}

template<typename T>
const T* Image<T>::data() const
{
	return m_data.data();
}

template class Image<Color>;

}

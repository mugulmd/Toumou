#include <toumou/io.hpp>
#include <toumou/geometry.hpp>

#include <OpenEXR/ImfNamespace.h>
#include <OpenEXR/ImfHeader.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfFrameBuffer.h>
#include <OpenEXR/ImfOutputFile.h>
#include <OpenEXR/ImfInputFile.h>

#include <Imath/ImathBox.h>

namespace IMF = OPENEXR_IMF_NAMESPACE;
using namespace IMF;


namespace toumou {

void write_EXR(const RayTracer& rt, const std::string& path)
{
	const int width = rt.image.width();
	const int height = rt.image.height();

	Header header(width, height);
	header.channels().insert("Color.R", Channel(IMF::FLOAT));
	header.channels().insert("Color.G", Channel(IMF::FLOAT));
	header.channels().insert("Color.B", Channel(IMF::FLOAT));
	header.channels().insert("Normal.X", Channel(IMF::FLOAT));
	header.channels().insert("Normal.Y", Channel(IMF::FLOAT));
	header.channels().insert("Normal.Z", Channel(IMF::FLOAT));
	header.channels().insert("Depth", Channel(IMF::FLOAT));
	header.channels().insert("Index", Channel(IMF::FLOAT));

	FrameBuffer buf;

	const Color* pixels_color = rt.image.data();
	buf.insert(
		"Color.R",
		Slice(
			IMF::FLOAT,
			(char*) &(pixels_color->x),
			sizeof(Color),
			sizeof(Color) * width
		)
	);
	buf.insert(
		"Color.G",
		Slice(
			IMF::FLOAT,
			(char*) &(pixels_color->y),
			sizeof(Color),
			sizeof(Color) * width
		)
	);
	buf.insert(
		"Color.B",
		Slice(
			IMF::FLOAT,
			(char*) &(pixels_color->z),
			sizeof(Color),
			sizeof(Color) * width
		)
	);

	const Vec3* pixels_normal = rt.normal_map.data();
	buf.insert(
		"Normal.X",
		Slice(
			IMF::FLOAT,
			(char*) &(pixels_normal->x),
			sizeof(Vec3),
			sizeof(Vec3) * width
		)
	);
	buf.insert(
		"Normal.Y",
		Slice(
			IMF::FLOAT,
			(char*) &(pixels_normal->y),
			sizeof(Vec3),
			sizeof(Vec3) * width
		)
	);
	buf.insert(
		"Normal.Z",
		Slice(
			IMF::FLOAT,
			(char*) &(pixels_normal->z),
			sizeof(Vec3),
			sizeof(Vec3) * width
		)
	);

	const float* pixels_depth = rt.depth_map.data();
	buf.insert(
		"Depth",
		Slice(
			IMF::FLOAT,
			(char*) pixels_depth,
			sizeof(float),
			sizeof(float) * width
		)
	);

	const float* pixels_uid = rt.index_map.data();
	buf.insert(
		"Index",
		Slice(
			IMF::FLOAT,
			(char*) pixels_uid,
			sizeof(float),
			sizeof(float) * width
		)
	);

	OutputFile file(path.c_str(), header);
	file.setFrameBuffer(buf);
	file.writePixels(height);
}

Image<Color> read_EXR(const std::string& path)
{
	InputFile file(path.c_str());

	Imath::Box2i dw = file.header().dataWindow();
	int width = dw.max.x - dw.min.x + 1;
	int height = dw.max.y - dw.min.y + 1;

	Image<Color> image(width, height);
	Color* pixels = image.data();

	FrameBuffer buf;

	buf.insert(
		"R",
		Slice(
			IMF::FLOAT,
			(char*) (&(pixels->x) - dw.min.x - dw.min.y * width),
			sizeof(Color),
			sizeof(Color) * width,
			1, 1,
			0.0
		)
	);

	buf.insert(
		"G",
		Slice(
			IMF::FLOAT,
			(char*) (&(pixels->y) - dw.min.x - dw.min.y * width),
			sizeof(Color),
			sizeof(Color) * width,
			1, 1,
			0.0
		)
	);

	buf.insert(
		"B",
		Slice(
			IMF::FLOAT,
			(char*) (&(pixels->z) - dw.min.x - dw.min.y * width),
			sizeof(Color),
			sizeof(Color) * width,
			1, 1,
			0.0
		)
	);

	file.setFrameBuffer(buf);
	file.readPixels(dw.min.y, dw.max.y);

	return image;
}

}

#include <toumou/io.hpp>
#include <toumou/image.hpp>
#include <toumou/color.hpp>
#include <toumou/geometry.hpp>

#include <OpenEXR/ImfNamespace.h>
#include <OpenEXR/ImfHeader.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfFrameBuffer.h>
#include <OpenEXR/ImfOutputFile.h>

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
			(char*)&(pixels_normal->x),
			sizeof(Vec3),
			sizeof(Vec3) * width
		)
	);
	buf.insert(
		"Normal.Y",
		Slice(
			IMF::FLOAT,
			(char*)&(pixels_normal->y),
			sizeof(Vec3),
			sizeof(Vec3) * width
		)
	);
	buf.insert(
		"Normal.Z",
		Slice(
			IMF::FLOAT,
			(char*)&(pixels_normal->z),
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

}

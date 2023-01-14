#include <lava/io.hpp>
#include <lava/image.hpp>
#include <lava/color.hpp>

#include <OpenEXR/ImfNamespace.h>
#include <OpenEXR/ImfHeader.h>
#include <OpenEXR/ImfChannelList.h>
#include <OpenEXR/ImfFrameBuffer.h>
#include <OpenEXR/ImfOutputFile.h>

namespace IMF = OPENEXR_IMF_NAMESPACE;
using namespace IMF;


namespace lava {

void write_EXR(const RayTracer& rt, const std::string& path)
{
	const int width = rt.image.width();
	const int height = rt.image.height();

	Header header(width, height);
	header.channels().insert("R", Channel(IMF::FLOAT));
	header.channels().insert("G", Channel(IMF::FLOAT));
	header.channels().insert("B", Channel(IMF::FLOAT));

	FrameBuffer buf;

	const Color* pixels_color = rt.image.data();
	buf.insert(
		"R",
		Slice(
			IMF::FLOAT,
			(char*) &(pixels_color->r),
			sizeof(Color),
			sizeof(Color) * width
		)
	);
	buf.insert(
		"G",
		Slice(
			IMF::FLOAT,
			(char*) &(pixels_color->g),
			sizeof(Color),
			sizeof(Color) * width
		)
	);
	buf.insert(
		"B",
		Slice(
			IMF::FLOAT,
			(char*) &(pixels_color->b),
			sizeof(Color),
			sizeof(Color) * width
		)
	);

	OutputFile file(path.c_str(), header);
	file.setFrameBuffer(buf);
	file.writePixels(height);
}

}

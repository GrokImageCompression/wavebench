/*
 *    Copyright (C) 2022 Grok Image Compression Inc.
 *
 *    This source code is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This source code is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cstdlib>

#define TCLAP_NAMESTARTSTRING "-"
#include "tclap/CmdLine.h"

#include "grk_includes.h"
using namespace grk;

int main(int argc, char** argv)
{
	try
	{
		TCLAP::CmdLine cmd("wavelet test bench command line", ' ', "1.0");
		TCLAP::ValueArg<uint32_t> widthArg("w", "width", "image width", false, 0,
										   "unsigned integer", cmd);
		TCLAP::ValueArg<uint32_t> heightArg("e", "height", "image height", false, 0,
											"unsigned integer", cmd);
		TCLAP::ValueArg<uint32_t> resolutionsArg("n", "resolutions", "number of resolutions", false,
												 0, "unsigned integer", cmd);
		TCLAP::ValueArg<uint32_t> reduceArg("r", "reduce", "number of resolutions to reduce", false,
											0, "unsigned integer", cmd);
		TCLAP::SwitchArg irreversibleArg("I", "irreversible", "irreversible", cmd);

		cmd.parse(argc, argv);

		uint32_t x0 = 501;
		uint32_t y0 = 501;
		uint32_t x1 = 1000 + x0;
		uint32_t y1 = 1000 + y0;
		uint8_t n = 5;
		uint8_t r = 0;
		bool irreversible = false;

		if(widthArg.isSet())
			x1 = x0 + widthArg.getValue();
		if(heightArg.isSet())
			y1 = y0 + heightArg.getValue();
		if(resolutionsArg.isSet())
			n = (uint8_t)resolutionsArg.getValue();
		if(reduceArg.isSet())
			r = (uint8_t)reduceArg.getValue();
		if(irreversibleArg.isSet())
			irreversible = true;

		// 1 create forward window
		auto fullTile = grk_rect32(x0, y0, x0, y0, x1, y1);
		auto windowFwd = new TileComponentWindow<int32_t>(
			!irreversible, true, grk_rect32(x0, y0, x0, y0, x1, y1), fullTile, n, 0);
		if(!windowFwd->alloc())
		{
			std::cerr << "Failed to allocate forward window" << std::endl;
			return -1;
		}

		// 2. copy data into forward window
		auto src = grk_buf2d<int32_t, AllocatorAligned>(fullTile);
		if(!src.alloc2d(false))
		{
			std::cerr << "Failed to allocate src buffer" << std::endl;
			return -1;
		}
		auto srcPtr = src.getBuffer();
		uint64_t ind = 0;
		for(uint32_t j = 0; j < src.height(); ++j)
		{
			uint64_t indLine = ind;
			for(uint32_t i = 0; i < src.width(); ++i)
				srcPtr[indLine++] = i & 255;
			ind += src.stride;
		}
		windowFwd->copy(n - 1, &src, false);

		// 3. run forward wavelet
		auto waveletFwd = std::make_unique<WaveletForward53<int32_t, 4, 16, 2>>(
			WaveletForward53<int32_t, 4, 16, 2>(windowFwd));
		if(!waveletFwd->run())
		{
			std::cerr << "Failed to run forward wavelet" << std::endl;
			return -1;
		}

		// 4. create reverse window
		auto windowRev =
			new TileComponentWindow<int32_t>(!irreversible, true, fullTile, fullTile, n, r);
		if(!windowRev->alloc())
		{
			std::cerr << "Failed to allocate reverse window" << std::endl;
			return -1;
		}

		// 5. run reverse wavelet
		auto waveletRev = std::make_unique<WaveletReverse53<int32_t, 4, 16, 2>>(
			WaveletReverse53<int32_t, 4, 16, 2>(windowRev));
		if(!waveletRev->run())
		{
			std::cerr << "Failed to run reverse wavelet" << std::endl;
			return -1;
		}

		// 6. cleanup
		delete windowRev;
		delete windowFwd;
	}
	catch(TCLAP::ArgException& e) // catch any exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		return 1;
	}

	return 0;
}

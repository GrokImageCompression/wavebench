/**
 *    Copyright (C) 2016-2022 Grok Image Compression Inc.
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
 *
 */
#pragma once

#include "grk_includes.h"
#include <stdexcept>
#include <algorithm>

/*
 Various coordinate systems are used to describe regions in the tile component buffer.

 1) Canvas coordinates:  JPEG 2000 global image coordinates.

 2) Tile component coordinates: canvas coordinates with sub-sampling applied

 */

#include "ResWindow.h"

namespace grk
{

template<class T>
constexpr T getFilterPad(bool isLossless)
{
	return isLossless ? 2 : 4;
}

template<typename T>
struct TileComponentWindow
{
	typedef grk_buf2d<T, AllocatorAligned> Buf2dAligned;

	TileComponentWindow(bool isLossless, bool wholeTileDecompress, grk_rect32 unreducedTileComp,
						grk_rect32 unreducedImageCompWindow, uint8_t numresolutions,
						uint8_t reduction)
	{
		// 1. set up bounds
		assert(reduction < numresolutions);
		grk_rect32 unreducedBounds = unreducedImageCompWindow.intersection(unreducedTileComp);
		assert(unreducedBounds.valid());
		auto reducedTileComp =
			ResWindow<T>::getBandWindow(reduction, BAND_ORIENT_LL, unreducedTileComp);

		// 2. generate resolutions
		auto currentRes = unreducedTileComp;
		for(uint8_t i = 0; i < numresolutions; ++i)
		{
			bool finalResolution = i == numresolutions - 1;
			tileResolutions_.push_back(currentRes);
			if(!finalResolution)
				currentRes = ResWindow<T>::getBandWindow(1, BAND_ORIENT_LL, currentRes);
		}
		std::vector<grk_rect32> windowResolutions_;
		auto currentWin = unreducedBounds;
		uint32_t filterWidth = wholeTileDecompress ? 0 : getFilterPad<uint32_t>(isLossless);
		for(uint8_t i = 0; i < numresolutions; ++i)
		{
			bool finalResolution = i == numresolutions - 1;
			windowResolutions_.push_back(currentWin);
			if(!finalResolution)
			{
				currentWin = ResWindow<T>::getBandWindow(1, BAND_ORIENT_LL, currentWin);
				currentWin.grow_IN_PLACE(filterWidth).clip_IN_PLACE(&tileResolutions_[i + 1]);
			}
		}
		std::reverse(tileResolutions_.begin(), tileResolutions_.end());
		std::reverse(windowResolutions_.begin(), windowResolutions_.end());

		// 3. create windows for all resolutions
		Buf2dAligned* resWindowBufLower = nullptr;
		for(uint8_t resno = 0; resno < numresolutions - reduction; ++resno)
		{
			auto resWindow = windowResolutions_[resno];
			auto resCurrent = new ResWindow<T>(tileResolutions_[resno], resWindow,
											   resWindowBufLower, filterWidth);
			resWindows_.push_back(resCurrent);
			resWindowBufLower = resCurrent->resWindowBufPadded_;
		}
	}
	virtual ~TileComponentWindow()
	{
		for(auto& b : resWindows_)
			delete b;
	}
	bool alloc()
	{
		for(auto& b : resWindows_)
		{
			if(!b->alloc(false))
				return false;
		}

		return true;
	}
	void copy(uint8_t resno, eBandOrientation orient, Buf2dAligned* rhs, bool writeToRHS)
	{
		assert(resno < resWindows_.size());
		auto band = getBand(resno, orient);
		if(writeToRHS)
			rhs->copy(band);
		else
			band->copy(rhs);
	}
	void copy(uint8_t resno, Buf2dAligned* rhs, bool writeToRHS)
	{
		assert(resno < resWindows_.size());
		auto win = resWindows_[resno]->resWindowBufPadded_;
		if(writeToRHS)
			rhs->copyFrom(win);
		else
			win->copyFrom(rhs);
	}

  protected:
	Buf2dAligned* getBand(uint8_t resno, eBandOrientation orient)
	{
		if(resno == 0)
		{
			assert(orient == BAND_ORIENT_LL);
			return resWindows_[0]->resWindowBufPadded_;
		}
		else
		{
			return resWindows_[resno]->bandWindowBuffersPadded_[orient];
		}
	}

	// windowed bounds for windowed decompress, otherwise full bounds
	std::vector<ResWindow<T>*> resWindows_;
	std::vector<grk_rect32> tileResolutions_;
};

} // namespace grk

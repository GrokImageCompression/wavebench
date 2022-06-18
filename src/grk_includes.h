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
#pragma once

#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <memory>

#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wimplicit-int-float-conversion"
#endif
#endif
#include <taskflow/taskflow.hpp>
#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif

#include "ChronoTimer.h"
#include "grk_intmath.h"
#include "geometry.h"
#include "buffer.h"
#include "ResWindow.h"
#include "TileComponentWindow.h"
#include "WaveletHWY.h"
#include "Wavelet.h"
#include "WaveletForward53.h"
#include "WaveletReverse53.h"
#include "util/testing.h"

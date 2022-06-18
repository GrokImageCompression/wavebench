/*
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
 *
 *    This source code incorporates work covered by the BSD 2-clause license.
 *    Please see the LICENSE file in the root directory for details.
 *
 */
#pragma once

namespace grk
{
/**
 Divide an integer by another integer and round upwards
 @param  a integer of type T
 @param  b integer of type T
 @return a divided by b
 */
template<typename T>
uint32_t ceildiv(T a, T b)
{
	assert(b);
	return (uint32_t)((a + (uint64_t)b - 1) / b);
}

template<typename T>
T ceildivpow2(T a, uint32_t b)
{
	return (T)((a + ((uint64_t)1 << b) - 1) >> b);
}

} // namespace grk

#include "WaveletHWY.h"

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "WaveletHWY.cpp"
#include <hwy/foreach_target.h>
#include <hwy/highway.h>
HWY_BEFORE_NAMESPACE();
namespace grk
{
namespace HWY_NAMESPACE
{
	using namespace hwy::HWY_NAMESPACE;

	static size_t hwy_num_lanes(void)
	{
		const HWY_FULL(int32_t) di;
		return Lanes(di);
	}

} // namespace HWY_NAMESPACE
} // namespace grk
HWY_AFTER_NAMESPACE();

#if HWY_ONCE
namespace grk
{
HWY_EXPORT(hwy_num_lanes);

size_t hwy_num_lanes(void)
{
	return HWY_DYNAMIC_DISPATCH(hwy_num_lanes)();
}

}; // namespace grk

#endif

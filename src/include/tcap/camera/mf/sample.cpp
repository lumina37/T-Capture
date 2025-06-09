#include <mfidl.h>
#include <mfobjects.h>

#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/sample.hpp"
#endif

namespace tcap::mf {

SampleBox::SampleBox(CComPtr<IMFSample>&& pSample) noexcept : pSample_(pSample) {}

std::expected<SampleBox, Error> SampleBox::create(CComPtr<IMFSample>&& pSample) noexcept {
    return SampleBox{std::move(pSample)};
}

}  // namespace tcap::mf

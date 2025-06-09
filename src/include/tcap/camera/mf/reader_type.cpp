#include <atlbase.h>
#include <mferror.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#include "tcap/camera/mf/media_type.hpp"
#include "tcap/camera/mf/reader.hpp"
#include "tcap/helper/error.hpp"

#ifndef _TCAP_LIB_HEADER_ONLY
#    include "tcap/camera/mf/reader_type.hpp"
#endif

namespace tcap::mf {

ReaderTypeBox::ReaderTypeBox(MediaTypeBox&& currentMediaTypeBox,
                             std::vector<MediaTypeBox>&& nativeMediaTypeBoxes) noexcept
    : currentMediaTypeBox_(std::move(currentMediaTypeBox)), nativeMediaTypeBoxes_(std::move(nativeMediaTypeBoxes)) {}

}  // namespace tcap::mf

/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <hb.h>
#include <memory>

namespace rl {
namespace type {

using HBBlobPtr = std::unique_ptr<hb_blob_t, decltype(&hb_blob_destroy)>;
using HBFacePtr = std::unique_ptr<hb_face_t, decltype(&hb_face_destroy)>;
using HBBufferPtr = std::unique_ptr<hb_buffer_t, decltype(&hb_buffer_destroy)>;

}  // namespace type
}  // namespace rl

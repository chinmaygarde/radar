/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/Mutex.h>
#include <Core/URI.h>
#include <Typography/Types.h>
#include <hb-ft.h>

namespace rl {
namespace type {

class FTFace {
 public:
  FTFace();

  FTFace(FT_Library library, const core::URI& uri);

  FTFace(FTFace&&);

  ~FTFace();

  bool isValid() const;

  const std::string& postscriptName() const;

 private:
  FT_Face _handle;
  bool _isValid;
  std::string _postscriptName;

  friend class FTFaceAccess;

  RL_DISALLOW_COPY_AND_ASSIGN(FTFace);
};

class RL_SCOPED_CAPABILITY FTFaceAccess {
 public:
  FTFaceAccess(core::Mutex& mutex, FTFace& face) RL_ACQUIRE(mutex);

  ~FTFaceAccess() RL_RELEASE();

  bool setPointSize(Point size);

  RL_WARN_UNUSED_RESULT
  hb_font_t* createHBFont() const;

 private:
  core::Mutex& _mutex;
  FTFace& _face;

  RL_DISALLOW_COPY_AND_ASSIGN(FTFaceAccess);
};

}  // namespace type
}  // namespace rl

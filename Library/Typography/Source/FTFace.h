/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>
#include <Typography/Types.h>
#include <hb-ft.h>
#include <mutex>

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

class FTFaceAccess {
 public:
  FTFaceAccess(std::mutex& mutex, FTFace& face);

  ~FTFaceAccess();

  bool setPointSize(Point size);

  RL_WARN_UNUSED_RESULT
  hb_font_t* createHBFont() const;

 private:
  std::mutex& _mutex;
  FTFace& _face;

  RL_DISALLOW_COPY_AND_ASSIGN(FTFaceAccess);
};

}  // namespace type
}  // namespace rl

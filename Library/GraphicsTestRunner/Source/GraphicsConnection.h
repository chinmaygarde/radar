/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Geometry/Size.h>
#include <memory>

namespace rl {
namespace testrunner {

class GraphicsConnection {
 public:
  static std::unique_ptr<GraphicsConnection> Create(const geom::Size& size);

  virtual ~GraphicsConnection();

  virtual bool isValid() const;

  virtual bool activate();

  virtual bool deactivate();

 protected:
  GraphicsConnection();

 private:
  std::unique_ptr<GraphicsConnection> _impl;

  RL_DISALLOW_COPY_AND_ASSIGN(GraphicsConnection);
};

}  // namespace testrunner
}  // namespace rl

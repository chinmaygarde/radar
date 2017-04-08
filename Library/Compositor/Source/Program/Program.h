/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/ThreadGuard.h>
#include <memory>
#include <string>
#include <vector>
#include "OpenGL.h"

namespace rl {
namespace compositor {

class Program {
 public:
  Program(std::string vertexShader, std::string fragmentShader);

  virtual ~Program();

  /**
   *  Links the program if necessary. Must be called on the compositor. If not
   *  called explicitly, program linking will occur before first use.
   */
  void linkIfNecessary();

  /**
   *  Uses the program, linking it first if necessary.
   *
   *  @return if program use was successful
   */
  bool use();

 protected:
  /**
   *  Invoked each time the program is linked. Subclasses may use this facility
   *  to cache indices and such. The base implementation does nothing.
   */
  virtual void onLinkSuccess() = 0;

  /**
   *  Fetches the index for the specified attribute
   *
   *  @param attribute the attribute name
   *
   *  @return the attribute index
   */
  GLint indexForAttribute(const std::string& attribute);

  /**
   *  Fetches the index for the specifed uniform
   *
   *  @param uniform the uniform name
   *
   *  @return the uniform index
   */
  GLint indexForUniform(const std::string& uniform);

 private:
  RL_DEBUG_THREAD_GUARD(_guard);
  std::string _vertexShader;
  std::string _fragmentShader;
  bool _linkingComplete;
  GLuint _program;

  RL_DISALLOW_COPY_AND_ASSIGN(Program);
};

}  // namespace compositor
}  // namespace rl

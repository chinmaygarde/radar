/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "StrokeProgram.h"

namespace rl {
namespace compositor {

static constexpr char kStrokeVertexShader[] = R"--(

  attribute vec2 A_Position;
  attribute vec2 A_Normal;
  attribute float A_SegmentContinuation;

  uniform mat4 U_ModelViewProjectionMatrix;
  uniform vec2 U_Size;
  uniform float U_StrokeSize;
  uniform float U_Feather;

  varying float V_SegmentContinuation;
  varying vec2 V_StrokeOffset;

  void main() {
    vec2 strokeOffset = A_Normal * vec2((U_StrokeSize * 0.5) + U_Feather);

    vec2 vertexPosition = (A_Position * U_Size) + strokeOffset;

    gl_Position =  U_ModelViewProjectionMatrix * vec4(vertexPosition, 0.0, 1.0);

    // For line breaks.
    V_SegmentContinuation = A_SegmentContinuation;

    // For interpolation at the edges for AA.
    V_StrokeOffset = strokeOffset;
  }

  )--";

static constexpr char kStrokeFragmentShader[] = R"--(

#ifdef GL_ES
  precision mediump float;
#endif

  uniform vec4 U_ContentColor;
  uniform float U_StrokeSize;
  uniform float U_Feather;

  varying float V_SegmentContinuation;
  varying vec2 V_StrokeOffset;

  void main() {
    float halfStroke = U_StrokeSize * 0.5;

    // Smoothly interpolate between the edge of the stroke and the edge with
    // the added feather distance.
    float featherComponent = 1.0 -
      smoothstep(halfStroke, halfStroke + U_Feather, length(V_StrokeOffset));

    // V_SegmentContinuation is interpolated.
    // We floor it to make the value either 0 or 1.
    float continuation = floor(V_SegmentContinuation);

    gl_FragColor = vec4(U_ContentColor.xyz,
                        U_ContentColor.w * continuation * featherComponent);
  }
  
  )--";

StrokeProgram::StrokeProgram()
    : Program(kStrokeVertexShader, kStrokeFragmentShader) {}

StrokeProgram::~StrokeProgram() = default;

void StrokeProgram::onLinkSuccess() {
  _modelViewProjectionUniform = indexForUniform("U_ModelViewProjectionMatrix");
  _contentColorUniform = indexForUniform("U_ContentColor");
  _sizeUniform = indexForUniform("U_Size");
  _featherUniform = indexForUniform("U_Feather");
  _strokeSizeUniform = indexForUniform("U_StrokeSize");
  _positionAttribute = indexForAttribute("A_Position");
  _normalAttribute = indexForAttribute("A_Normal");
  _segmentContinuationAttribute = indexForAttribute("A_SegmentContinuation");
}

GLint StrokeProgram::modelViewProjectionUniform() const {
  return _modelViewProjectionUniform;
}

GLint StrokeProgram::contentColorUniform() const {
  return _contentColorUniform;
}

GLint StrokeProgram::sizeUniform() const {
  return _sizeUniform;
}

GLint StrokeProgram::featherUniform() const {
  return _featherUniform;
}

GLint StrokeProgram::strokeSizeUniform() const {
  return _strokeSizeUniform;
}

GLint StrokeProgram::positionAttribute() const {
  return _positionAttribute;
}

GLint StrokeProgram::normalAttribute() const {
  return _normalAttribute;
}

GLint StrokeProgram::segmentContinuationAttribute() const {
  return _segmentContinuationAttribute;
}

}  // namespace compositor
}  // namespace rl

# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

################################################################################
# Library
################################################################################

file(GLOB URI_PARSER_SRC
  Source/src/UriCommon.c
  Source/src/UriCompare.c
  Source/src/UriEscape.c
  Source/src/UriFile.c
  Source/src/UriIp4.c
  Source/src/UriIp4Base.c
  Source/src/UriNormalize.c
  Source/src/UriNormalizeBase.c
  Source/src/UriParse.c
  Source/src/UriParseBase.c
  Source/src/UriQuery.c
  Source/src/UriRecompose.c
  Source/src/UriResolve.c
  Source/src/UriShorten.c
)

add_library(uriparser ${URI_PARSER_SRC})

target_include_directories(uriparser
  PUBLIC
    Headers
  PRIVATE
    Source
    Headers/uriparser
)

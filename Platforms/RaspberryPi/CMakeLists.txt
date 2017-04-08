# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

file(GLOB RADAR_RASPI_SOURCES
  "Source/*.h"
  "Source/*.cc"
  "../../Samples/*.h"
  "../../Samples/*.cc"
)

# Generate the Executable

include_directories(
  "../../Samples"
  "/opt/vc/include"
  "/opt/vc/include/interface/vcos/pthreads"
  "/opt/vc/include/interface/vmcs_host/linux"
)

link_directories(
  "/opt/vc/lib"
)

add_executable(RadarRaspberryPi ${RADAR_RASPI_SOURCES})

target_link_libraries(RadarRaspberryPi
  Shell
  pthread
  rt
  EGL
  GLESv2
  openmaxil
  bcm_host
  vcos
  vchiq_arm
)

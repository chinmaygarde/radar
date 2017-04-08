# This source file is part of the Radar project.
# Licensed under the MIT License. See LICENSE file for details.

file(GLOB RADAR_NACL_SOURCES
  "Source/*.h"
  "Source/*.cc"
  "../../Samples/*.h"
  "../../Samples/*.cc"
)

# Generate the Executable

include_directories ("../../Samples")

add_executable(RadarNacl ${RADAR_NACL_SOURCES})

target_link_libraries(RadarNacl
  Shell
  "ppapi"
  "ppapi_cpp"
  "nacl_io"
)

# Finalize PEXEs

# Execuatable Shell
add_custom_command(
  OUTPUT "RadarNacl.pexe"
  COMMAND ${RL_PNACL_FINALIZE} -o RadarNacl.pexe RadarNacl
  DEPENDS RadarNacl
  COMMENT "Finalizing Native Client PEXE" VERBATIM
)

add_custom_target(
  RadarNaclPexe ALL
  DEPENDS "RadarNacl.pexe"
)

# Copy over the HTML harness and manifest

configure_file("Resources/Radar.html" "Radar.html" COPYONLY)
configure_file("Resources/Radar.nmf" "Radar.nmf" COPYONLY)
configure_file("Resources/Server" "Server" COPYONLY)
configure_file("Resources/GDBInit" "GDBInit" COPYONLY)
configure_file("Resources/Debug" "Debug" COPYONLY)

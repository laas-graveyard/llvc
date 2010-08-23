##----------------------------------------------------
## Copyright 2010, CNRS-AIST JRL
## Author Clement Petit
## Edit   Claire Dune & Stephane Embarki
##
## Model tracker LLVS client
##----------------------------------------------------

include(LLVT-toolbox)

ADD_LLV_CLIENT(
  ${PROJECT_NAME}
  SHARED LIBRARY
  SOURCES
	action-grab.cpp
	action-tracking.cpp
	action-tracking-mbt.cpp
	action-tracking-kalman.cpp
	action-tracking-with-command.cpp
	action-display.cpp
	action-display-mbt.cpp
	action-display-kalman.cpp
	action-display-with-command.cpp
	tools/indent.cc
	tools/visp-io.cc
  COMPILE_FLAGS
  "-DCMAKE_INSTALL_PREFIX='\"${CMAKE_INSTALL_PREFIX}\"' -g -Wall -fexceptions "
  )

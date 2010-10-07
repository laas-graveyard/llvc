# Copyright (C) 2010 Claire Dune, Clement Petit, Stephane Embarki,
#                    Thomas Moulard, CNRS-AIST JRL, CNRS/AIST.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

INCLUDE(../cmake/idl.cmake)

# Add required definitions.
ADD_DEFINITIONS(${VISP_CFLAGS})
ADD_DEFINITIONS(${LLVS_CFLAGS})

# The shared library is being built right now.
# Required for dllimport/dllexport mechanisms in
# the generated header config.hh.
ADD_DEFINITIONS(-DBUILDING_LLVC)

# Define logging directory location.
ADD_DEFINITIONS(-DLLVC_LOGGINGDIR="${CMAKE_INSTALL_PREFIX}/var/log")

# This line is required to handle retro-compatibility issues with
# some CMake commandes which used to not search for their parameters
# in the build directory.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Generate LLVS IDL.
SET(LLVT_IDL_DIR ${LLVT_PREFIX}/share/idl/llvs)
GENERATE_IDL_FILE(BRep ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(BtlSlamInterface ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(common ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(GetGyroAndAccelerometer ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(LowLevelVisionSystem ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(ModelTrackerInterface ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(OpenHRPCommon ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(PointTrackerInterface ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(Scene ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(ServerCommand ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(StereoVision ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(ViewSimulator ${LLVT_IDL_DIR})
GENERATE_IDL_FILE(visionsensor ${LLVT_IDL_DIR})


# The client library.
ADD_LIBRARY(llvc
  SHARED
  ${ALL_IDL_STUBS}

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
  )

# Set shared library version.
SET_TARGET_PROPERTIES(llvc PROPERTIES SOVERSION ${PROJECT_VERSION})

# Link against Boost libraries.
TARGET_LINK_LIBRARIES(llvc ${VISP_LIBS})
TARGET_LINK_LIBRARIES(llvc ${LLVS_LDFLAGS})
TARGET_LINK_LIBRARIES(llvc ${LLVT_LDFLAGS})
TARGET_LINK_LIBRARIES(llvc ${OMNIORB4_LDFLAGS})

INSTALL(TARGETS llvc DESTINATION lib)

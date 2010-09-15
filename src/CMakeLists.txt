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

# Add required definitions.
ADD_DEFINITIONS(${VISP_CFLAGS})
ADD_DEFINITIONS(${LLVS_CFLAGS})

# The shared library is being built right now.
# Required for dllimport/dllexport mechanisms in
# the generated header config.hh.
ADD_DEFINITIONS(-DBUILDING_LLVC)

# Define logging directory location.
ADD_DEFINITIONS(-DLLVC_LOGGINGDIR="${CMAKE_INSTALL_PREFIX}/var/log")

# The client library.
ADD_LIBRARY(llvc
  SHARED
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
TARGET_LINK_LIBRARIES(llvc ${LLVS_LIBS})

INSTALL(TARGETS llvc DESTINATION lib)

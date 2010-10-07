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

# Requires at least CMake 2.6 to configure the package.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/image/visp.cmake)

SET(PROJECT_NAME llvc)
SET(PROJECT_VERSION 1.0.0.99)
SET(PROJECT_DESCRIPTION "Client for the Low Lever Vision Server")
SET(PROJECT_URL "")

SET(CXX_DISABLE_WERROR True)

SETUP_PROJECT()

SET(PKG_CONFIG_LIBS "${PKG_CONFIG_LIBS} ${LIBINCL_KW}llvs${LIB_EXT}")

FIND_VISP()

# ADD_REQUIRED_DEPENDENCY(LLVS)
ADD_REQUIRED_DEPENDENCY(LLVT)

# Search for omniORB.
ADD_REQUIRED_DEPENDENCY(omniORB4)

# Search for Boost.
FIND_PACKAGE(Boost 1.34 COMPONENTS filesystem system program_options)

# Source files exploration
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(example)

SETUP_PROJECT_FINALIZE()

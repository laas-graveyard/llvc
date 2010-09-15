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

SET(PROJECT_NAME llvc)
SET(PROJECT_VERSION 0.1)
SET(PROJECT_DESCRIPTION "Client for the Low Lever Vision Server")
SET(PROJECT_URL "")

SETUP_PROJECT()

# FIND_VISP()
# -----------
#
# Find ViSP through the use of visp-config.
#
# To make ViSP detection both robust and simple, rely on the
# visp-config tool instead of the specic CMake mechanism.
#
# One just has to make sure that visp-config is available in its path
# to make ViSP detected automagically.
#
# Implementation note: please do *not* modify this code to add a
# variable which override the path and the standard look-up algorithm,
# it introduces complexity for nothing. Fix your path instead.
MACRO(FIND_VISP)
  FIND_PROGRAM(VISP_CONFIG visp-config)
  EXECUTE_PROCESS(
    COMMAND ${VISP_CONFIG} --prefix
    OUTPUT_VARIABLE VISP_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  EXECUTE_PROCESS(
    COMMAND ${VISP_CONFIG} --cflags
    OUTPUT_VARIABLE VISP_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  EXECUTE_PROCESS(
    COMMAND ${VISP_CONFIG} --libs
    OUTPUT_VARIABLE VISP_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  EXECUTE_PROCESS(
    COMMAND ${VISP_CONFIG} --dumpversion
    OUTPUT_VARIABLE VISP_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
ENDMACRO(FIND_VISP)

FIND_VISP()
ADD_REQUIRED_DEPENDENCY(LLVS)

# Source files exploration
ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(unitTesting)

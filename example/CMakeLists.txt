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

# ADD_EXAMPLE NAME
# ----------------
#
# Add an example.
#
# NAME: example name
#
MACRO(ADD_EXAMPLE NAME)
  ADD_EXECUTABLE(${PROJECT_NAME}-${NAME} ${PROJECT_NAME}-${NAME}.cpp)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME}-${NAME} llvc)

  INSTALL(
    TARGETS ${NAME}
    DESTINATION bin
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
                OWNER_WRITE
                OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
    )
ENDMACRO(ADD_EXAMPLE)

# Add required definitions.
ADD_DEFINITIONS(${VISP_CFLAGS})
ADD_DEFINITIONS(${LLVS_CFLAGS})

# Search for generated headers.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/../src)


ADD_EXAMPLE(grab)
ADD_EXAMPLE(grab-with-display)
ADD_EXAMPLE(tracking)
ADD_EXAMPLE(tracking-kalman)
ADD_EXAMPLE(tracking-with-command)
ADD_EXAMPLE(calibration)

# Link against Boost when required.
TARGET_LINK_LIBRARIES(llvc-calibration ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(llvc-tracking ${Boost_LIBRARIES})

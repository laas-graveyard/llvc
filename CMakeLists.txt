##----------------------------------------------------
## Copyright 2010, CNRS-AIST JRL
## Author Clement Petit
## Edit   Claire Dune & Stephane Embarki
##
## Model tracker LLVS client
##----------------------------------------------------

# Project declaration
cmake_minimum_required (VERSION 2.6.4)
project (LLVC)
set(${PROJECT_NAME}_LIBRARY_NAME llvc-0)

# Dependent packages
find_package (
	Modules
	COMPONENTS
	image
	middleware
)

# Add required libraries
include(LLVT-toolbox)
ADD_PACKAGES(OpenCV)

# Add optional libraries
ADD_PACKAGE_OPTION(VISP DESCRIPTION "search for visual servoing plateform.")
ADD_PACKAGE_OPTION(OmniORB4 OUTPUT_VAR OMNIORB4)

# Search for Boost.
FIND_PACKAGE(Boost 1.34 COMPONENTS filesystem system program_options)

# Source files exploration
add_subdirectory (include)
add_subdirectory (src)
add_subdirectory (unitTesting)

# Add uninstall target
include(install-toolbox)
ADD_UNINSTALL_TARGET()
ADD_PKGCONFIG_FILE()

##----------------------------------------------------
## Copyright 2010, CNRS-AIST JRL
## Author Clement Petit
## Edit   Claire Dune & Stephane Embarki
##
## Model tracker LLVS client
##----------------------------------------------------

# Project declaration
cmake_minimum_required (VERSION 2.6.4)
project (MTC)

# Project specific module files
list (
	APPEND 
	CMAKE_MODULE_PATH 
	$ENV{ROBOTPKG_BASE}/share/cmake
)

# Dependent packages
find_package (
	Modules
	COMPONENTS
	image
	middleware
)

# Add required libraries
include(LLVT-toolbox)
ADD_LLV_PACKAGES(OpenCV)

# Add optional libraries
ADD_LLV_OPTION(VISP DESCRIPTION "search for visual servoing plateform.")
ADD_LLV_OPTION(MBT  DESCRIPTION "search for model tracker." DISABLED)
ADD_LLV_OPTION(NMBT DESCRIPTION "search for model tracker new version.")
ADD_LLV_OPTION(OmniORB4 OUTPUT_VAR OMNIORB4)

# Source files exploration
add_subdirectory (include)
add_subdirectory (src)

# Add uninstall target
include(install-toolbox)
ADD_UNINSTALL_TARGET()

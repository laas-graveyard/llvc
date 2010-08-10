##----------------------------------------------------
## Copyright 2010, CNRS-AIST JRL
## Author Clement Petit
## Edit   Claire Dune & Stephane Embarki
##
## Model tracker LLVS client
##----------------------------------------------------

include (include-toolbox)

# Generate the package dependent file
CONFIGURE_LLV_FILE(
	mtcConfig.h.cmake
	mtcConfig.h
)

# Add header list
ADD_HEADERS(
	FILES
		ActionTrackingClient.h
                ActionDisplayClient.h 
)

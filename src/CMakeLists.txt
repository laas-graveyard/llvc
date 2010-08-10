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
	SOURCES
		ActionTrackingClient.cpp
                ActionDisplayClient.cpp
		MTClient.cpp
	COMPILE_FLAGS
		"-g -Wall -Wno-unused -fexceptions "
)

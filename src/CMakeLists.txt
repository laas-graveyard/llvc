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
	ActionGrabClient.cpp
	ActionTrackingClient.cpp
	DisplayTrackingClient.cpp
  COMPILE_FLAGS
  "-g -Wall -fexceptions "
  )

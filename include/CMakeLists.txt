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
		llvc/action-grab.h
		llvc/action-tracking.h
                llvc/action-tracking-kalman.h
		llvc/action-tracking-mbt.h
                llvc/action-tracking-with-command.h
                llvc/action-display.h
                llvc/action-display-mbt.h
                llvc/action-display-kalman.h
                llvc/tools/indent.hh
                llvc/tools/visp-io.hh
)

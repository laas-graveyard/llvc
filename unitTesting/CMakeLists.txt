include(LLVT-toolbox)

ADD_LLV_CLIENT(
  ${PROJECT_NAME}-grab
  SOURCES
	llvc-grab.cpp
  LINK_WITH
	${PROJECT_NAME}
  COMPILE_FLAGS
  "-g -Wall -fexceptions "
  )

ADD_LLV_CLIENT(
  ${PROJECT_NAME}-grab-with-display
  SOURCES
	llvc-grab-with-display.cpp
  LINK_WITH
	${PROJECT_NAME}
  COMPILE_FLAGS
  "-g -Wall -fexceptions "
  )

ADD_LLV_CLIENT(
  ${PROJECT_NAME}-tracking
  SOURCES
	llvc-tracking.cpp
  LINK_WITH
	${PROJECT_NAME}
  COMPILE_FLAGS
  "-g -Wall -fexceptions "
  )

ADD_LLV_CLIENT(
  ${PROJECT_NAME}-tracking-kalman
  SOURCES
	llvc-tracking-kalman.cpp
  LINK_WITH
	${PROJECT_NAME}
  COMPILE_FLAGS
  "-g -Wall -fexceptions "
  )

ADD_LLV_CLIENT(
  ${PROJECT_NAME}-calibration
  SOURCES
	llvc-calibration.cpp
  LINK_WITH
	${PROJECT_NAME}
  COMPILE_FLAGS
  "-g -Wall -fexceptions "
  )

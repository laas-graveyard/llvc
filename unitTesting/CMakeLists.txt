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

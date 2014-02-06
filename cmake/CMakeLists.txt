cmake_minimum_required ( VERSION 2.8.8 )
project ( sledge )


set ( BUILD_HOST false )

add_subdirectory( "${LIBMOAI_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/libmoai" )

set (HOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../sledge")
set (SLEDGE_SRC
	${HOST_ROOT}/SledgeCore.cpp
	${HOST_ROOT}/SledgeGraphicsHandler.cpp
	${HOST_ROOT}/SledgeHost.cpp
	${HOST_ROOT}/SledgeHostMain.cpp
	${HOST_ROOT}/SledgeInputHandler.cpp
	${HOST_ROOT}/SledgeInputManager.cpp
)

if (BUILD_OSX)
  set (SLEDGE_SRC ${SLEDGE_SRC}
	${HOST_ROOT}/FolderWatcher-mac.mm
  )
endif (BUILD_OSX)

if (BUILD_WINDOWS)
  set (SLEDGE_SRC ${SLEDGE_SRC}
	${HOST_ROOT}/FolderWatcher-win.cpp
  )
endif (BUILD_WINDOWS)

set (SLEDGE_INCLUDES
	"${SDK_ROOT}/src"
	"${SDK_ROOT}/src/moai-config"
)

include_directories(${SLEDGE_INCLUDES})
add_executable(sledge ${SLEDGE_SRC})
target_link_libraries ( sledge 
  host-modules
  moai-sim
  moai-util
  moai-core
  zlcore
)

if ( BUILD_WINDOWS )
  target_link_libraries ( sledge
    winmm
  )
endif (BUILD_WINDOWS)

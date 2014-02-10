cmake_minimum_required ( VERSION 2.8.8 )
project ( sledge )


set ( BUILD_HOST false )

add_subdirectory( ${LIBMOAI_DIR} ${CMAKE_CURRENT_BINARY_DIR}/libmoaistatic )

set (HOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")


set (HUSKY_SRC
    ${HOST_ROOT}/Husky/shared/HuskyLoader.cpp
  )

set (HUSKY_INCLUDES
    ${HOST_ROOT}/Husky/shared
)


add_library(husky STATIC ${HUSKY_SRC})
target_include_directories(husky PUBLIC ${HUSKY_INCLUDES})
target_link_libraries(husky moai-core moai-util moai-sim zlcore)


set (MOAI_HUSKY_SRC
  ${HOST_ROOT}/moai-husky/MOAIHusky.cpp
)

set (MOAI_HUSKY_INCLUDES
  ${HOST_ROOT}/moai-husky
)

add_library(moai-husky STATIC ${MOAI_HUSKY_SRC})
target_include_directories(moai-husky PUBLIC ${MOAI_HUSKY_INCLUDES})
target_link_libraries(moai-husky moai-core moai-util moai-sim zlcore husky)

set (SLEDGE_SRC
	${HOST_ROOT}/sledge/SledgeCore.cpp
	${HOST_ROOT}/sledge/SledgeGraphicsHandler.cpp
	${HOST_ROOT}/sledge/SledgeHost.cpp
	${HOST_ROOT}/sledge/SledgeHostMain.cpp
	${HOST_ROOT}/sledge/SledgeInputHandler.cpp
	${HOST_ROOT}/sledge/SledgeInputManager.cpp
)

if (BUILD_OSX)
  set (SLEDGE_SRC ${SLEDGE_SRC}
	${HOST_ROOT}/sledge/FolderWatcher-mac.mm
  )
endif (BUILD_OSX)

if (BUILD_WINDOWS)
  set (SLEDGE_SRC ${SLEDGE_SRC}
	${HOST_ROOT}/sledge/FolderWatcher-win.cpp
  )
endif (BUILD_WINDOWS)

set (SLEDGE_INCLUDES
	"${SDK_ROOT}/src"
	"${SDK_ROOT}/src/moai-config"
  "${SDK_ROOT}/3rdparty/sdl2-2.0.0/include"
  "${HOST_ROOT}/"
)

include_directories(${SLEDGE_INCLUDES})
add_executable(sledge ${SLEDGE_SRC})
target_link_libraries ( sledge 
  moai-husky
  host-modules
  moai-core
  moai-util
  moai-sim
  SDL2-static
  zlcore
)

if ( BUILD_WINDOWS )
  target_link_libraries ( sledge
    winmm
  )
endif (BUILD_WINDOWS)

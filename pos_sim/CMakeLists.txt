#Project Setup

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(Pos)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

find_package(ROOT)
include_directories(${ROOT_INCLUDE_DIRS})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*hh)
file(GLOB MACRO_FILES
	"*.mac"
)
file(GLOB DATA_FILES
	"*.dat"
)

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(Positronium Positronium.cc ${sources} ${headers})
target_include_directories(Positronium PRIVATE include)
target_link_libraries(Positronium ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#add_custom_target(MultScint DEPENDS MultScint)

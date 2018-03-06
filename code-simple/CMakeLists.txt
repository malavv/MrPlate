cmake_minimum_required(VERSION 2.6)
project(SimpleMrPlate)

set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

set(EMULATOR_ROOT "${ROOT}/../code-emulator")

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${EMULATOR_ROOT}/cmake")
set(BIN_DIR ${SimpleMrPlate_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SDL2 "D:/libs/sdl/vs/SDL2-2.0.7")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(include)
include_directories("${EMULATOR_ROOT}/lib/Adafruit_SSD1306")
include_directories("${EMULATOR_ROOT}/lib/Adafruit-GFX-Library")
include_directories("${EMULATOR_ROOT}/src/emulator")
include_directories("${EMULATOR_ROOT}/src")

add_executable(SimpleMrPlate src/main.cpp
	${EMULATOR_ROOT}/src/emulator/emulator.cpp
	${EMULATOR_ROOT}/lib/Adafruit_SSD1306/Adafruit_SSD1306.cpp 
	${EMULATOR_ROOT}/lib/Adafruit-GFX-Library/Adafruit_GFX.cpp 
	${EMULATOR_ROOT}/src/emulator/ui.cpp)

target_link_libraries(SimpleMrPlate ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
install(TARGETS SimpleMrPlate RUNTIME DESTINATION ${BIN_DIR})

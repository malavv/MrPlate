FindSDL2.cmake
==============

FindSDL2 is a CMake module to locate SDL2 headers, libraries and additional linker flags. Following variables are provided:

  * SDL2_FOUND (True, if the system has SDL2.)
  * SDL2_INCLUDE_DIR (Path to the SDL2 include directory.)
  * SDL2_LIBRARIES (SDL2 libraries.)
  * SDL2_LINK_FLAGS (Additional linker flags for SDL2.)
  * SDL2_SHARED_LIBRARY_PATH (Path to the SDL2 shared library.)

### Example

    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
    target_link_libraries(tgt ${SDL2_LIBRARIES})
    set_target_properties(tgt PROPERTIES LINK_FLAGS ${SDL2_LINK_FLAGS})

### Notes

On Microsoft Windows platform, module expects to have environment variable SDL2_ROOT_DIR defined.



FindSDL2_image.cmake
==============

FindSDL2_image is a CMake module to locate SDL2_image headers and libraries. Following variables are provided:

  * SDL2_image_FOUND (True, if the system has SDL2_image.)
  * SDL2_image_INCLUDE_DIR (Path to the SDL2_image include directory.)
  * SDL2_image_LIBRARY (SDL2_image library.)
  * SDL2_image_SHARED_LIBRARIES (Paths to the SDL2_image shared libraries.)

### Example

    find_package(SDL2_image REQUIRED)
    include_directories(${SDL2_image_INCLUDE_DIR})
    target_link_libraries(tgt ${SDL2_image_LIBRARY})


### Notes

On Microsoft Windows platform, module expects to have environment variable SDL2_IMAGE_ROOT_DIR defined.



FindSDL2_ttf.cmake
==============

FindSDL2_ttf is a CMake module to locate SDL2_ttf headers and libraries. Following variables are provided:

  * SDL2_ttf_FOUND (True, if the system has SDL2_ttf.)
  * SDL2_ttf_INCLUDE_DIR (Path to the SDL2_ttf include directory.)
  * SDL2_ttf_LIBRARY (SDL2_ttf library.)
  * SDL2_ttf_SHARED_LIBRARIES (Paths to the SDL2_ttf shared libraries.)

### Example

    find_package(SDL2_ttf REQUIRED)
    include_directories(${SDL2_ttf_INCLUDE_DIR})
    target_link_libraries(tgt ${SDL2_ttf_LIBRARY})

### Notes

On Microsoft Windows platform, module expects to have environment variable SDL2_TTF_ROOT_DIR defined.

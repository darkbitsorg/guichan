cmake_minimum_required(VERSION 2.4.6)

# Path to local modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

project(guichan)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 9)
set(PATCH_LEVEL 0)
set(${PROJECT_NAME}_SOVERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_LEVEL})
set(${PROJECT_NAME}_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_LEVEL})

# set default cmake build type to RelWithDebInfo (None Debug Release
# RelWithDebInfo MinSizeRel)
# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE "RelWithDebInfo")
# endif()

include_directories("${PROJECT_SOURCE_DIR}/include")
add_definitions("-DGUICHAN_BUILD -DGUICHAN_EXTENSION_BUILD")

# The Guichan core library
file(GLOB GUICHAN_HEADER include/guichan.hpp)
file(
  GLOB
  GUICHAN_HEADERS
  include/guichan/actionevent.hpp
  include/guichan/actionlistener.hpp
  include/guichan/basiccontainer.hpp
  include/guichan/cliprectangle.hpp
  include/guichan/color.hpp
  include/guichan/deathlistener.hpp
  include/guichan/defaultfont.hpp
  include/guichan/event.hpp
  include/guichan/exception.hpp
  include/guichan/focushandler.hpp
  include/guichan/focuslistener.hpp
  include/guichan/font.hpp
  include/guichan/genericinput.hpp
  include/guichan/glut.hpp
  include/guichan/graphics.hpp
  include/guichan/gui.hpp
  include/guichan/image.hpp
  include/guichan/imagefont.hpp
  include/guichan/imageloader.hpp
  include/guichan/input.hpp
  include/guichan/inputevent.hpp
  include/guichan/key.hpp
  include/guichan/keyevent.hpp
  include/guichan/keyinput.hpp
  include/guichan/keylistener.hpp
  include/guichan/listmodel.hpp
  include/guichan/mouseevent.hpp
  include/guichan/mouseinput.hpp
  include/guichan/mouselistener.hpp
  include/guichan/platform.hpp
  include/guichan/rectangle.hpp
  include/guichan/selectionevent.hpp
  include/guichan/selectionlistener.hpp
  include/guichan/widget.hpp
  include/guichan/widgetlistener.hpp)
file(GLOB GUICHAN_WIDGET_HEADERS include/guichan/widgets/*.hpp)
file(GLOB GUICHAN_CONTRIB_WIDGET_HEADERS include/guichan/contrib/widgets/*.hpp)
file(GLOB GUICHAN_SRC src/*.cpp)
file(GLOB GUICHAN_WIDGET_SRC src/widgets/*.cpp)

# Grouping of the source for nicer display in IDEs such as Visual Studio
source_group(src FILES ${GUICHAN_HEADER})
source_group(src/guichan FILES ${GUICHAN_HEADERS} ${GUICHAN_SRC})
source_group(src/guichan/widgets FILES ${GUICHAN_WIDGET_HEADERS}
                                       ${GUICHAN_WIDGET_SRC})
source_group(src/guichan/contrib/widgets
             FILES ${GUICHAN_CONTRIB_WIDGET_HEADERS})

option(BUILD_GUICHAN_SHARED
       "Build the Guichan core library as a shared library." ON)
if(BUILD_GUICHAN_SHARED)
  set(GUICHAN_LIBRARY_TYPE SHARED)
else(BUILD_GUICHAN_SHARED)
  set(GUICHAN_LIBRARY_TYPE STATIC)
endif(BUILD_GUICHAN_SHARED)
add_library(
  ${PROJECT_NAME}
  ${GUICHAN_LIBRARY_TYPE}
  ${GUICHAN_HEADER}
  ${GUICHAN_HEADERS}
  ${GUICHAN_WIDGET_HEADERS}
  ${GUICHAN_CONTRIB_WIDGET_HEADERS}
  ${GUICHAN_SRC}
  ${GUICHAN_WIDGET_SRC})
add_custom_target(lib DEPENDS ${PROJECT_NAME}) # Create symlink
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES VERSION ${${PROJECT_NAME}_VERSION}
             SOVERSION ${${PROJECT_NAME}_SOVERSION}
             CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared libraries
                                   # without conflict
             OUTPUT_NAME ${PROJECT_NAME}) # Avoid conflicts between library and
                                          # binary target names
install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib
  PERMISSIONS
    OWNER_READ
    OWNER_WRITE
    OWNER_EXECUTE
    GROUP_READ
    GROUP_EXECUTE
    WORLD_READ
    WORLD_EXECUTE)
install(FILES ${GUICHAN_HEADER} DESTINATION include/)
install(FILES ${GUICHAN_HEADERS} DESTINATION include/guichan/)
install(FILES ${GUICHAN_WIDGET_HEADERS} DESTINATION include/guichan/widgets/)
install(FILES ${GUICHAN_CONTRIB_WIDGET_HEADERS}
        DESTINATION include/guichan/contrib/widgets/)

# The Guichan Allegro extension library
option(ENABLE_ALLEGRO "Enable the Guichan Allegro extension" ON)
option(BUILD_GUICHAN_ALLEGRO_SHARED
       "Build the Guichan Allegro extension library as a shared library." ON)
find_package(Allegro)
if(ENABLE_ALLEGRO AND ALLEGRO_FOUND)

  include_directories(${ALLEGRO_INCLUDE_DIR})

  # The Guichan Allegro extension source
  file(GLOB GUICHAN_ALLEGRO_HEADER include/guichan/allegro.hpp)
  file(GLOB GUICHAN_ALLEGRO_HEADERS include/guichan/allegro/*.hpp)
  file(GLOB GUICHAN_ALLEGRO_CONTRIB_HEADERS
       include/guichan/contrib/allegro/*.hpp)
  file(GLOB GUICHAN_ALLEGRO_SRC src/allegro/*.cpp)

  # Grouping of the source for nicer display in IDEs such as Visual Studio
  source_group(src/guichan FILES ${GUICHAN_ALLEGRO_HEADER})
  source_group(src/guichan/allegro FILES ${GUICHAN_ALLEGRO_HEADERS}
                                         ${GUICHAN_ALLEGRO_SRC})
  source_group(src/guichan/contrib/allegro
               FILES ${GUICHAN_ALLEGRO_CONTRIB_HEADERS})

  if(BUILD_GUICHAN_ALLEGRO_SHARED)
    set(GUICHAN_ALLEGRO_LIBRARY_TYPE SHARED)
  else(BUILD_GUICHAN_ALLEGRO_SHARED)
    set(GUICHAN_ALLEGRO_LIBRARY_TYPE STATIC)
  endif(BUILD_GUICHAN_ALLEGRO_SHARED)
  add_library(
    ${PROJECT_NAME}_allegro
    ${GUICHAN_ALLEGRO_LIBRARY_TYPE} ${GUICHAN_ALLEGRO_HEADER}
    ${GUICHAN_ALLEGRO_HEADERS} ${GUICHAN_ALLEGRO_CONTRIB_HEADERS}
    ${GUICHAN_ALLEGRO_SRC})
  if(WIN32)
    target_link_libraries(${PROJECT_NAME}_allegro ${ALLEG_LIBRARY}
                          ${PROJECT_NAME})
  endif(WIN32)
  add_custom_target(lib DEPENDS ${PROJECT_NAME}_allegro) # Create symlink
  set_target_properties(
    ${PROJECT_NAME}_allegro
    PROPERTIES VERSION ${${PROJECT_NAME}_VERSION}
               SOVERSION ${${PROJECT_NAME}_SOVERSION}
               CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared
                                     # libraries without conflict
               OUTPUT_NAME ${PROJECT_NAME}_allegro) # Avoid conflicts between
                                                    # library and binary target
                                                    # names
  install(
    TARGETS ${PROJECT_NAME}_allegro
    DESTINATION lib
    PERMISSIONS
      OWNER_READ
      OWNER_WRITE
      OWNER_EXECUTE
      GROUP_READ
      GROUP_EXECUTE
      WORLD_READ
      WORLD_EXECUTE)
  install(FILES ${GUICHAN_ALLEGRO_HEADER} DESTINATION include/guichan/)
  install(FILES ${GUICHAN_ALLEGRO_HEADERS} DESTINATION include/guichan/allegro/)
  install(FILES ${GUICHAN_ALLEGRO_CONTRIB_HEADERS}
          DESTINATION include/guichan/contrib/allegro/)
endif(ENABLE_ALLEGRO AND ALLEGRO_FOUND)

# The Guichan OpenGL extension library
option(ENABLE_OPENGL "Enable the Guichan OpenGL extension" ON)
option(BUILD_GUICHAN_OPENGL_SHARED
       "Build the Guichan OpenGL extension library as a shared library." ON)
find_package(OpenGL)
if(ENABLE_OPENGL AND OPENGL_FOUND)

  include_directories(${OPENGL_INCLUDE_DIR})

  # The Guican OpenGL extension source
  file(GLOB GUICHAN_OPENGL_HEADER include/guichan/opengl.hpp)
  file(GLOB GUICHAN_OPENGL_HEADERS include/guichan/opengl/*.hpp)
  file(GLOB GUICHAN_OPENGL_CONTRIB_HEADERS include/guichan/contrib/opengl/*.hpp)
  file(GLOB GUICHAN_OPENGL_SRC src/opengl/*.cpp)

  # Grouping of the source for nicer display in IDEs such as Visual Studio
  source_group(src/guichan FILES ${GUICHAN_OPENGL_HEADER})
  source_group(src/guichan/opengl FILES ${GUICHAN_OPENGL_HEADERS}
                                        ${GUICHAN_OPENGL_SRC})
  source_group(src/guichan/contrib/opengl
               FILES ${GUICHAN_OPENGL_CONTRIB_HEADERS})

  if(BUILD_GUICHAN_OPENGL_SHARED)
    set(GUICHAN_OPENGL_LIBRARY_TYPE SHARED)
  else(BUILD_GUICHAN_OPENGL_SHARED)
    set(GUICHAN_OPENGL_LIBRARY_TYPE STATIC)
  endif(BUILD_GUICHAN_OPENGL_SHARED)
  add_library(
    ${PROJECT_NAME}_opengl
    ${GUICHAN_OPENGL_LIBRARY_TYPE} ${GUICHAN_OPENGL_HEADER}
    ${GUICHAN_OPENGL_HEADERS} ${GUICHAN_OPENGL_CONTRIB_HEADERS}
    ${GUICHAN_OPENGL_SRC})
  if(WIN32)
    target_link_libraries(${PROJECT_NAME}_opengl ${OPENGL_LIBRARY}
                          ${PROJECT_NAME})
  endif(WIN32)
  add_custom_target(lib DEPENDS ${PROJECT_NAME}_opengl) # Create symlink
  set_target_properties(
    ${PROJECT_NAME}_opengl
    PROPERTIES VERSION ${${PROJECT_NAME}_VERSION}
               SOVERSION ${${PROJECT_NAME}_SOVERSION}
               CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared
                                     # libraries without conflict
               OUTPUT_NAME ${PROJECT_NAME}_opengl) # Avoid conflicts between
                                                   # library and binary target
                                                   # names
  install(
    TARGETS ${PROJECT_NAME}_opengl
    DESTINATION lib
    PERMISSIONS
      OWNER_READ
      OWNER_WRITE
      OWNER_EXECUTE
      GROUP_READ
      GROUP_EXECUTE
      WORLD_READ
      WORLD_EXECUTE)
  install(FILES ${GUICHAN_OPENGL_HEADER} DESTINATION include/guichan/)
  install(FILES ${GUICHAN_OPENGL_HEADERS} DESTINATION include/guichan/opengl/)
  install(FILES ${GUICHAN_OPENGL_CONTRIB_HEADERS}
          DESTINATION include/guichan/contrib/opengl/)
endif(ENABLE_OPENGL AND OPENGL_FOUND)

# The Guichan SDL extension library
option(ENABLE_SDL "Enable the Guichan SDL extension" ON)
option(BUILD_GUICHAN_SDL_SHARED
       "Build the Guichan SDL extension library as a shared library." ON)
find_package(SDL)
find_package(SDL_image)
if(ENABLE_SDL
   AND SDL_FOUND
   AND SDLIMAGE_FOUND)

  include_directories(${SDL_INCLUDE_DIR})

  # The Guichan SDL extension source
  file(GLOB GUICHAN_SDL_HEADER include/guichan/sdl.hpp)
  file(GLOB GUICHAN_SDL_HEADERS include/guichan/sdl/*.hpp)
  file(GLOB GUICHAN_SDL_CONTRIB_HEADERS include/guichan/contrib/sdl/*.hpp)
  file(GLOB GUICHAN_SDL_SRC src/sdl/*.cpp)

  # Grouping of the source for nicer display in IDEs such as Visual Studio
  source_group(src/guichan FILES ${GUICHAN_SDL_HEADER})
  source_group(src/guichan/sdl FILES ${GUICHAN_SDL_HEADERS} ${GUICHAN_SDL_SRC})
  source_group(src/guichan/contrib/sdl FILES ${GUICHAN_SDL_CONTRIB_HEADERS})

  if(BUILD_GUICHAN_SDL_SHARED)
    set(GUICHAN_SDL_LIBRARY_TYPE SHARED)
  else(BUILD_GUICHAN_SDL_SHARED)
    set(GUICHAN_SDL_LIBRARY_TYPE STATIC)
  endif(BUILD_GUICHAN_SDL_SHARED)
  add_library(
    ${PROJECT_NAME}_sdl
    ${GUICHAN_SDL_LIBRARY_TYPE} ${GUICHAN_SDL_HEADER} ${GUICHAN_SDL_HEADERS}
    ${GUICHAN_SDL_CONTRIB_HEADERS} ${GUICHAN_SDL_SRC})
  if(WIN32)
    if(MINGW)
      target_link_libraries(
        ${PROJECT_NAME}_sdl ${MINGW32_LIBRARY} ${SDL_LIBRARY}
        ${SDLIMAGE_LIBRARY} SDLmain ${PROJECT_NAME})
    else(MINGW)
      target_link_libraries(${PROJECT_NAME}_sdl ${SDL_LIBRARY}
                            ${SDLIMAGE_LIBRARY} SDLmain ${PROJECT_NAME})
    endif(MINGW)
  endif(WIN32)
  add_custom_target(lib DEPENDS ${PROJECT_NAME}_sdl) # Create symlink
  set_target_properties(
    ${PROJECT_NAME}_sdl
    PROPERTIES VERSION ${${PROJECT_NAME}_VERSION}
               SOVERSION ${${PROJECT_NAME}_SOVERSION}
               CLEAN_DIRECT_OUTPUT 1 # Allow creating static and shared
                                     # libraries without conflict
               OUTPUT_NAME ${PROJECT_NAME}_sdl) # Avoid conflicts between
                                                # library and binary target
                                                # names
  install(
    TARGETS ${PROJECT_NAME}_sdl
    DESTINATION lib
    PERMISSIONS
      OWNER_READ
      OWNER_WRITE
      OWNER_EXECUTE
      GROUP_READ
      GROUP_EXECUTE
      WORLD_READ
      WORLD_EXECUTE)
  install(FILES ${GUICHAN_SDL_HEADER} DESTINATION include/guichan/)
  install(FILES ${GUICHAN_SDL_HEADERS} DESTINATION include/guichan/sdl/)
  install(FILES ${GUICHAN_SDL_CONTRIB_HEADERS}
          DESTINATION include/guichan/contrib/sdl/)
endif(
  ENABLE_SDL
  AND SDL_FOUND
  AND SDLIMAGE_FOUND)

# Make uninstall target
# configure_file(
#   "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#   "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
# add_custom_target(uninstall "${CMAKE_COMMAND}" -P
#                             "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

message(STATUS "Guichan has been built, hooray!")
# message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
# message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
# message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
# message(STATUS "${PROJECT_NAME}_DEPENDS = \"${${PROJECT_NAME}_DEPENDS}\"")
# message(STATUS "BUILD_WITH = \"${BUILD_WITH}\"")
# message(STATUS "INSTALL_DOC = ${INSTALL_DOC}")
# message(STATUS "Change a value with: cmake -D<Variable>=<Value>")
# message(
#   STATUS
#     "-------------------------------------------------------------------------------"
# )
# message(STATUS)

# # force some variables that could be defined in the command line to be written
# to cache
# set(BUILD_SHARED_LIBS
#     "${BUILD_SHARED_LIBS}"
#     CACHE BOOL "Set to OFF to build static libraries" FORCE)
# set(CMAKE_INSTALL_PREFIX
#     "${CMAKE_INSTALL_PREFIX}"
#     CACHE PATH "Where to install ${PROJECT_NAME}" FORCE)
# set(CMAKE_BUILD_TYPE
#     "${CMAKE_BUILD_TYPE}"
#     CACHE
#       STRING
#       "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
#       FORCE)
# set(CMAKE_MODULE_PATH
#     "${CMAKE_MODULE_PATH}"
#     CACHE PATH "Path to custom CMake Modules" FORCE)
# set(INSTALL_DOC
#     "${INSTALL_DOC}"
#     CACHE BOOL "Set to OFF to skip build/install Documentation" FORCE)
# # export build settings
# include(CMakeExportBuildSettings)
# cmake_export_build_settings("${PROJECT_NAME}BuildSettings.cmake")
# # export library dependencies (keep this as the last line in the file)
# export_library_dependencies("${PROJECT_NAME}LibDeps.cmake")

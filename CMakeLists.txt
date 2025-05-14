cmake_minimum_required(VERSION 3.12...3.30)

# Path to local modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

project(guichan VERSION 0.9.0 LANGUAGES CXX)

# We're maintaining binary compatibility within minor releases at the moment
set(PROJECT_SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

# Set up pkg-config variables
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(libdir ${CMAKE_INSTALL_PREFIX}/lib)
set(includedir ${CMAKE_INSTALL_PREFIX}/include)

add_compile_definitions(GUICHAN_BUILD GUICHAN_EXTENSION_BUILD)

# Find all dependencies first to check availability
find_package(Allegro)
find_package(OpenGL)
find_package(SDL)
find_package(SDL_image)

# Set default values for options
set(SDL_DEFAULT OFF)
if(SDL_FOUND AND SDLIMAGE_FOUND)
  set(SDL_DEFAULT ON)
endif()

option(ENABLE_ALLEGRO "Enable the Guichan Allegro extension" ${ALLEGRO_FOUND})
option(ENABLE_OPENGL "Enable the Guichan OpenGL extension" ${OPENGL_FOUND})
option(ENABLE_SDL "Enable the Guichan SDL extension" ${SDL_DEFAULT})

option(BUILD_GUICHAN_SHARED
       "Build the Guichan core library as a shared library." ON)

option(BUILD_GUICHAN_ALLEGRO_SHARED
       "Build the Guichan Allegro extension library as a shared library." ON)

option(BUILD_GUICHAN_OPENGL_SHARED
       "Build the Guichan OpenGL extension library as a shared library." ON)

option(BUILD_GUICHAN_SDL_SHARED
       "Build the Guichan SDL extension library as a shared library." ON)

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

if(BUILD_GUICHAN_SHARED)
  set(GUICHAN_LIBRARY_TYPE SHARED)
else()
  set(GUICHAN_LIBRARY_TYPE STATIC)
endif()
add_library(
  ${PROJECT_NAME}
  ${GUICHAN_LIBRARY_TYPE}
  ${GUICHAN_HEADER}
  ${GUICHAN_HEADERS}
  ${GUICHAN_WIDGET_HEADERS}
  ${GUICHAN_CONTRIB_WIDGET_HEADERS}
  ${GUICHAN_SRC}
  ${GUICHAN_WIDGET_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_SOVERSION})
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include)
install(FILES ${GUICHAN_HEADER} DESTINATION include/)
install(FILES ${GUICHAN_HEADERS} DESTINATION include/guichan/)
install(FILES ${GUICHAN_WIDGET_HEADERS} DESTINATION include/guichan/widgets/)
install(FILES ${GUICHAN_CONTRIB_WIDGET_HEADERS}
        DESTINATION include/guichan/contrib/widgets/)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/guichan.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/guichan.pc
  @ONLY)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/guichan.pc
  DESTINATION lib/pkgconfig)

# The Guichan Allegro extension library
if(ENABLE_ALLEGRO)
  if(NOT ALLEGRO_FOUND)
    message(FATAL_ERROR "Allegro extension was enabled but Allegro library was not found.")
  endif()

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
  else()
    set(GUICHAN_ALLEGRO_LIBRARY_TYPE STATIC)
  endif()
  add_library(
    ${PROJECT_NAME}_allegro
    ${GUICHAN_ALLEGRO_LIBRARY_TYPE} ${GUICHAN_ALLEGRO_HEADER}
    ${GUICHAN_ALLEGRO_HEADERS} ${GUICHAN_ALLEGRO_CONTRIB_HEADERS}
    ${GUICHAN_ALLEGRO_SRC})

  target_include_directories(${PROJECT_NAME}_allegro PRIVATE ${ALLEGRO_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME}_allegro PRIVATE ${PROJECT_NAME})

  if(WIN32)
    target_link_libraries(${PROJECT_NAME}_allegro PRIVATE ${ALLEG_LIBRARY})
  endif()

  set_target_properties(
    ${PROJECT_NAME}_allegro
    PROPERTIES VERSION ${PROJECT_VERSION}
               SOVERSION ${PROJECT_SOVERSION})
  install(
    TARGETS ${PROJECT_NAME}_allegro
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)
  install(FILES ${GUICHAN_ALLEGRO_HEADER} DESTINATION include/guichan/)
  install(FILES ${GUICHAN_ALLEGRO_HEADERS} DESTINATION include/guichan/allegro/)
  install(FILES ${GUICHAN_ALLEGRO_CONTRIB_HEADERS}
          DESTINATION include/guichan/contrib/allegro/)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/guichan_allegro.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/guichan_allegro.pc
    @ONLY)

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/guichan_allegro.pc
    DESTINATION lib/pkgconfig)
endif()

# The Guichan OpenGL extension library
if(ENABLE_OPENGL)
  if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL extension was enabled but OpenGL library was not found.")
  endif()

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
  else()
    set(GUICHAN_OPENGL_LIBRARY_TYPE STATIC)
  endif()
  add_library(
    ${PROJECT_NAME}_opengl
    ${GUICHAN_OPENGL_LIBRARY_TYPE} ${GUICHAN_OPENGL_HEADER}
    ${GUICHAN_OPENGL_HEADERS} ${GUICHAN_OPENGL_CONTRIB_HEADERS}
    ${GUICHAN_OPENGL_SRC})

  target_include_directories(${PROJECT_NAME}_opengl PRIVATE ${OPENGL_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME}_opengl PRIVATE ${PROJECT_NAME})

  if(WIN32)
    target_link_libraries(${PROJECT_NAME}_opengl PRIVATE ${OPENGL_LIBRARY})
  endif()

  set_target_properties(
    ${PROJECT_NAME}_opengl
    PROPERTIES VERSION ${PROJECT_VERSION}
               SOVERSION ${PROJECT_SOVERSION})
  install(
    TARGETS ${PROJECT_NAME}_opengl
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)
  install(FILES ${GUICHAN_OPENGL_HEADER} DESTINATION include/guichan/)
  install(FILES ${GUICHAN_OPENGL_HEADERS} DESTINATION include/guichan/opengl/)
  install(FILES ${GUICHAN_OPENGL_CONTRIB_HEADERS}
          DESTINATION include/guichan/contrib/opengl/)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/guichan_opengl.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/guichan_opengl.pc
    @ONLY)

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/guichan_opengl.pc
    DESTINATION lib/pkgconfig)
endif()

# The Guichan SDL extension library
if(ENABLE_SDL)
  if(NOT SDL_FOUND OR NOT SDLIMAGE_FOUND)
    message(FATAL_ERROR "SDL extension was enabled but SDL or SDL_image libraries were not found.")
  endif()

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
  else()
    set(GUICHAN_SDL_LIBRARY_TYPE STATIC)
  endif()
  add_library(
    ${PROJECT_NAME}_sdl
    ${GUICHAN_SDL_LIBRARY_TYPE} ${GUICHAN_SDL_HEADER} ${GUICHAN_SDL_HEADERS}
    ${GUICHAN_SDL_CONTRIB_HEADERS} ${GUICHAN_SDL_SRC})

  target_include_directories(${PROJECT_NAME}_sdl PRIVATE ${SDL_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME}_sdl PRIVATE ${PROJECT_NAME})

  if(WIN32)
    if(MINGW)
      target_link_libraries(
        ${PROJECT_NAME}_sdl PRIVATE ${MINGW32_LIBRARY} ${SDL_LIBRARY}
        ${SDLIMAGE_LIBRARY} SDLmain)
    else()
      target_link_libraries(${PROJECT_NAME}_sdl PRIVATE ${SDL_LIBRARY}
                            ${SDLIMAGE_LIBRARY} SDLmain)
    endif()
  endif()

  set_target_properties(
    ${PROJECT_NAME}_sdl
    PROPERTIES VERSION ${PROJECT_VERSION}
               SOVERSION ${PROJECT_SOVERSION})
  install(
    TARGETS ${PROJECT_NAME}_sdl
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)
  install(FILES ${GUICHAN_SDL_HEADER} DESTINATION include/guichan/)
  install(FILES ${GUICHAN_SDL_HEADERS} DESTINATION include/guichan/sdl/)
  install(FILES ${GUICHAN_SDL_CONTRIB_HEADERS}
          DESTINATION include/guichan/contrib/sdl/)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/guichan_sdl.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/guichan_sdl.pc
    @ONLY)

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/guichan_sdl.pc
    DESTINATION lib/pkgconfig)
endif()

# Export targets for downstream projects
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION lib/cmake/${PROJECT_NAME})

message(STATUS "Guichan ${PROJECT_VERSION} has been configured successfully!")
message(STATUS "Build configuration:")
message(STATUS "  Install prefix:   ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Build type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "  Extensions:")
message(STATUS "    Allegro:        ${ENABLE_ALLEGRO}")
message(STATUS "    OpenGL:         ${ENABLE_OPENGL}")
message(STATUS "    SDL:            ${ENABLE_SDL}")
message(STATUS "")
message(STATUS "  Library types:")
message(STATUS "    Core library:   ${GUICHAN_LIBRARY_TYPE}")
if(ENABLE_ALLEGRO AND ALLEGRO_FOUND)
  message(STATUS "    Allegro:        ${GUICHAN_ALLEGRO_LIBRARY_TYPE}")
endif()
if(ENABLE_OPENGL AND OPENGL_FOUND)
  message(STATUS "    OpenGL:         ${GUICHAN_OPENGL_LIBRARY_TYPE}")
endif()
if(ENABLE_SDL AND SDL_FOUND AND SDLIMAGE_FOUND)
  message(STATUS "    SDL:            ${GUICHAN_SDL_LIBRARY_TYPE}")
endif()
message(STATUS "")

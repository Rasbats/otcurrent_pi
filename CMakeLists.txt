# ~~~
# Author:      Rasbats aka. Mike Rossiter
# Copyright:
# License:     wxWidgets License
# ~~~

# -------- Cmake setup ---------
#
cmake_minimum_required(VERSION 3.5.1)

cmake_policy(SET CMP0042 NEW)

if (POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif ()

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)

option(USE_GL "Enable OpenGL support" OFF)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

# Locations where cmake looks for cmake modules.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build ${CMAKE_SOURCE_DIR}/cmake)

if (WIN32)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/buildwin)
endif ()
message(STATUS "Cmake version: ${CMAKE_VERSION}.")


# -------- Build setup (mostly in PluginCompiler) --------
#
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
message(STATUS "Building: ${BUILD_TYPE}")


# -------- Options ----------

set(OCPN_TEST_REPO
    "opencpn/otcurrent-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "opencpn/otcurrent-beta"
    CACHE STRING 
    "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "opencpn/otcurrent-prod"
    CACHE STRING 
    "Default repository for tagged builds not matching 'beta'"
)
option(OTCURRENT_USE_SVG "Use SVG graphics" ON)


# -------  Plugin setup --------
#
project(otcurrent_pi VERSION 2.8.0)

set(DISPLAY_NAME otcurrent)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME otcurrent) # As of GetCommonName() in plugin API
set(CPACK_PACKAGE_CONTACT "Mike Rossiter")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Predict future tidal currents")
set(CPACK_PACKAGE_DESCRIPTION [=[
Allows predictions of future tidal currents, based on the harmonics data in
OpenCPN.
]=])

set(PKG_RELEASE "1")
set(PKG_AUTHOR "Mike Rossiter")


SET(SRC
        src/bbox.cpp
        src/bbox.h		
        src/otcurrent_pi.h
        src/otcurrent_pi.cpp
        src/otcurrentOverlayFactory.cpp
        src/otcurrentOverlayFactory.h
        src/otcurrentUIDialogBase.cpp
        src/otcurrentUIDialogBase.h
        src/otcurrentUIDialog.cpp
        src/otcurrentUIDialog.h
        src/icons.h
        src/icons.cpp
        src/tcmgr.cpp
        src/timectrl.cpp
        src/timectrl.h
        src/tcmgr.h
        src/tinystr.h
	    src/tinyxml.h
    )
 
macro(SetupOpenGL)
  find_package(OpenGL)
  if (OPENGL_FOUND)
    # Linking to OPENGL::GL should make it, but fails on Windows.
    target_link_libraries(${PACKAGE_NAME} ${OPENGL_LIBRARIES})
    include_directories(${OPENGL_INCLUDE_DIR})
  endif ()
  if (APPLE)
    # This is certainly neither stable nor portable, but I just cannot
    # figure out how to handle it properly. FindOpenGL should
    # actually find this directory, but doesn't (:
    string(CONCAT _glinclude
       "/Applications/Xcode-*app/Contents/Developer/Platforms"
       "/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System"
       "/Library/Frameworks/OpenGL.framework/Versions/A/Headers"
    )
    file(GLOB _glinclude ${_glinclude})
    if (EXISTS "${_glinclude}")
      include_directories("${_glinclude}")
    else ()
      message(WARNING "Cannot find gl.h, using bundled copy")
      include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/GL")
    endif ()
  endif ()
endmacro()    

# -------- Setup completed, build the plugin --------
#
add_library(${CMAKE_PROJECT_NAME} SHARED ${SRC})
include_directories(BEFORE ${CMAKE_BINARY_DIR}/include)

add_subdirectory("libs/api-16")
target_link_libraries(${CMAKE_PROJECT_NAME} ocpn::api)

set(VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(PACKAGE_NAME ${CMAKE_PROJECT_NAME})

# Create the minimim ocpn version id string like ov50
string(REGEX REPLACE "\([0-9]\)\.\([0-9]\).*" "ov\\1\\2"
       OCPN_MIN_VERSION ${API_OCPN_MIN_VERSION})

if (OTCURRENT_USE_SVG)
  add_definitions(-DOTCURRENT_USE_SVG)
endif ()

# Set up targets. Targets sets up a recursive call with BUILD_TYPE set to
# 'flatpak', 'pkg' or 'tarball'. The initial call without BUILD_TYPE ends
# here.
#
include(Targets)
create_targets(
  ${PROJECT_SOURCE_DIR}/flatpak/org.opencpn.OpenCPN.Plugin.otcurrent.yaml
)
if ("${BUILD_TYPE}" STREQUAL "")
  return ()
endif ()

if (NOT ${BUILD_TYPE} STREQUAL "flatpak")
  # Build package as required (flatpak already dealt with).
  #
  include(PluginCompiler)
  include(PluginLibs)
  include(PluginLocalization)
  include(PluginInstall)
  include(PluginPackage)

  add_subdirectory("libs/jsoncpp")
  target_link_libraries(${PACKAGE_NAME} ocpn::jsoncpp)
  
  add_subdirectory("libs/tinyxml")
  target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)
  
  add_subdirectory("libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)
  
  setupOpenGL()
endif ()

configure_file(
  ${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/include//config.h
  @ONLY
)

configure_file(
  # The cloudsmith upload script
  ${CMAKE_SOURCE_DIR}/ci/upload.sh.in ${CMAKE_BINARY_DIR}/upload.sh
  @ONLY
)
configure_file(
  # The cloudsmith upload script, windows bat file.
  ${CMAKE_SOURCE_DIR}/ci/upload.bat.in ${CMAKE_BINARY_DIR}/upload.bat
  @ONLY
)


set(checksum "@checksum@")
configure_file(
  # The XML metadata file
  ${CMAKE_SOURCE_DIR}/otcurrent-plugin.xml.in
  ${CMAKE_BINARY_DIR}/${pkg_displayname}.xml.in
  @ONLY
)

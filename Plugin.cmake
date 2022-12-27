# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.


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

#
#
# -------  Plugin setup --------
#
set(PKG_NAME otcurrent_pi)
set(PKG_VERSION  4.0.0)
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME otcurrent)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME otcurrent) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "Predict future tidal currents")
set(PKG_DESCRIPTION [=[
Allows predictions of future tidal currents, based on the harmonics data in
OpenCPN.
]=])

set(PKG_AUTHOR "Mike Rossiter")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/Rasbats/otcurrent_pi)
set(PKG_INFO_URL https://opencpn.org/OpenCPN/plugins/otcurrent.html)

set(SRC	
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
        src/tcmgr.h
        src/timectrl.cpp
        src/timectrl.h
	    src/android/wx/datectrl.h
	    src/android/wx/timectrl.h
	    src/gl_private.h
	    src/pidc.cpp
	    src/pidc.h
	    src/IDX_entry.cpp
	    src/IDX_entry.h
	    src/logger.cpp
	    src/logger.h
	    src/Station_Data.cpp
	    src/Station_Data.h
	    src/TC_Error_Code.h
	    src/TCDataFactory.cpp
	    src/TCDataFactory.h
	    src/TCDataSource.cpp
	    src/TCDataSource.h
	    src/TCDS_Ascii_Harmonic.cpp
	    src/TCDS_Ascii_Harmonic.h
	    src/TCDS_Binary_Harmonic.cpp
	    src/TCDS_Binary_Harmonic.h
	    src/georef.cpp
	    src/georef.h
	    src/GL/gl.h
	    src/GL/glext.h	    
)

set(PKG_API_LIB api-16)  #  A directory in libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.
  if (OTCURRENT_USE_SVG)
    target_compile_definitions(${PACKAGE_NAME} PUBLIC OTCURRENT_USE_SVG)
  endif ()

  add_definitions(-DocpnUSE_GL)

  if (QT_ANDROID)
    add_definitions(-DUSE_ANDROID_GLES2)
  endif ()
  
endmacro ()

macro(add_plugin_libraries)
  # Add libraries required by this plugin
  add_subdirectory("libs/tinyxml")
  target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

  add_subdirectory("libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

  add_subdirectory("libs/plugingl")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)
    
endmacro ()

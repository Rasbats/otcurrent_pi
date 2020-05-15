##---------------------------------------------------------------------------
## Author:      Dave Register
##***************************************************************************
## *   Copyright (C) 2010 by David S. Register                               *
## *   This program is free software; you can redistribute it and/or modify  *
## *   it under the terms of the GNU General Public License as published by  *
## *   the Free Software Foundation; either version 2 of the License, or     *
## *   (at your option) any later version.                                   *
## *                                                                         *
## *   This program is distributed in the hope that it will be useful,       *
## *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
## *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
## *   GNU General Public License for more details.                          *
## *                                                                         *
## *   You should have received a copy of the GNU General Public License     *
## *   along with this program; if not, write to the                         *
## *   Free Software Foundation, Inc.,                                       *
## *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
## ***************************************************************************

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(otcurrent_pi)

SET(PACKAGE_NAME otcurrent_pi)
SET(VERBOSE_NAME OTCurrent)
SET(TITLE_NAME OTCurrent)
SET(CPACK_PACKAGE_CONTACT "Mike Rossiter")

SET(VERSION_MAJOR "2")
SET(VERSION_MINOR "6")

SET(VERSION_DATE "2020-05-13")
SET(OCPN_MIN_VERSION "ov50")

set (CMAKE_CXX_STANDARD 11)

#SET(CMAKE_BUILD_TYPE Debug)
OPTION(OTCURRENT_USE_SVG "Use SVG graphics" ON)
# For OpenCPN version 5.x, NOT 5.0 ON. For version 5.0 OFF
OPTION(MANAGED_PLUGIN "Use managed plugin" OFF)

INCLUDE("cmake/PluginConfigure.cmake")

IF(OTCURRENT_USE_SVG)
  ADD_DEFINITIONS(-DOTCURRENT_USE_SVG)
ENDIF(OTCURRENT_USE_SVG)


IF(MANAGED_PLUGIN)
ADD_DEFINITIONS(-DMANAGED_PLUGIN)
ENDIF(MANAGED_PLUGIN)

SET(SRC_otcurrent
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


ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_otcurrent})

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")

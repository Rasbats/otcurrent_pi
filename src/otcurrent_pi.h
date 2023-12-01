/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  otcurrent Plugin
 * Author:   David Register, Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#ifndef _otcurrentPI_H_
#define _otcurrentPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include "ocpn_plugin.h"
#include "otcurrentOverlayFactory.h"
#include "otcurrentUIDialog.h"


#include <wx/window.h>

extern wxString myVColour[5]; 

#include "config.h"

class piDC;

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define otcurrent_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class otcurrent_pi : public opencpn_plugin_116
{
public:
      otcurrent_pi(void *ppimgr);
      ~otcurrent_pi(void);

//    The required PlugIn Methods
      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();

//    The override PlugIn Methods
      bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
	  bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
      void SetCursorLatLon(double lat, double lon);
      void SendTimelineMessage(wxDateTime time);
      void SetDefaults(void);
      int  GetToolbarToolCount(void);
      void ShowPreferencesDialog( wxWindow* parent );
      void OnToolbarToolCallback(int id);

// Other public methods
      void SetotcurrentDialogX    (int x){ m_otcurrent_dialog_x = x;};
      void SetotcurrentDialogY    (int x){ m_otcurrent_dialog_y = x;}
      void SetotcurrentDialogSizeX(int x){ m_otcurrent_dialog_sx = x;}
      void SetotcurrentDialogSizeY(int x){ m_otcurrent_dialog_sy = x;}
      void SetColorScheme(PI_ColorScheme cs);

      void OnotcurrentDialogClose();

      bool GetCopyRate() { return  m_bCopyUseRate; }
      bool GetCopyDirection() { return  m_bCopyUseDirection; }
	  bool GetCopyResolution() { return  m_bCopyUseHighRes; }
	  bool GetCopyColour() { return m_botcurrentUseHiDef ; }

	  wxString GetFolderSelected() {return m_CopyFolderSelected;}
	  int      GetIntervalSelected() {return m_CopyIntervalSelected;}
  
      otcurrentOverlayFactory *GetotcurrentOverlayFactory(){ return m_potcurrentOverlayFactory; }
      wxString          m_CopyFolderSelected;
	  int               m_CopyIntervalSelected;
      int m_otcurrent_dialog_x, m_otcurrent_dialog_y;
      int m_otcurrent_dialog_sx, m_otcurrent_dialog_sy;


private:
      bool LoadConfig(void);
      bool SaveConfig(void);

      wxFileConfig     *m_pconfig;
      wxWindow         *m_parent_window;

      otcurrentUIDialog     *m_potcurrentDialog;
      otcurrentOverlayFactory *m_potcurrentOverlayFactory;

      int              m_display_width, m_display_height;
      int              m_leftclick_tool_id;


      // preference data
      bool              m_botcurrentUseHiDef;
      bool              m_botcurrentUseGradualColors;
	  bool              m_bCopyUseRate;
      bool              m_bCopyUseDirection;
	  bool				m_bCopyUseHighRes;

	  
	  

      int              m_bTimeZone;
     
      int              m_bStartOptions;
      wxString         m_RequestConfig;
      wxString         *pTC_Dir;
      
      bool             m_botcurrentShowIcon;

      int              m_height;

      bool			   m_bShowotcurrent;
	  wxBitmap		   m_panelBitmap;
	

};

//----------------------------------------------------------------------------------------
// Prefrence dialog definition
//----------------------------------------------------------------------------------------

class otcurrentPreferencesDialog : public otcurrentPreferencesDialogBase
{
public:
		
	otcurrentPreferencesDialog( wxWindow *pparent)
    : otcurrentPreferencesDialogBase(pparent) {}
    ~otcurrentPreferencesDialog() {}

private:
    
};
#endif

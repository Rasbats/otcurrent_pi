/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OTCurrent Plugin
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


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/fileconf.h>
#include <wx/stdpaths.h>

#include "otcurrent_pi.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new OTCurrent_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//    OTCurrent PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"


//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

OTCurrent_pi::OTCurrent_pi(void *ppimgr)
      :opencpn_plugin_17(ppimgr)
{
      // Create the PlugIn icons
      initialize_images();
      m_bShowOTCurrent = false;
}

OTCurrent_pi::~OTCurrent_pi(void)
{
      delete _img_OTCurrent_pi;
      delete _img_OTCurrent;
}

int OTCurrent_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-OTCurrent_pi") );

      // Set some default private member parameters
      m_OTCurrent_dialog_x = 0;
      m_OTCurrent_dialog_y = 0;
      m_OTCurrent_dialog_sx = 200;
      m_OTCurrent_dialog_sy = 400;
      m_pOTCurrentDialog = NULL;
      m_pOTCurrentOverlayFactory = NULL;

      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();
	 
      //    And load the configuration items
      LoadConfig();

      // Get a pointer to the opencpn display canvas, to use as a parent for the OTCurrent dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    This PlugIn needs a toolbar icon, so request its insertion if enabled locally
      if(m_bOTCurrentShowIcon)
          m_leftclick_tool_id = InsertPlugInTool(_T(""), _img_OTCurrent, _img_OTCurrent, wxITEM_CHECK,
                                                 _("OTCurrent"), _T(""), NULL,
                                                 OTCurrent_TOOL_POSITION, 0, this);	  

      return (WANTS_OVERLAY_CALLBACK |
              WANTS_OPENGL_OVERLAY_CALLBACK |
              WANTS_CURSOR_LATLON       |
              WANTS_TOOLBAR_CALLBACK    |
              INSTALLS_TOOLBAR_TOOL     |
              WANTS_CONFIG              |
              WANTS_PREFERENCES         
              //WANTS_PLUGIN_MESSAGING
            );
}

bool OTCurrent_pi::DeInit(void)
{
    if(m_pOTCurrentDialog) {
        m_pOTCurrentDialog->Close();
        delete m_pOTCurrentDialog;
        m_pOTCurrentDialog = NULL;
    }

    delete m_pOTCurrentOverlayFactory;
    m_pOTCurrentOverlayFactory = NULL;

    return true;
}

int OTCurrent_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int OTCurrent_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int OTCurrent_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int OTCurrent_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *OTCurrent_pi::GetPlugInBitmap()
{
      return _img_OTCurrent_pi;
}

wxString OTCurrent_pi::GetCommonName()
{
      return _T("OTCurrent");
}


wxString OTCurrent_pi::GetShortDescription()
{
      return _("OTCurrent PlugIn for OpenCPN");
}


wxString OTCurrent_pi::GetLongDescription()
{
      return _("OTCurrent PlugIn for OpenCPN\nProvides an overlay of Tidal Current (Stream) Arrows.\n\n\
			   ");
}

void OTCurrent_pi::SetDefaults(void)
{
}


int OTCurrent_pi::GetToolbarToolCount(void)
{
      return 1;
}

void OTCurrent_pi::ShowPreferencesDialog( wxWindow* parent )
{
    OTCurrentPreferencesDialog *Pref = new OTCurrentPreferencesDialog(parent);

    Pref->m_cbUseRate->SetValue(m_bCopyUseRate);
    Pref->m_cbUseDirection->SetValue(m_bCopyUseDirection);
	Pref->m_cbFillColour->SetValue(m_bOTCurrentUseHiDef);

 if( Pref->ShowModal() == wxID_OK ) {

	 //bool copyFillColour = true;


     bool copyrate = Pref->m_cbUseRate->GetValue();
     bool copydirection = Pref->m_cbUseDirection->GetValue();
	 bool FillColour = Pref->m_cbFillColour->GetValue();

		 if (m_bOTCurrentUseHiDef != FillColour){		 
			 m_bOTCurrentUseHiDef = FillColour;
		 }
	 
        if( m_bCopyUseRate != copyrate || m_bCopyUseDirection != copydirection ) {
             m_bCopyUseRate = copyrate;
             m_bCopyUseDirection = copydirection;           
         }

		
         if(m_pOTCurrentDialog )
		 {
			 m_pOTCurrentDialog->OpenFile(true);
			 m_pOTCurrentDialog->m_bUseRate = m_bCopyUseRate;
			 m_pOTCurrentDialog->m_bUseDirection = m_bCopyUseDirection;	
			 m_pOTCurrentDialog->m_bUseFillColour = m_bOTCurrentUseHiDef;
		 }

		 if (m_pOTCurrentOverlayFactory)
		 {
			 m_pOTCurrentOverlayFactory->m_bShowRate = m_bCopyUseRate;
			 m_pOTCurrentOverlayFactory->m_bShowDirection = m_bCopyUseDirection;
			 m_pOTCurrentOverlayFactory->m_bShowFillColour = m_bOTCurrentUseHiDef;
		 }

         SaveConfig();
     }

	
}

void OTCurrent_pi::OnToolbarToolCallback(int id)
{
    if(!m_pOTCurrentDialog)
    {
		
        		
		m_pOTCurrentDialog = new OTCurrentUIDialog(m_parent_window, this);
        wxPoint p = wxPoint(m_OTCurrent_dialog_x, m_OTCurrent_dialog_y);
        m_pOTCurrentDialog->Move(0,0);        // workaround for gtk autocentre dialog behavior
        m_pOTCurrentDialog->Move(p);

        // Create the drawing factory
        m_pOTCurrentOverlayFactory = new OTCurrentOverlayFactory( *m_pOTCurrentDialog );
        m_pOTCurrentOverlayFactory->SetParentSize( m_display_width, m_display_height);
		
		
        
    }

   m_pOTCurrentDialog->OpenFile(true);
      // Qualify the OTCurrent dialog position
            bool b_reset_pos = false;

#ifdef __WXMSW__
        //  Support MultiMonitor setups which an allow negative window positions.
        //  If the requested window does not intersect any installed monitor,
        //  then default to simple primary monitor positioning.
            RECT frame_title_rect;
            frame_title_rect.left =   m_OTCurrent_dialog_x;
            frame_title_rect.top =    m_OTCurrent_dialog_y;
            frame_title_rect.right =  m_OTCurrent_dialog_x + m_OTCurrent_dialog_sx;
            frame_title_rect.bottom = m_OTCurrent_dialog_y + 30;


            if(NULL == MonitorFromRect(&frame_title_rect, MONITOR_DEFAULTTONULL))
                  b_reset_pos = true;
#else
       //    Make sure drag bar (title bar) of window on Client Area of screen, with a little slop...
            wxRect window_title_rect;                    // conservative estimate
            window_title_rect.x = m_OTCurrent_dialog_x;
            window_title_rect.y = m_OTCurrent_dialog_y;
            window_title_rect.width = m_OTCurrent_dialog_sx;
            window_title_rect.height = 30;

            wxRect ClientRect = wxGetClientDisplayRect();
            ClientRect.Deflate(60, 60);      // Prevent the new window from being too close to the edge
            if(!ClientRect.Intersects(window_title_rect))
                  b_reset_pos = true;

#endif

            if(b_reset_pos)
            {
                  m_OTCurrent_dialog_x = 20;
                  m_OTCurrent_dialog_y = 170;
                  m_OTCurrent_dialog_sx = 300;
                  m_OTCurrent_dialog_sy = 540;
            }

      //Toggle OTCurrent overlay display
      m_bShowOTCurrent = !m_bShowOTCurrent;

      //    Toggle dialog?
      if(m_bShowOTCurrent) {
          m_pOTCurrentDialog->Show();
      } else {
          m_pOTCurrentDialog->Hide();         
          }

      // Toggle is handled by the toolbar but we must keep plugin manager b_toggle updated
      // to actual status to ensure correct status upon toolbar rebuild
      SetToolbarItemState( m_leftclick_tool_id, m_bShowOTCurrent );
      RequestRefresh(m_parent_window); // refresh mainn window
}

void OTCurrent_pi::OnOTCurrentDialogClose()
{
    m_bShowOTCurrent = false;
    SetToolbarItemState( m_leftclick_tool_id, m_bShowOTCurrent );

    m_pOTCurrentDialog->Hide();
    //if(m_pOTCurrentDialog->pReq_Dialog) m_pOTCurrentDialog->pReq_Dialog->Hide();

    SaveConfig();

    RequestRefresh(m_parent_window); // refresh mainn window

}

bool OTCurrent_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    if(!m_pOTCurrentDialog ||
       !m_pOTCurrentDialog->IsShown() ||
       !m_pOTCurrentOverlayFactory)
        return false;

    m_pOTCurrentDialog->SetViewPort( vp );
    m_pOTCurrentOverlayFactory->RenderOTCurrentOverlay ( dc, vp );
    return true;
}

bool OTCurrent_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    if(!m_pOTCurrentDialog ||
       !m_pOTCurrentDialog->IsShown() ||
       !m_pOTCurrentOverlayFactory)
        return false;

    m_pOTCurrentDialog->SetViewPort( vp );
    m_pOTCurrentOverlayFactory->RenderGLOTCurrentOverlay ( pcontext, vp );
    return true;
}
void OTCurrent_pi::SetCursorLatLon(double lat, double lon)
{
    if(m_pOTCurrentDialog)
        m_pOTCurrentDialog->SetCursorLatLon(lat, lon);
}

bool OTCurrent_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T( "/PlugIns/OTCurrent" ) );

	m_bCopyUseRate = pConf->Read ( _T ( "OTCurrentUseRate" ),1);
    m_bCopyUseDirection = pConf->Read ( _T ( "OTCurrentUseDirection" ), 1);
	m_bOTCurrentUseHiDef = pConf->Read ( _T ( "OTCurrentUseFillColour" ), 1);

	

    m_OTCurrent_dialog_sx = pConf->Read ( _T ( "OTCurrentDialogSizeX" ), 300L );
    m_OTCurrent_dialog_sy = pConf->Read ( _T ( "OTCurrentDialogSizeY" ), 540L );
    m_OTCurrent_dialog_x =  pConf->Read ( _T ( "OTCurrentDialogPosX" ), 20L );
    m_OTCurrent_dialog_y =  pConf->Read ( _T ( "OTCurrentDialogPosY" ), 170L );


	
    return true;
}

bool OTCurrent_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T( "/PlugIns/OTCurrent" ) );
    pConf->Write ( _T ( "OTCurrentUseRate" ), m_bCopyUseRate );
    pConf->Write ( _T ( "OTCurrentUseDirection" ), m_bCopyUseDirection );
	pConf->Write ( _T ( "OTCurrentUseFillColour" ), m_bOTCurrentUseHiDef );

    pConf->Write ( _T ( "OTCurrentDialogSizeX" ),  m_OTCurrent_dialog_sx );
    pConf->Write ( _T ( "OTCurrentDialogSizeY" ),  m_OTCurrent_dialog_sy );
    pConf->Write ( _T ( "OTCurrentDialogPosX" ),   m_OTCurrent_dialog_x );
    pConf->Write ( _T ( "OTCurrentDialogPosY" ),   m_OTCurrent_dialog_y );

	
    return true;
}

void OTCurrent_pi::SetColorScheme(PI_ColorScheme cs)
{
    DimeWindow(m_pOTCurrentDialog);
}


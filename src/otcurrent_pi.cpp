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


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/fileconf.h>
#include <wx/stdpaths.h>

#include "otcurrent_pi.h"
#include "otcurrentUIDialogBase.h"
#include "otcurrentUIDialog.h"

wxString myVColour[] = {_T("rgb(127, 0, 255)"), _T("rgb(0, 166, 80)"),  _T("rgb(253, 184, 19)"),  _T("rgb(248, 128, 64)"),  _T("rgb(248, 0, 0)")};

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new otcurrent_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}



//---------------------------------------------------------------------------------------------------------
//
//    otcurrent PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"


//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

otcurrent_pi::otcurrent_pi(void *ppimgr)
      :opencpn_plugin_17(ppimgr)
{
      // Create the PlugIn icons
      initialize_images();
      m_bShowotcurrent = false;
	  
     

}

otcurrent_pi::~otcurrent_pi(void)
{
      delete _img_otcurrent_pi;
      delete _img_otcurrent;
}

int otcurrent_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-otcurrent_pi") );

      // Set some default private member parameters
      m_otcurrent_dialog_x = 0;
      m_otcurrent_dialog_y = 0;
      m_otcurrent_dialog_sx = 200;
      m_otcurrent_dialog_sy = 400;
      m_potcurrentDialog = NULL;
      m_potcurrentOverlayFactory = NULL;
      m_botcurrentShowIcon = true;

      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();
	 
      //    And load the configuration items
      LoadConfig();



      // Get a pointer to the opencpn display canvas, to use as a parent for the otcurrent dialog
      m_parent_window = GetOCPNCanvasWindow();

	

      //    This PlugIn needs a toolbar icon, so request its insertion if enabled locally
      if(m_botcurrentShowIcon)
          m_leftclick_tool_id = InsertPlugInTool(_T(""), _img_otcurrent, _img_otcurrent, wxITEM_CHECK,
                                                 _("otcurrent"), _T(""), NULL,
                                                 otcurrent_TOOL_POSITION, 0, this);	  

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

bool otcurrent_pi::DeInit(void)
{
    if(m_potcurrentDialog) {
        m_potcurrentDialog->Close();
        delete m_potcurrentDialog;
        m_potcurrentDialog = NULL;
    }

    delete m_potcurrentOverlayFactory;
    m_potcurrentOverlayFactory = NULL;

    return true;
}

int otcurrent_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int otcurrent_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int otcurrent_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int otcurrent_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *otcurrent_pi::GetPlugInBitmap()
{
      return _img_otcurrent_pi;
}

wxString otcurrent_pi::GetCommonName()
{
      return _T("otcurrent");
}


wxString otcurrent_pi::GetShortDescription()
{
      return _("otcurrent PlugIn for OpenCPN");
}


wxString otcurrent_pi::GetLongDescription()
{
      return _("otcurrent PlugIn for OpenCPN\nProvides an overlay of Tidal Current (Stream) Arrows.\n\n\
			   ");
}

void otcurrent_pi::SetDefaults(void)
{
}


int otcurrent_pi::GetToolbarToolCount(void)
{
      return 1;
}

void otcurrent_pi::ShowPreferencesDialog( wxWindow* parent )
{
    otcurrentPreferencesDialog *Pref = new otcurrentPreferencesDialog(parent);

    Pref->m_cbUseRate->SetValue(m_bCopyUseRate);
    Pref->m_cbUseDirection->SetValue(m_bCopyUseDirection);
	Pref->m_cbFillColour->SetValue(m_botcurrentUseHiDef);

	wxColour myC0 = wxColour(myVColour[0]);
	Pref->myColourPicker0->SetColour(myC0);
    
	wxColour myC1 = wxColour(myVColour[1]);
	Pref->myColourPicker1->SetColour(myC1);

	wxColour myC2 = wxColour(myVColour[2]);
	Pref->myColourPicker2->SetColour(myC2);

	wxColour myC3 = wxColour(myVColour[3]);
	Pref->myColourPicker3->SetColour(myC3);

	wxColour myC4 = wxColour(myVColour[4]);
	Pref->myColourPicker4->SetColour(myC4);


 if( Pref->ShowModal() == wxID_OK ) {

	 //bool copyFillColour = true;
    
	 myVColour[0] = Pref->myColourPicker0->GetColour().GetAsString();
	 myVColour[1] = Pref->myColourPicker1->GetColour().GetAsString();
	 myVColour[2] = Pref->myColourPicker2->GetColour().GetAsString();
	 myVColour[3] = Pref->myColourPicker3->GetColour().GetAsString();
	 myVColour[4] = Pref->myColourPicker4->GetColour().GetAsString();


     bool copyrate = Pref->m_cbUseRate->GetValue();
     bool copydirection = Pref->m_cbUseDirection->GetValue();
	 bool FillColour = Pref->m_cbFillColour->GetValue();

		 if (m_botcurrentUseHiDef != FillColour){		 
			 m_botcurrentUseHiDef = FillColour;
		 }
	 
        if( m_bCopyUseRate != copyrate || m_bCopyUseDirection != copydirection||  m_botcurrentUseHiDef != FillColour ) {
             m_bCopyUseRate = copyrate;
             m_bCopyUseDirection = copydirection;   
			 m_botcurrentUseHiDef = FillColour;
         }

		
         if(m_potcurrentDialog )
		 {	
			 m_potcurrentDialog->OpenFile(true);
			 m_potcurrentDialog->m_FolderSelected = m_CopyFolderSelected;
			 m_potcurrentDialog->m_IntervalSelected = m_CopyIntervalSelected;

			 m_potcurrentDialog->m_bUseRate = m_bCopyUseRate;
			 m_potcurrentDialog->m_bUseDirection = m_bCopyUseDirection;	
			 m_potcurrentDialog->m_bUseFillColour = m_botcurrentUseHiDef;

			 m_potcurrentDialog->myUseColour[0] = myVColour[0];
 			 m_potcurrentDialog->myUseColour[1] = myVColour[1];
 			 m_potcurrentDialog->myUseColour[2] = myVColour[2];
 			 m_potcurrentDialog->myUseColour[3] = myVColour[3];
 			 m_potcurrentDialog->myUseColour[4] = myVColour[4];			 
		 }

		 if (m_potcurrentOverlayFactory)
		 {			 
			 m_potcurrentOverlayFactory->m_bShowRate = m_bCopyUseRate;
			 m_potcurrentOverlayFactory->m_bShowDirection = m_bCopyUseDirection;
			 m_potcurrentOverlayFactory->m_bShowFillColour = m_botcurrentUseHiDef;
		 }

         SaveConfig();
		 
		 RequestRefresh(m_parent_window); // refresh main window
     }
	
}

void otcurrent_pi::OnToolbarToolCallback(int id)
{
    
	if(!m_potcurrentDialog)
    {
		       		
		m_potcurrentDialog = new otcurrentUIDialog(m_parent_window, this);
        wxPoint p = wxPoint(m_otcurrent_dialog_x, m_otcurrent_dialog_y);
        m_potcurrentDialog->Move(0,0);        // workaround for gtk autocentre dialog behavior
        m_potcurrentDialog->Move(p);

        // Create the drawing factory
        m_potcurrentOverlayFactory = new otcurrentOverlayFactory( *m_potcurrentDialog );
        m_potcurrentOverlayFactory->SetParentSize( m_display_width, m_display_height);		
        
    }

      // Qualify the otcurrent dialog position
            bool b_reset_pos = false;

#ifdef __WXMSW__
        //  Support MultiMonitor setups which an allow negative window positions.
        //  If the requested window does not intersect any installed monitor,
        //  then default to simple primary monitor positioning.
            RECT frame_title_rect;
            frame_title_rect.left =   m_otcurrent_dialog_x;
            frame_title_rect.top =    m_otcurrent_dialog_y;
            frame_title_rect.right =  m_otcurrent_dialog_x + m_otcurrent_dialog_sx;
            frame_title_rect.bottom = m_otcurrent_dialog_y + 30;


            if(NULL == MonitorFromRect(&frame_title_rect, MONITOR_DEFAULTTONULL))
                  b_reset_pos = true;
#else
       //    Make sure drag bar (title bar) of window on Client Area of screen, with a little slop...
            wxRect window_title_rect;                    // conservative estimate
            window_title_rect.x = m_otcurrent_dialog_x;
            window_title_rect.y = m_otcurrent_dialog_y;
            window_title_rect.width = m_otcurrent_dialog_sx;
            window_title_rect.height = 30;

            wxRect ClientRect = wxGetClientDisplayRect();
            ClientRect.Deflate(60, 60);      // Prevent the new window from being too close to the edge
            if(!ClientRect.Intersects(window_title_rect))
                  b_reset_pos = true;

#endif

            if(b_reset_pos)
            {
                  m_otcurrent_dialog_x = 20;
                  m_otcurrent_dialog_y = 170;
                  m_otcurrent_dialog_sx = 300;
                  m_otcurrent_dialog_sy = 540;
            }

      //Toggle otcurrent overlay display
      m_bShowotcurrent = !m_bShowotcurrent;

      //    Toggle dialog?
      if(m_bShowotcurrent) {
          m_potcurrentDialog->Show();
      } else {
          m_potcurrentDialog->Hide();         
          }

      // Toggle is handled by the toolbar but we must keep plugin manager b_toggle updated
      // to actual status to ensure correct status upon toolbar rebuild
      SetToolbarItemState( m_leftclick_tool_id, m_bShowotcurrent );
      RequestRefresh(m_parent_window); // refresh main window
}

void otcurrent_pi::OnotcurrentDialogClose()
{
    m_bShowotcurrent = false;
    SetToolbarItemState( m_leftclick_tool_id, m_bShowotcurrent );

    m_potcurrentDialog->Hide();

    SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window

}

bool otcurrent_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    if(!m_potcurrentDialog ||
       !m_potcurrentDialog->IsShown() ||
       !m_potcurrentOverlayFactory)
        return false;

    m_potcurrentDialog->SetViewPort( vp );
    m_potcurrentOverlayFactory->RenderotcurrentOverlay ( dc, vp );
    return true;
}

bool otcurrent_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    if(!m_potcurrentDialog ||
       !m_potcurrentDialog->IsShown() ||
       !m_potcurrentOverlayFactory)
        return false;

    m_potcurrentDialog->SetViewPort( vp );
    m_potcurrentOverlayFactory->RenderGLotcurrentOverlay ( pcontext, vp );
    return true;
}
void otcurrent_pi::SetCursorLatLon(double lat, double lon)
{
    if(m_potcurrentDialog)
        m_potcurrentDialog->SetCursorLatLon(lat, lon);
}

bool otcurrent_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T( "/PlugIns/otcurrent" ) );

	m_bCopyUseRate = pConf->Read ( _( "otcurrentUseRate" ),1);
    m_bCopyUseDirection = pConf->Read ( _( "otcurrentUseDirection" ), 1);
	m_botcurrentUseHiDef = pConf->Read ( _( "otcurrentUseFillColour" ), 1);

	m_CopyFolderSelected = pConf->Read ( _( "otcurrentFolder" ));
	if (m_CopyFolderSelected == wxEmptyString){

	  wxString g_SData_Locn = *GetpSharedDataLocation();

      // Establish location of Tide and Current data
      pTC_Dir = new wxString(_T("tcdata"));
      pTC_Dir->Prepend(g_SData_Locn);

	  m_CopyFolderSelected = *pTC_Dir;	  
	}

	m_CopyIntervalSelected = pConf->Read ( _T ( "otcurrentInterval"), 20L);

    m_otcurrent_dialog_sx = pConf->Read ( _( "otcurrentDialogSizeX" ), 300L );
    m_otcurrent_dialog_sy = pConf->Read ( _( "otcurrentDialogSizeY" ), 540L );
    m_otcurrent_dialog_x =  pConf->Read ( _( "otcurrentDialogPosX" ), 20L );
    m_otcurrent_dialog_y =  pConf->Read ( _( "otcurrentDialogPosY" ), 170L );
	
    pConf->Read( _T("VColour0"), &myVColour[0], myVColour[0] );
    pConf->Read( _T("VColour1"), &myVColour[1], myVColour[1] );
	pConf->Read( _T("VColour2"), &myVColour[2], myVColour[2] );
	pConf->Read( _T("VColour3"), &myVColour[3], myVColour[3] );
	pConf->Read( _T("VColour4"), &myVColour[4], myVColour[4] );
	
    return true;
}

bool otcurrent_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T( "/PlugIns/otcurrent" ) );
    pConf->Write ( _( "otcurrentUseRate" ), m_bCopyUseRate );
    pConf->Write ( _( "otcurrentUseDirection" ), m_bCopyUseDirection );
	pConf->Write ( _( "otcurrentUseFillColour" ), m_botcurrentUseHiDef );

	pConf->Write ( _( "otcurrentFolder" ), m_CopyFolderSelected); 
	pConf->Write ( _( "otcurrentInterval" ), m_CopyIntervalSelected);

    pConf->Write ( _( "otcurrentDialogSizeX" ),  m_otcurrent_dialog_sx );
    pConf->Write ( _( "otcurrentDialogSizeY" ),  m_otcurrent_dialog_sy );
    pConf->Write ( _( "otcurrentDialogPosX" ),   m_otcurrent_dialog_x );
    pConf->Write ( _( "otcurrentDialogPosY" ),   m_otcurrent_dialog_y );

	pConf->Write( _T("VColour0"), myVColour[0] );
	pConf->Write( _T("VColour1"), myVColour[1] );
	pConf->Write( _T("VColour2"), myVColour[2] );
	pConf->Write( _T("VColour3"), myVColour[3] );
	pConf->Write( _T("VColour4"), myVColour[4] );


    return true;
}

void otcurrent_pi::SetColorScheme(PI_ColorScheme cs)
{
    DimeWindow(m_potcurrentDialog);
}




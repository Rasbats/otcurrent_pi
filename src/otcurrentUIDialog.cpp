/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  otcurrent Object
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 *
 */
#include <wx/intl.h>
#include "wx/wx.h"
#include "wx/datetime.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/debug.h>
#include <wx/graphics.h>
#include <wx/stdpaths.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "otcurrent_pi.h"
#include "folder.xpm"
#include "icons.h"
#include <wx/arrimpl.cpp>

#ifdef __WXMSW__
#include <windows.h>
#endif
#include <memory.h> 

#include <wx/colordlg.h>
#include <wx/event.h>
#include "otcurrent_pi.h"

using namespace std;


#define FAIL(X) do { error = X; goto failed; } while(0)


//date/time in the desired time zone format
static wxString TToString( const wxDateTime date_time, const int time_zone )
{
    wxDateTime t( date_time );
    t.MakeFromTimezone( wxDateTime::UTC );
    if( t.IsDST() ) t.Subtract( wxTimeSpan( 1, 0, 0, 0 ) );
    switch( time_zone ) {
        case 0: return t.Format( _T(" %a %d-%b-%Y  %H:%M LOC"), wxDateTime::Local );
        case 1:
        default: return t.Format( _T(" %a %d-%b-%Y %H:%M  UTC"), wxDateTime::UTC );
    }
}

otcurrentUIDialog::otcurrentUIDialog(wxWindow *parent, otcurrent_pi *ppi, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: otcurrentUIDialogBase(parent, id, title, pos, size, style), m_ptcmgr(0), m_vp(0)
{
    pParent = parent;
    pPlugIn = ppi;

    wxFileConfig *pConf = GetOCPNConfigObject();

    if(pConf) {
        pConf->SetPath ( _T ( "/Plugins/otcurrent" ) );

		pConf->Read ( _T ( "otcurrentUseRate" ), &m_bUseRate, 1 );
        pConf->Read ( _T ( "otcurrentUseDirection" ), &m_bUseDirection, 1);
		pConf->Read(_T("otcurrentUseHighResolution"), &m_bUseHighRes, 1);
		pConf->Read ( _T ( "otcurrentUseFillColour" ), &m_bUseFillColour, 1);
		pConf->Read(_T("otcurrentUseScale"), &m_sUseScale, _T("1"));

		pConf->Read ( _T ( "otcurrentInterval" ), &m_IntervalSelected, 20L);
		pConf->Read ( _T ( "otcurrentFolder" ), &m_FolderSelected);

		pConf->Read( _T("VColour0"), &myVColour[0], myVColour[0] );
		pConf->Read( _T("VColour1"), &myVColour[1], myVColour[1] );
		pConf->Read( _T("VColour2"), &myVColour[2], myVColour[2] );
		pConf->Read( _T("VColour3"), &myVColour[3], myVColour[3] );
		pConf->Read( _T("VColour4"), &myVColour[4], myVColour[4] );
		
		myUseColour[0] = myVColour[0];
		myUseColour[1] = myVColour[1];
		myUseColour[2] = myVColour[2];
		myUseColour[3] = myVColour[3];
		myUseColour[4] = myVColour[4];
		


    }

    m_bpPrev->SetBitmap(wxBitmap( prev1 ), wxLEFT);
    m_bpNext->SetBitmap(wxBitmap( next1 ), wxRIGHT);
    m_bpNow->SetBitmap(*_img_Clock, wxLEFT);

    this->Connect( wxEVT_MOVE, wxMoveEventHandler( otcurrentUIDialog::OnMove ) );
    m_timePickerTime->Connect( wxEVT_TIME_CHANGED, wxDateEventHandler( otcurrentUIDialog::OnDateTimeChanged ), NULL, this );
    m_dtNow = wxDateTime::Now(); 
    MakeDateTimeLabel(m_dtNow);

	//wxMessageBox(m_FolderSelected);
	
    if (m_FolderSelected == wxEmptyString){
        
		wxString shareLocn;
		
		shareLocn = *GetpSharedDataLocation();		

        wxString g_SData_Locn = shareLocn;
        // Establish location of Tide and Current data
        pTC_Dir = new wxString(_T("tcdata"));
        pTC_Dir->Prepend(g_SData_Locn);
        m_FolderSelected = *pTC_Dir;

        m_dirPicker1->SetPath(m_FolderSelected);
        m_dirPicker1->GetTextCtrlValue();
    }
    else{
        m_dirPicker1->SetPath(m_FolderSelected);
    }

	//wxMessageBox(m_FolderSelected);
    LoadTCMFile();
	m_choice1->SetSelection(m_IntervalSelected);
	int i = m_choice1->GetSelection();
	wxString c = m_choice1->GetString(i);	
	double value;
	c.ToDouble(&value);
	m_dInterval = value;


	wxDateTime::TimeZone tz(wxDateTime::Local);
	long offset = tz.GetOffset(); // in sec from GMT0
	double doffset = (double)offset;
	double h = (long)(doffset / 3600);
	double m = (doffset/3600 - h) * 60;
	wxString tzs;
	tzs = wxString::Format(_T("UTC %+03.0f:%02.0f"), h, m);
	m_stTimeZone->SetLabel(tzs);

	DimeWindow( this );
	
}

void otcurrentUIDialog::LoadTCMFile()
{
    delete m_ptcmgr;
    wxString TCDir = m_FolderSelected;
    TCDir.Append(wxFileName::GetPathSeparator());

    wxLogMessage(_("Using Tide/Current data from:  ") + TCDir);

    wxString cache_locn = TCDir; 
    m_ptcmgr = new TCMgr(TCDir, cache_locn);
}


otcurrentUIDialog::~otcurrentUIDialog()
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(pConf) {
        pConf->SetPath ( _T ( "/Plugins/otcurrent" ) );

		pConf->Write ( _T ( "otcurrentUseRate" ), m_bUseRate );
		pConf->Write ( _T ( "otcurrentUseDirection" ), m_bUseDirection );
		pConf->Write(_T("otcurrentUseHighResolution"), m_bUseHighRes);
		pConf->Write ( _T ( "otcurrentUseFillColour" ), m_bUseFillColour );
		pConf->Write(_T("otcurrentUseScale"), m_sUseScale);

		pConf->Write( _T("VColour0"), myVColour[0] );
		pConf->Write( _T("VColour1"), myVColour[1] );
		pConf->Write( _T("VColour2"), myVColour[2] );
		pConf->Write( _T("VColour3"), myVColour[3] );
		pConf->Write( _T("VColour4"), myVColour[4] );

		int c = m_choice1->GetSelection();
		wxString myP = m_choice1->GetString(c);
		pConf->Write ( _T ( "otcurrentInterval" ), c ); 

		wxString myF = m_dirPicker1->GetPath();
		pConf->Write ( _T ( "otcurrentFolder" ), myF ); 

    }
    
    this->Disconnect( wxEVT_MOVE, wxMoveEventHandler( otcurrentUIDialog::OnMove ) );
    m_timePickerTime->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( otcurrentUIDialog::OnDateTimeChanged ), NULL, this );
    
    delete m_ptcmgr;
}

void otcurrentUIDialog::SetCursorLatLon( double lat, double lon )
{
    m_cursor_lon = lon;
    m_cursor_lat = lat;
}

void otcurrentUIDialog::SetViewPort( PlugIn_ViewPort *vp )
{
    if(m_vp == vp)  return;

    delete m_vp;
    m_vp = new PlugIn_ViewPort(*vp);
}

void otcurrentUIDialog::OnClose( wxCloseEvent& event )
{
    pPlugIn->OnotcurrentDialogClose();
}

void otcurrentUIDialog::OnMove( wxMoveEvent& event )
{
    //    Record the dialog position
    wxPoint p = GetPosition();
    pPlugIn->SetotcurrentDialogX( p.x );
    pPlugIn->SetotcurrentDialogY( p.y );

    event.Skip();
}

void otcurrentUIDialog::OnSize( wxSizeEvent& event )
{
    //    Record the dialog size
    wxSize p = event.GetSize();
    pPlugIn->SetotcurrentDialogSizeX( p.x );
    pPlugIn->SetotcurrentDialogSizeY( p.y );

    event.Skip();
}

void otcurrentUIDialog::OpenFile(bool newestFile)
{
	m_bUseRate = pPlugIn->GetCopyRate();
	m_bUseDirection = pPlugIn->GetCopyDirection();
	m_bUseHighRes = pPlugIn->GetCopyResolution();
	m_bUseFillColour = pPlugIn->GetCopyColour();
	m_sUseScale = pPlugIn->GetCopyScale();

	m_FolderSelected = pPlugIn->GetFolderSelected();
	m_IntervalSelected = pPlugIn->GetIntervalSelected();
    LoadTCMFile();
}

void otcurrentUIDialog::OnFolderSelChanged(wxFileDirPickerEvent& event)
{
	m_FolderSelected = m_dirPicker1->GetPath();
    LoadTCMFile();

	RequestRefresh(pParent);	
}

void otcurrentUIDialog::OnDateTimeChanged( wxDateEvent& event )
{	
	wxDateTime dm;
	wxDateTime tm;
	wxTimeSpan mySpan;

#ifndef __OCPN__ANDROID__
	 dm = m_datePickerDate->GetValue();
#else	
	wxString DateString = m_datePickerDate->GetValue();  
    dm.ParseDate(DateString);
#endif
    int h, m, s;
 
#ifndef __OCPN__ANDROID__
	 m_timePickerTime->GetTime( &h, &m, &s );
	 mySpan = wxTimeSpan(h, m, s);
#else	
	wxDateTime tm = m_timePickerTime->GetTimeCtrlValue();
	wxString ts = tm.FormatISOTime();
	wxString hs = ts.Mid(0, 2);
	wxString ms = ts.Mid(3, 2);
	wxString ss = ts.Mid(5, 2);
	mySpan = wxTimeSpan(wxAtoi(hs), wxAtoi(ms), wxAtoi(ss));
#endif
    
    dm.Add( mySpan );
    
    m_dtNow = dm;
    
    MakeDateTimeLabel(m_dtNow);
    
    RequestRefresh( pParent );
}

void otcurrentUIDialog::OnNow( wxCommandEvent& event ){
	
	m_dtNow = wxDateTime::Now();
	MakeDateTimeLabel(m_dtNow);

	RequestRefresh( pParent );

	onPrev = false;
	onNext = false;	
}

void otcurrentUIDialog::OnPrev( wxCommandEvent& event ){

	int i = m_choice1->GetSelection();
	wxString c = m_choice1->GetString(i);	
	double value;
    c.ToDouble(&value);
	m_dInterval = value;

	wxTimeSpan m_ts = wxTimeSpan::Minutes(m_dInterval) ;
	m_dtNow.Subtract(m_ts);
	MakeDateTimeLabel(m_dtNow);

	RequestRefresh( pParent );

}

void otcurrentUIDialog::OnNext( wxCommandEvent& event ){
	
    int i = m_choice1->GetSelection();
	wxString c = m_choice1->GetString(i);	

	double value;
	c.ToDouble(&value);
	m_dInterval = value;

	wxTimeSpan m_ts = wxTimeSpan::Minutes(m_dInterval) ;
	m_dtNow.Add(m_ts);
	MakeDateTimeLabel(m_dtNow);

	RequestRefresh( pParent );

}

void otcurrentUIDialog::SetInterval( wxCommandEvent& event ){
	int i = m_choice1->GetSelection();
	wxString c = m_choice1->GetString(i);	
	double value;
	c.ToDouble(&value);
	m_dInterval = value;
}

wxString otcurrentUIDialog::MakeDateTimeLabel(wxDateTime myDateTime)
{			
    const wxString dateLabel(myDateTime.Format( _T( "%a") ));
    m_staticTextDatetime->SetLabel(dateLabel);
    
   // m_datePickerDate->SetValue(myDateTime.GetDateOnly());
   // m_timePickerTime->SetTime(myDateTime.GetHour(),myDateTime.GetMinute(), myDateTime.GetSecond());

    return dateLabel;
}

void otcurrentUIDialog::About(wxCommandEvent& event)
{
	
       wxMessageBox(
_("Tidal Current\n------------------------------------------------------\n\n\n\n\n\nUse this data with caution.\nUse in conjunction with Tidal Current Atlases and Tidal Diamonds\n\n------------------------------------------------------\n\nNote: 1 Rates shown are for a position corresponding to the centre\nof the base of the arrow. Tidal rate is shown as knots.\n\n")
     , _("About Tidal Arrows"), wxOK | wxICON_INFORMATION, this);
}

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

#if !wxCHECK_VERSION(2,9,4) /* to work with wx 2.8 */
#define SetBitmap SetBitmapLabel
#endif


otcurrentUIDialog::otcurrentUIDialog(wxWindow *parent, otcurrent_pi *ppi)
: otcurrentUIDialogBase(parent)
{
	pParent = parent;
    pPlugIn = ppi;

    wxFileConfig *pConf = GetOCPNConfigObject();

    if(pConf) {
        pConf->SetPath ( _T ( "/Settings/otcurrent" ) );

		pConf->Read ( _T ( "otcurrentUseRate" ), &m_bUseRate );
        pConf->Read ( _T ( "otcurrentUseDirection" ), &m_bUseDirection);
		pConf->Read ( _T ( "otcurrentUseFillColour" ), &m_bUseFillColour);

		pConf->Read ( _T ( "otcurrentInterval" ), &m_IntervalSelected);
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

    m_bpPrev->SetBitmap(wxBitmap( prev1 ));
    m_bpNext->SetBitmap(wxBitmap( next1 ));
    m_bpNow->SetBitmap(*_img_Clock );


    this->Connect( wxEVT_MOVE, wxMoveEventHandler( otcurrentUIDialog::OnMove ) );

	m_dtNow = wxDateTime::Now(); 
	MakeDateTimeLabel(m_dtNow);
	
	 if (m_FolderSelected == wxEmptyString){

			  wxString g_SData_Locn = *GetpSharedDataLocation();

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
	
	m_choice1->SetSelection(m_IntervalSelected);
	int i = m_choice1->GetSelection();
	wxString c = m_choice1->GetString(i);	
	double value;
	c.ToDouble(&value);
	m_dInterval = value;

	DimeWindow( this );

    Fit();
    SetMinSize( GetBestSize() );
	
}

otcurrentUIDialog::~otcurrentUIDialog()
{
    wxFileConfig *pConf = GetOCPNConfigObject();;

    if(pConf) {
        pConf->SetPath ( _T ( "/Settings/otcurrent" ) );

		pConf->Write ( _T ( "otcurrentUseRate" ), m_bUseRate );
		pConf->Write ( _T ( "otcurrentUseDirection" ), m_bUseDirection );
		pConf->Write ( _T ( "otcurrentUseFillColour" ), m_bUseFillColour );

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
}

void otcurrentUIDialog::SetCursorLatLon( double lat, double lon )
{
    m_cursor_lon = lon;
    m_cursor_lat = lat;
}

void otcurrentUIDialog::SetViewPort( PlugIn_ViewPort *vp )
{
    if(m_vp == vp)  return;

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
	m_bUseFillColour = pPlugIn->GetCopyColour();

	m_FolderSelected = pPlugIn->GetFolderSelected();
	m_IntervalSelected = pPlugIn->GetIntervalSelected();
}

void otcurrentUIDialog::OnFolderSelChanged(wxFileDirPickerEvent& event)
{
	m_FolderSelected = m_dirPicker1->GetPath();
	RequestRefresh(pParent);	
}

void otcurrentUIDialog::OnCalendarShow( wxCommandEvent& event )
{	

	CalendarDialog CalDialog ( this, -1, _("START Date/Time"),
	                          wxPoint(100, 100), wxSize(200, 250) );
	if ( CalDialog.ShowModal() == wxID_OK ){
		
		wxDateTime dm = CalDialog.dialogCalendar->GetDate();
		wxString myTime = CalDialog._timeText->GetValue();
        wxString val = myTime.Mid(0,1);

		if ( val == wxT(" ")){
			myTime = wxT("0") + myTime.Mid(1,5);
		}
	
		wxDateTime dt;
		dt.ParseTime(myTime);
				
		wxString todayHours = dt.Format(_T("%H"));
        wxString todayMinutes = dt.Format(_T("%M"));
		
		double h;
		double m;

		todayHours.ToDouble(&h);
		todayMinutes.ToDouble(&m);
		myTimeOfDay = wxTimeSpan(h,m,0,0);	

		dm = CalDialog.dialogCalendar->GetDate();
		
		m_dtNow = dm + myTimeOfDay;

		MakeDateTimeLabel(m_dtNow);
		RequestRefresh(pParent);
	}
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
		wxDateTime dt = myDateTime;

		wxString s2 = dt.Format ( _T( "%a %d %b %Y"));
		wxString s = dt.Format(_T("%H:%M")); 
		wxString dateLabel = s2 + _T(" ") + s;	

		m_textCtrl1->SetValue(dateLabel);				

		return dateLabel;	
}

void otcurrentUIDialog::About(wxCommandEvent& event)
{
	
       wxMessageBox(
_("Tidal Current\n--------------------------------------------------------------\n\n\n\n\n\nUse this data with caution.\nUse in conjunction with Tidal Current Atlases and Tidal Diamonds\n\n--------------------------------------------------------------------\n\nNote: 1 Rates shown are for a position corresponding to the centre\nof the base of the arrow. Tidal rate is shown as knots.\n\n")
     , _("About Tidal Arrows"), wxOK | wxICON_INFORMATION, this);
}


CalendarDialog::CalendarDialog ( wxWindow * parent, wxWindowID id, const wxString & title,
                           const wxPoint & position, const wxSize & size, long style )
: wxDialog( parent, id, title, position, size, style)
{
		
	wxString dimensions = wxT(""), s;
	wxPoint p;
	wxSize  sz;
 
	sz.SetWidth(180);
	sz.SetHeight(150);
	
	p.x = 6; p.y = 2;
	s.Printf(_(" x = %d y = %d\n"), p.x, p.y);
	dimensions.append(s);
	s.Printf(_(" width = %d height = %d\n"), sz.GetWidth(), sz.GetHeight());
	dimensions.append(s);
	dimensions.append(wxT("here"));
 
	dialogCalendar = new wxCalendarCtrl(this, -1, wxDefaultDateTime, p, sz, wxCAL_SHOW_HOLIDAYS ,_("Tide Calendar"));

	m_staticText = new wxStaticText(this,wxID_ANY,_("Time:"),wxPoint(15,155),wxSize(60,21));

	_timeText = new wxTimeTextCtrl(this,wxID_ANY,wxT("12:00"),wxPoint(75,155),wxSize(60,21));

    _spinCtrl=new wxSpinButton(this,wxID_ANY,wxPoint(136,155),wxSize(20,21),wxSP_VERTICAL|wxSP_ARROW_KEYS);
	_spinCtrl->Connect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( CalendarDialog::spinUp ), NULL, this );
	_spinCtrl->Connect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( CalendarDialog::spinDown ), NULL, this );
	
	p.y += sz.GetHeight() + 30;
	wxButton * b = new wxButton( this, wxID_OK, _("OK"), p, wxDefaultSize );
	p.x += 110;
	wxButton * c = new wxButton( this, wxID_CANCEL, _("Cancel"), p, wxDefaultSize );
    
}



void CalendarDialog::spinUp(wxSpinEvent& event)
{
		_timeText->OnArrowUp();
}

void CalendarDialog::spinDown(wxSpinEvent& event)
{
         _timeText->OnArrowDown();
}
	



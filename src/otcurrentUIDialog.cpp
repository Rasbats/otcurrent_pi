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
#include "wx/tokenzr.h"
#include "wx/datetime.h"
#include "wx/sound.h"
#include <wx/wfstream.h>
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
#include <vector>

#include <iostream> 
#include <fstream>
#ifdef __WXMSW__
#include <windows.h>
#endif
#include <memory.h> 

#include <wx/colordlg.h>
#include <wx/event.h>

#include "otcurrent_pi.h"

enum
{
            FORWARD_ONE_HOUR_STEP    =3600,
            FORWARD_TEN_MINUTES_STEP =600,
            FORWARD_ONE_MINUTES_STEP =60,
            BACKWARD_ONE_HOUR_STEP    =-3600,
            BACKWARD_TEN_MINUTES_STEP =-600,
            BACKWARD_ONE_MINUTES_STEP =-60
};

using namespace std;


#if defined (_WIN32)
int round (double x) {
	int i = (int) x;
	if (x >= 0.0) {
		return ((x-i) >= 0.5) ? (i + 1) : (i);
	} else {
		return (-x+i >= 0.5) ? (i - 1) : (i);
	}
}
#endif

#define FAIL(X) do { error = X; goto failed; } while(0)


enum { SAILDOCS,ZYotcurrent };
enum { GFS,COAMPS,RTOFS };


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
    mBool = false;
	pParent = parent;
    pPlugIn = ppi;

    wxFileConfig *pConf = GetOCPNConfigObject();

    if(pConf) {
        pConf->SetPath ( _ ( "/Settings/otcurrent" ) );

		pConf->Read ( _ ( "otcurrentUseRate" ), &m_bUseRate );
        pConf->Read ( _ ( "otcurrentUseDirection" ), &m_bUseDirection);
		pConf->Read ( _ ( "otcurrentUseFillColour" ), &m_bUseFillColour);

		pConf->Read ( _ ( "otcurrentInterval" ), &m_IntervalSelected);

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
	
	m_dInterval = 0;

	DimeWindow( this );

    Fit();
    SetMinSize( GetBestSize() );	
}

otcurrentUIDialog::~otcurrentUIDialog()
{
    wxFileConfig *pConf = GetOCPNConfigObject();;

    if(pConf) {
        pConf->SetPath ( _ ( "/Settings/otcurrent" ) );

		pConf->Write ( _ ( "otcurrentUseRate" ), m_bUseRate );
		pConf->Write ( _ ( "otcurrentUseDirection" ), m_bUseDirection );
		pConf->Write ( _ ( "otcurrentUseFillColour" ), m_bUseFillColour );

		pConf->Write( _T("VColour0"), myVColour[0] );
		pConf->Write( _T("VColour1"), myVColour[1] );
		pConf->Write( _T("VColour2"), myVColour[2] );
		pConf->Write( _T("VColour3"), myVColour[3] );
		pConf->Write( _T("VColour4"), myVColour[4] );

		int c = m_choice1->GetSelection();
		wxString myP = m_choice1->GetString(c);
		pConf->Write ( _ ( "otcurrentInterval" ), c ); 



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
		
		MakeDateTimeLabel(dt);
		
		wxString todayHours = dt.Format(_T("%H"));
        wxString todayMinutes = dt.Format(_T("%M"));
		
		double h;
		double m;

		todayHours.ToDouble(&h);
		todayMinutes.ToDouble(&m);
		myTimeOfDay = wxTimeSpan(h,m,0,0);	

		dm = CalDialog.dialogCalendar->GetDate();
		
		m_dtNow = dm + myTimeOfDay;
		
		RequestRefresh(pParent);
	}
}

void otcurrentUIDialog::OnNow( wxCommandEvent& event ){
	
	m_dtNow = wxDateTime::Now();
	m_dInterval = 0;
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





void otcurrentUIDialog::LoadHarmonics()
{
	  //  Establish a "home" location
        
	  g_SData_Locn = *GetpSharedDataLocation();

      // Establish location of Tide and Current data
      pTC_Dir = new wxString(_T("tcdata"));
      pTC_Dir->Prepend(g_SData_Locn);
      pTC_Dir->Append(wxFileName::GetPathSeparator());  
	
      wxString TCDir;
      TCDir = *pTC_Dir;
      
      wxLogMessage(_("Using Tide/Current data from:  ") + TCDir);
	  wxString cache_locn = TCDir; 

	  wxString harm2test = TCDir;
      harm2test.Append( _("HARMONIC") );
	
	  ptcmgr = new TCMgr(TCDir, cache_locn);     	
}

int otcurrentUIDialog::FindPortID(wxString myPort)
{	
	        for ( int i=1 ; i<ptcmgr->Get_max_IDX() +1 ; i++ )
            {				
						IDX_entry *pIDX = ptcmgr->GetIDX_entry (i);

                        char type = pIDX->IDX_type;             // Entry "TCtcIUu" identifier
                        if ( ( type == 't' ) ||  ( type == 'T' ) )  // only Tides
                        {                              
							  wxString s = wxString(pIDX->IDX_reference_name,wxConvUTF8); 
							  if ( s == myPort)
							  {								  
								  return i;
							  }							  
						}

			}
			return 0;
}
void otcurrentUIDialog::CalcHW(int PortCode)
{
	m_choice2->Clear();	

	if (PortCode == 0)
	{
		wxMessageBox(_("No tidal data for this port"), _("No Tidal Data"));
		return;
	}
	//    Figure out this computer timezone minute offset
        wxDateTime this_now = wxDateTime::Now();
        wxDateTime this_gmt = this_now.ToGMT();

#if wxCHECK_VERSION(2, 6, 2)
        wxTimeSpan diff = this_now.Subtract ( this_gmt );
#else
        wxTimeSpan diff = this_gmt.Subtract ( this_now );
#endif		        
		int diff_mins = diff.GetMinutes();	

		IDX_entry *pIDX = ptcmgr->GetIDX_entry ( PortCode );
		int station_offset = ptcmgr->GetStationTimeOffset(pIDX); //-60 for French Harmonics_V7.zip

        m_corr_mins = station_offset - diff_mins;
        if ( this_now.IsDST() )
              m_corr_mins += 60;				
		

		//    Establish the inital drawing day as today
        m_graphday = m_datePicker1->GetValue();
        wxDateTime graphday_00 = m_datePicker1->GetValue();
        time_t t_graphday_00 = graphday_00.GetTicks();

        //    Correct a Bug in wxWidgets time support
        if ( !graphday_00.IsDST() && m_graphday.IsDST() )
                t_graphday_00 -= 3600;
        if ( graphday_00.IsDST() && !m_graphday.IsDST() )
                t_graphday_00 += 3600;

        m_t_graphday_00_at_station = t_graphday_00 - ( m_corr_mins * 60 );

        //Get the timezone of the station
        int h = (station_offset / 60);
		int m = station_offset - (h * 60);
        if ( m_graphday.IsDST() )
              h += 1;
        m_stz.Printf(_T("Z %+03d:%02d"), h, m);
		m_staticText1->SetLabel(m_stz);
		//

		int i, c, n, e;
		c = 0;
		e = 0;
		double myArrayOfRanges[6];

		float tcmax, tcmin;
		float dir;
                        
		tcmax = -10;
        tcmin = 10;
        
		float val = 0;
		int list_index = 0 ;
		int array_index = 0;
        bool  wt = 0;
		float myLW = 0.0, myHW = 0.0;

		wxString sHWLW = _T("");

                        // get tide flow sens ( flood or ebb ? )
						
                        ptcmgr->GetTideFlowSens(m_t_graphday_00_at_station, BACKWARD_ONE_HOUR_STEP, pIDX->IDX_rec_num, tcv[0], val, wt);
		
						for ( i=0 ; i<26 ; i++ )
                        {
                                int tt = m_t_graphday_00_at_station + ( i * FORWARD_ONE_HOUR_STEP );
                                ptcmgr->GetTideOrCurrent ( tt, pIDX->IDX_rec_num, tcv[i], dir );
								
                                if ( tcv[i] > tcmax )
                                        tcmax = tcv[i];

                                                if ( tcv[i] < tcmin )
                                                   tcmin = tcv[i];                                                
                                                    if ( ! ((tcv[i] > val) == wt) )                // if tide flow sens change
                                                    {
                                                      float tcvalue;                                        //look backward for HW or LW
                                                      time_t tctime;
                                                      ptcmgr->GetHightOrLowTide(tt, BACKWARD_TEN_MINUTES_STEP, BACKWARD_ONE_MINUTES_STEP, tcv[i], wt, pIDX->IDX_rec_num, tcvalue, tctime);

                                                      wxDateTime tcd ;                                                              //write date
                                                      wxString s, s1, s2;
                                                      tcd.Set( tctime + ( m_corr_mins * 60 ) ) ;

													  s2 = tcd.Format ( _T( "%A %d %B %Y"));
                                                      s.Printf(tcd.Format(_T("%H:%M  ")));													 

                                                      s1.Printf( _T("%05.2f "),tcvalue);    												  
	
													  Station_Data *pmsd = pIDX->pref_sta_data;                         //write unit
 													  
													  
													  ( wt )? sHWLW = _("HW") : sHWLW = _("LW"); 
													 											                                                        
													  // Fill the array with tide data
													  euTC[array_index][0] = s2 + _T(" ") + s;													  													
													  euTC[array_index][1] = s1;
													  euTC[array_index][2] = wxString(pmsd->units_abbrv ,wxConvUTF8);
													  euTC[array_index][3] = sHWLW;

													  if (euTC[array_index][3] == _("LW")) 
													  {									
														myLW = tcvalue;
													  }
													  
													  if (euTC[array_index][3] == _("HW")) 
													  {
														myHW = tcvalue;
														m_choice2->Insert(euTC[array_index][0],list_index); 
														// nearestHW for the now button
														nearestHW[e] = euTC[array_index][0];
														e++;														
														list_index++;
													  }  

													   myRange = myHW - myLW;
													 
													  if ((abs(myRange) == myHW) || (abs(myRange) == myLW))
													  {
															// out of range
													  }
													  else
													  {
														  myArrayOfRanges[c] = myRange;
														  c++;
													  }
														
													  array_index++;
													  
                                                      wt = !wt ;     //change tide flow sens

                                                    }

													val = tcv[i];                                                                                                
                        }
						c--;
						n = 0;
						double AddRanges = 0;
						for (n; n<c; n++){
						   AddRanges = AddRanges + myArrayOfRanges[n];
						}
						// myRange for the speed of current calculation
						myRange = AddRanges/n;												
}

double otcurrentUIDialog::CalcCurrent(double m_spRange, double m_npRange, double m_spRateDiamond, double m_npRateDiamond, double m_rangeOnDay)
{
	if (m_spRateDiamond == m_npRateDiamond)
		return m_spRateDiamond;
	else {
		// y = mx + c
		double m,c,x; 
		m = (m_spRange - m_npRange) / (m_spRateDiamond - m_npRateDiamond);
		c = m_spRange - (m * m_spRateDiamond);
		x = (m_rangeOnDay - c)/m ;	
		return x;
	}
}


int otcurrentUIDialog::CalcHoursFromHWNow()
{
	
	wxDateTime myDateTime;
	wxTimeSpan diff;
	double myDiff, myTest;

	myTest = 26;
	
	wxDateTime this_now = wxDateTime::Now();
	int t = this_now.GetTicks();
	int i = 0;
	int m; 		
	double d;

	for (i; i<8;i++)
	{
		myDateTime.ParseDateTime(nearestHW[i]);
		m = myDateTime.GetTicks(); 

		d = t - m;
		myDiff = (d/60)/60;  
		
		if (abs(myDiff) < abs(myTest))
		{
			myTest = myDiff;
		}
	}		

	int c = m_choice2->GetCount();
	for  (c=0; c<8; c++)
	{
		for (i=0; i<8;i++)
		{
		if (m_choice2->GetString(c) == nearestHW[i])
			{
				m_choice2->SetSelection(c);
			}
		}
	}

	int f = round( myTest);   
    return f ;
}

int otcurrentUIDialog::round(double c)
{   
	// c = -0.52
	int a = c; //a =  0
	int b = 0;
	double input = c;

	if (a == 0)
	{
	  if (c < 0)
	  {
	  c = c + a;   // -0.52  
	  }
	  else	
	  {
		c = c - a;   //
	  }
	}
	else
	{
	c = c - a; //-2.6 --2 c = -0.6
	}
	
	if ( abs(c) > 0.5) 
	{
		b = 1;  //1
	}
	else
	{
		b = 0;
	}
	
	if ( a > 0) //a -2
	{
		c = a + b;
	}
	else{
		if (a == 0){  
			
			if (input >= 0){
				c = b;
			}
			else{				
				c -= b;
			}   
		}
		else{		
			c = a - b;
		}
	}

	return c;
}






vector<Position> otcurrentUIDialog::OnRecord()
{	
return my_positions;
}

void otcurrentUIDialog::SetFromHW(int fromHW) 
{

 button_id = fromHW;
}






void otcurrentUIDialog::About(wxCommandEvent& event)
{
	
       wxMessageBox(
_("Tidal Data for UKHO Tidal Diamonds\n--------------------------------------------------------------\nThe standard OpenCPN distribution has tidal data for the\nfollowing ports, which this plugin uses:\n\nPLYMOUTH (DEVONPORT)\nPORTSMOUTH\nDOVER\nSHEERNESS\nLOWESTOFT\nIMMINGHAM\nLEITH\nABERDEEN\nWICK\nLERWICK\nULLAPOOL\nLIVERPOOL (GLADSTONE DOCK)\nHOLYHEAD\nMILFORD HAVEN\nPORT OF BRISTOL (AVONMOUTH)\nST. HELIER\n\nUse this data with caution.\nUse in conjunction with UKHO Tidal Stream Atlases and tidal diamonds\n\n--------------------------------------------------------------------\n\nNote: 1 Rates shown are for a position corresponding to the centre\nof the base of the arrow. Tidal rate is shown as knots.\nNote: 2 Rates are calculated by using the method shown in UKHO Tidal\nStream Atlases. This is based on the average range for the day\n")
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
 
	dialogCalendar = new wxCalendarCtrl(this, -1, wxDefaultDateTime, p, sz, wxCAL_SHOW_HOLIDAYS ,wxT("Tide Calendar"));

	m_staticText = new wxStaticText(this,wxID_ANY,wxT("Time:"),wxPoint(15,155),wxSize(60,21));

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
	



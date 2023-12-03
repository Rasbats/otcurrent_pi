/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  otcurrent Plugin Friends
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
 */

#ifndef __otcurrentUIDIALOG_H__
#define __otcurrentUIDIALOG_H__

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include <wx/fileconf.h>
#include <wx/glcanvas.h>

#include "otcurrentUIDialogBase.h"

#include "tinyxml.h"
#include <wx/progdlg.h>
#include <list>
#include <vector>

#include "tcmgr.h"

#include "wx/dateevt.h"
#include "wx/stattext.h"
#include "ocpn_plugin.h"
#include "wx/dialog.h"
#include <wx/calctrl.h>
#include "wx/window.h"
#include "timectrl.h"
#include <wx/colordlg.h>
#include <wx/dirdlg.h>
#include <wx/arrstr.h>
#include <wx/sizer.h>
#include "qtstylesheet.h"

using namespace std;

#ifndef PI
#define PI 3.1415926535897931160E0 /* pi */
#endif

#if !defined(NAN)
static const long long lNaN = 0xfff8000000000000;
#define NAN (*(double*)&lNaN)
#endif

#define RT_RCDATA2 MAKEINTRESOURCE(999)

class otcurrentOverlayFactory;
class PlugIn_ViewPort;
class PositionRecordSet;

class wxFileConfig;
class otcurrent_pi;
class wxGraphicsContext;
class IDX_entry;

class Position {
public:
  double latD, lonD;
  wxString lat, lon;
  wxString port_num;

  wxString minus_6, minus_5, minus_4, minus_3, minus_2, minus_1, zero;
  wxString plus_1, plus_2, plus_3, plus_4, plus_5, plus_6;
  Position *prev, *next; /* doubly linked circular list of positions */
};

class PortTides {
public:
  wxString m_portID, m_portName, m_IDX;
  double m_spRange, m_npRange;
};

class StandardPort {
public:
  wxString PORT_NUMBER, PORT_NAME, MEAN_SPRING_RANGE, MEAN_NEAP_RANGE, EXTRA,
      IDX;
};

class otcurrentUIDialog : public otcurrentUIDialogBase {
public:
  otcurrentUIDialog(wxWindow* parent, otcurrent_pi* ppi);
  ~otcurrentUIDialog();

#ifdef __OCPN__ANDROID__
  void OnMouseEvent(wxMouseEvent& event);
#endif

  void OpenFile(bool newestFile = false);

  void SetCursorLatLon(double lat, double lon);
  void SetViewPort(PlugIn_ViewPort* vp);
  PlugIn_ViewPort* vp;

  int round(double c);

  bool m_bUseRate;
  bool m_bUseDirection;
  bool m_bUseHighRes;
  bool m_bUseFillColour;

  wxString myUseColour[5];

  wxDateTime m_dtNow;
  double m_dInterval;

  bool onNext;
  bool onPrev;

  wxString m_FolderSelected;
  TCMgr* ptcmgr;
  int m_IntervalSelected;

  time_t myCurrentTime;

  void OnCalendarShow(wxCommandEvent& event);
  void OnSelectData(wxCommandEvent& event);
  void OnSelectInterval(wxCommandEvent& event);
  void OnNow(wxCommandEvent& event);
  wxString MakeDateTimeLabel(wxDateTime myDateTime);

  wxArrayString TideCurrentDataSet;

  void LoadTCMFile();
  void LoadHarmonics();

private:
  void OnClose(wxCloseEvent& event);
  void OnPrev(wxCommandEvent& event);
  void OnNext(wxCommandEvent& event);
  void SetInterval(wxCommandEvent& event);
  void About(wxCommandEvent& event);

  //    Data
  wxWindow* pParent;
  otcurrent_pi* pPlugIn;

  PlugIn_ViewPort* m_vp;

  double m_cursor_lat, m_cursor_lon;
  wxString g_SData_Locn;
  wxString* pTC_Dir;

  int m_corr_mins;
  wxString m_stz;
  int m_t_graphday_00_at_station;
  wxDateTime m_graphday;
  int m_plot_y_offset;

  bool isNowButton;
  wxTimeSpan myTimeOfDay;
};

class CalendarDialog : public wxDialog {
public:
  CalendarDialog(wxWindow* parent, wxWindowID id, const wxString& title,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

#ifdef __OCPN__ANDROID__
#include "android/wx/datectrl.h"
  wxDatePickerCtrl* dialogCalendar;
#else
  class wxDatePickerCtrl;
  wxCalendarCtrl* dialogCalendar;
#endif
  wxBoxSizer* itemBoxSizer;
  wxBoxSizer* itemBoxSizer1;
  wxBoxSizer* itemBoxSizer2;
  wxBoxSizer* itemBoxSizerFinal;

  wxButton* c;
  wxButton* b;

  wxStaticText* m_staticTextDate;
  wxStaticText* m_staticText;
  wxTextCtrl* _timeText;

private:

};

#endif

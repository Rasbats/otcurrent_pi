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
#include "otcurrent_pi.h"
#include "qtstylesheet.h"

#ifdef __ANDROID__
wxWindow* g_Window;
#endif

using namespace std;

#define FAIL(X)  \
  do {           \
    error = X;   \
    goto failed; \
  } while (0)

// date/time in the desired time zone format
static wxString TToString(const wxDateTime date_time, const int time_zone) {
  wxDateTime t(date_time);
  t.MakeFromTimezone(wxDateTime::UTC);
  if (t.IsDST()) t.Subtract(wxTimeSpan(1, 0, 0, 0));
  switch (time_zone) {
    case 0:
      return t.Format(_T(" %a %d-%b-%Y  %H:%M LOC"), wxDateTime::Local);
    case 1:
    default:
      return t.Format(_T(" %a %d-%b-%Y %H:%M  UTC"), wxDateTime::UTC);
  }
}

#if !wxCHECK_VERSION(2, 9, 4) /* to work with wx 2.8 */
#define SetBitmap SetBitmapLabel
#endif

otcurrentUIDialog::otcurrentUIDialog(wxWindow* parent, otcurrent_pi* ppi)
    : otcurrentUIDialogBase(parent), ptcmgr(0), m_vp(0) {
  this->Fit();
  pParent = parent;
  pPlugIn = ppi;

  wxFileConfig* pConf = GetOCPNConfigObject();
#ifdef __ANDROID__

  g_Window = this;
  GetHandle()->setStyleSheet(qtStyleSheet);

  Connect(wxEVT_MOTION, wxMouseEventHandler(otcurrentUIDialog::OnMouseEvent));

#endif

  if (pConf) {
    pConf->SetPath(_T ( "/PlugIns/otcurrent_pi" ));

    pConf->Read(_T ( "otcurrentUseRate" ), &m_bUseRate);
    pConf->Read(_T ( "otcurrentUseDirection" ), &m_bUseDirection);
    pConf->Read(_T("otcurrentUseHighResolution"), &m_bUseHighRes);
    pConf->Read(_T ( "otcurrentUseFillColour" ), &m_bUseFillColour);
    pConf->Read("otcurrentArrowScale", &m_arrow_scale);

    pConf->Read(_T("VColour0"), &myVColour[0], myVColour[0]);
    pConf->Read(_T("VColour1"), &myVColour[1], myVColour[1]);
    pConf->Read(_T("VColour2"), &myVColour[2], myVColour[2]);
    pConf->Read(_T("VColour3"), &myVColour[3], myVColour[3]);
    pConf->Read(_T("VColour4"), &myVColour[4], myVColour[4]);

    myUseColour[0] = myVColour[0];
    myUseColour[1] = myVColour[1];
    myUseColour[2] = myVColour[2];
    myUseColour[3] = myVColour[3];
    myUseColour[4] = myVColour[4];
  }

  m_bpPrev->SetBitmap(wxBitmap(prev1));
  m_bpNext->SetBitmap(wxBitmap(next1));
  m_bpNow->SetBitmap(*_img_Clock);

  // this->Connect( wxEVT_MOVE, wxMoveEventHandler( otcurrentUIDialog::OnMove )
  // );
  m_dtNow = wxDateTime::Now();
  MakeDateTimeLabel(m_dtNow);

  m_IntervalSelected = pPlugIn->GetIntervalSelected();
  m_FolderSelected = pPlugIn->GetFolderSelected();

  m_dirPicker1->SetValue(m_FolderSelected);
  m_choice1->SetSelection(m_IntervalSelected);

  LoadTCMFile();
  LoadHarmonics();

  int i = m_choice1->GetSelection();
  wxString c = m_choice1->GetString(i);
  double value;
  c.ToDouble(&value);
  m_dInterval = value;
}

#ifdef __ANDROID__
wxPoint g_mouse_pos_screen;

void otcurrentUIDialog::OnMouseEvent(wxMouseEvent& event) {
  g_mouse_pos_screen = ClientToScreen(event.GetPosition());

  if (event.Dragging()) {
    m_resizeStartPoint = event.GetPosition();
    int x = wxMax(0, m_resizeStartPoint.x);
    int y = wxMax(0, m_resizeStartPoint.y);
    int xmax = ::wxGetDisplaySize().x - GetSize().x;
    x = wxMin(x, xmax);
    int ymax =
        ::wxGetDisplaySize().y - (GetSize().y);  // Some fluff at the bottom
    y = wxMin(y, ymax);

    g_Window->Move(x, y);
  }
}
#endif  // End of Android functions for move/resize

void otcurrentUIDialog::LoadHarmonics() {
  if (!ptcmgr) {
    ptcmgr = new TCMgr;
    ptcmgr->LoadDataSources(TideCurrentDataSet);
  } else {
    bool b_newdataset = false;

    //      Test both ways
    wxArrayString test = ptcmgr->GetDataSet();
    for (unsigned int i = 0; i < test.GetCount(); i++) {
      bool b_foundi = false;
      for (unsigned int j = 0; j < TideCurrentDataSet.GetCount(); j++) {
        if (TideCurrentDataSet[j] == test[i]) {
          b_foundi = true;
          break;  // j loop
        }
      }
      if (!b_foundi) {
        b_newdataset = true;
        break;  //  i loop
      }
    }

    test = TideCurrentDataSet;
    for (unsigned int i = 0; i < test.GetCount(); i++) {
      bool b_foundi = false;
      for (unsigned int j = 0; j < ptcmgr->GetDataSet().GetCount(); j++) {
        if (ptcmgr->GetDataSet()[j] == test[i]) {
          b_foundi = true;
          break;  // j loop
        }
      }
      if (!b_foundi) {
        b_newdataset = true;
        break;  //  i loop
      }
    }

    if (b_newdataset) ptcmgr->LoadDataSources(TideCurrentDataSet);
  }
}

void otcurrentUIDialog::LoadTCMFile() {
  wxString TCDir = m_FolderSelected;
  TCDir.Append(wxFileName::GetPathSeparator());
  wxLogMessage(_("Using Tide/Current data from:  ") + TCDir);

  wxString default_tcdata0 = TCDir + _T("harmonics-dwf-20210110-free.tcd");
  wxString default_tcdata1 = TCDir + _T("HARMONIC.IDX");

  // if (!TideCurrentDataSet.GetCount()) {
  TideCurrentDataSet.Add(default_tcdata0);
  TideCurrentDataSet.Add(default_tcdata1);
  //}
}

otcurrentUIDialog::~otcurrentUIDialog() {
  wxFileConfig* pConf = GetOCPNConfigObject();

  if (pConf) {
    pConf->SetPath(_T ( "/PlugIns/otcurrent_pi" ));

    pConf->Write(_T ( "otcurrentUseRate" ), m_bUseRate);
    pConf->Write(_T ( "otcurrentUseDirection" ), m_bUseDirection);
    pConf->Write(_T("otcurrentUseHighResolution"), m_bUseHighRes);
    pConf->Write(_T ( "otcurrentUseFillColour" ), m_bUseFillColour);
    pConf->Write("otcurrentArrowScale", m_arrow_scale);

    pConf->Write(_T("VColour0"), myVColour[0]);
    pConf->Write(_T("VColour1"), myVColour[1]);
    pConf->Write(_T("VColour2"), myVColour[2]);
    pConf->Write(_T("VColour3"), myVColour[3]);
    pConf->Write(_T("VColour4"), myVColour[4]);

    int c = m_choice1->GetSelection();
    m_IntervalSelected = c;
    wxString myP = m_choice1->GetString(c);
    pConf->Write(_T ( "otcurrentInterval" ), myP);

    wxString myF = m_dirPicker1->GetValue();
    pConf->Write(_T ( "otcurrentFolder" ), myF);
  }
  delete ptcmgr;
}

void otcurrentUIDialog::SetCursorLatLon(double lat, double lon) {
  m_cursor_lon = lon;
  m_cursor_lat = lat;
}

void otcurrentUIDialog::SetViewPort(PlugIn_ViewPort* vp) {
  if (m_vp == vp) return;

  delete m_vp;
  m_vp = new PlugIn_ViewPort(*vp);
}

void otcurrentUIDialog::OnClose(wxCloseEvent& event) {
  m_FolderSelected = m_dirPicker1->GetValue();
  pPlugIn->m_CopyFolderSelected = m_FolderSelected;

  int i = m_choice1->GetSelection();
  m_IntervalSelected = i;
  pPlugIn->m_CopyIntervalSelected = m_IntervalSelected;

  pPlugIn->OnotcurrentDialogClose();
}

void otcurrentUIDialog::OpenFile(bool newestFile) {
  m_bUseRate = pPlugIn->GetCopyRate();
  m_bUseDirection = pPlugIn->GetCopyDirection();
  m_bUseHighRes = pPlugIn->GetCopyResolution();
  m_bUseFillColour = pPlugIn->GetCopyColour();
  m_arrow_scale = pPlugIn->GetCopyArrowScale() + 1;

  m_IntervalSelected = pPlugIn->GetIntervalSelected();
  if (m_FolderSelected == wxEmptyString) {
#ifndef __ANDROID__
    m_FolderSelected = pPlugIn->GetFolderSelected();
    m_dirPicker1->SetValue(m_FolderSelected);
    wxDirDialog* d = new wxDirDialog(this, _("Choose the tcdata directory"), "",
                                     0, wxDefaultPosition);
    if (d->ShowModal() == wxID_OK) {
      m_dirPicker1->SetValue(d->GetPath());
      m_FolderSelected = m_dirPicker1->GetValue();
    }
#else
    wxString tc =
        "/storage/emulated/0/Android/data/org.opencpn.opencpn/files/tcdata";
    m_dirPicker1->SetValue(tc);
    m_FolderSelected = tc;

#endif
  }

  LoadTCMFile();
}

void otcurrentUIDialog::OnSelectData(wxCommandEvent& event) {
#ifndef __ANDROID__
  wxDirDialog* d =
      new wxDirDialog(this, _("Choose a directory"), "", 0, wxDefaultPosition);
  if (d->ShowModal() == wxID_OK) {
    m_dirPicker1->SetValue(d->GetPath());
    m_FolderSelected = m_dirPicker1->GetValue();
    pPlugIn->m_CopyFolderSelected = m_FolderSelected;
  }
#else
  wxString dir_spec;
  int response = PlatformDirSelectorDialog(g_Window, &dir_spec,
                                           _("Choose Harmonics Directory"),
                                           m_dirPicker1->GetValue());
  if (response == wxID_OK) {
    m_dirPicker1->SetValue(dir_spec);
    m_FolderSelected = dir_spec;
    pPlugIn->m_CopyFolderSelected = m_FolderSelected;
  }
#endif

  LoadTCMFile();
  LoadHarmonics();
  RequestRefresh(pParent);
}

void otcurrentUIDialog::OnSelectInterval(wxCommandEvent& event) {
  int i = m_choice1->GetSelection();
  m_IntervalSelected = i;
  pPlugIn->m_CopyIntervalSelected = m_IntervalSelected;
}
void otcurrentUIDialog::OnCalendarShow(wxCommandEvent& event) {
  CalendarDialog CalDialog(this, -1, _("START Date/Time"), wxPoint(100, 100),
                           wxSize(-1, -1));

#ifdef __ANDROID__
  wxDateTime now = wxDateTime::Now();
  CalDialog.dialogCalendar->SetValue(now);
#endif

  if (CalDialog.ShowModal() == wxID_OK) {
    wxDateTime dm = CalDialog.dialogCalendar->GetDate();
    wxString myTime = CalDialog._timeText->GetValue();
    wxString val = myTime.Mid(0, 1);

    if (val == wxT(" ")) {
      myTime = wxT("0") + myTime.Mid(1, 5);
    }

    wxDateTime dt;
    dt.ParseTime(myTime);

    wxString todayHours = dt.Format(_T("%H"));
    wxString todayMinutes = dt.Format(_T("%M"));

    double h;
    double m;

    todayHours.ToDouble(&h);
    todayMinutes.ToDouble(&m);
    myTimeOfDay = wxTimeSpan(h, m, 0, 0);

    dm = CalDialog.dialogCalendar->GetDate();

    m_dtNow = dm + myTimeOfDay;

    MakeDateTimeLabel(m_dtNow);
    RequestRefresh(pParent);
  }
}

void otcurrentUIDialog::OnNow(wxCommandEvent& event) {
  m_dtNow = wxDateTime::Now();
  MakeDateTimeLabel(m_dtNow);

  RequestRefresh(pParent);

  onPrev = false;
  onNext = false;
}

void otcurrentUIDialog::OnPrev(wxCommandEvent& event) {
  int i = m_choice1->GetSelection();
  wxString c = m_choice1->GetString(i);
  double value;
  c.ToDouble(&value);
  m_dInterval = value;

  wxTimeSpan m_ts = wxTimeSpan::Minutes(m_dInterval);
  m_dtNow.Subtract(m_ts);
  MakeDateTimeLabel(m_dtNow);

  RequestRefresh(pParent);
}

void otcurrentUIDialog::OnNext(wxCommandEvent& event) {
  int i = m_choice1->GetSelection();
  wxString c = m_choice1->GetString(i);

  double value;
  c.ToDouble(&value);
  m_dInterval = value;

  wxTimeSpan m_ts = wxTimeSpan::Minutes(m_dInterval);
  m_dtNow.Add(m_ts);
  MakeDateTimeLabel(m_dtNow);

  RequestRefresh(pParent);
}

void otcurrentUIDialog::SetInterval(wxCommandEvent& event) {
  int i = m_choice1->GetSelection();
  wxString c = m_choice1->GetString(i);
  double value;
  c.ToDouble(&value);
  m_dInterval = value;
}

wxString otcurrentUIDialog::MakeDateTimeLabel(wxDateTime myDateTime) {
  wxDateTime dt = myDateTime;

  wxString s2 = dt.Format(_T( "%a %d %b %Y"));
  wxString s = dt.Format(_T("%H:%M"));
  wxString dateLabel = s2 + _T(" ") + s;

  m_textCtrl1->SetValue(dateLabel);

  return dateLabel;
}

void otcurrentUIDialog::About(wxCommandEvent& event) {
  wxMessageBox(_("Tidal "
                 "Current\n----------------------------------------------------"
                 "----------\n\n\n\n\n\nUse this data with caution.\nUse in "
                 "conjunction with Tidal Current Atlases and Tidal "
                 "Diamonds\n\n-------------------------------------------------"
                 "-------------------\n\nNote: 1 Rates shown are for a "
                 "position corresponding to the centre\nof the base of the "
                 "arrow. Tidal rate is shown as knots.\n\n"),
               _("About Tidal Arrows"), wxOK | wxICON_INFORMATION, this);
}

CalendarDialog::CalendarDialog(wxWindow* parent, wxWindowID id,
                               const wxString& title, const wxPoint& position,
                               const wxSize& size, long style)
    : wxDialog(parent, id, title, position, size, style) {
  wxString dimensions = wxT(""), s;
  wxPoint p;
  wxSize sz;

  sz.SetWidth(200);
  sz.SetHeight(600);

  p.x = 6;
  p.y = 2;
  s.Printf(_(" x = %d y = %d\n"), p.x, p.y);
  dimensions.append(s);
  s.Printf(_(" width = %d height = %d\n"), sz.GetWidth(), sz.GetHeight());
  dimensions.append(s);
  dimensions.append(wxT("here"));

  itemBoxSizerFinal = new wxBoxSizer(wxVERTICAL);

#ifndef __ANDROID__

  dialogCalendar = new wxCalendarCtrl(this, -1, wxDefaultDateTime,
                                      wxDefaultPosition, wxDefaultSize,
                                      wxCAL_SHOW_HOLIDAYS, _("Tide Calendar"));

#else
  m_staticTextDate = new wxStaticText(this, wxID_ANY, _("Date:"),
                                      wxDefaultPosition, wxDefaultSize);
  itemBoxSizerFinal->Add(m_staticTextDate, 0, wxEXPAND | wxALL, 10);
  dialogCalendar =
      new wxDatePickerCtrl(this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition,
                           wxDefaultSize, wxDP_DEFAULT);
#endif

  itemBoxSizerFinal->Add(dialogCalendar, 0, wxEXPAND | wxALL, 10);

  itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
  m_staticText = new wxStaticText(this, wxID_ANY, _("Time:"), wxDefaultPosition,
                                  wxDefaultSize);
  _timeText = new wxTextCtrl(this, wxID_ANY, "12:00", wxDefaultPosition,
                             wxDefaultSize, 0);

  itemBoxSizer1->Add(m_staticText, 1, wxEXPAND | wxALL, 10);
  itemBoxSizer1->Add(_timeText, 1, wxEXPAND | wxALL, 10);

  itemBoxSizerFinal->Add(itemBoxSizer1, 0, wxEXPAND | wxALL, 10);

  itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);

  c = new wxButton(this, wxID_CANCEL, _("Cancel"), p, wxDefaultSize);
  b = new wxButton(this, wxID_OK, _("OK"), p, wxDefaultSize);

  itemBoxSizer2->Add(c, 1, wxEXPAND | wxALL, 10);
  itemBoxSizer2->Add(b, 1, wxEXPAND | wxALL, 10);

  itemBoxSizerFinal->Add(itemBoxSizer2, 0, wxEXPAND | wxALL, 10);

  this->SetSizer(itemBoxSizerFinal);
  this->Layout();
  itemBoxSizerFinal->Fit(this);
}

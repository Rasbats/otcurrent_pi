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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // precompiled headers

#include <wx/glcanvas.h>
#include <wx/graphics.h>
#include <wx/progdlg.h>

#include "otcurrent_pi.h"
#include <vector>

#ifdef __WXOSX__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#endif

#ifdef USE_GLES2
#include "GLES2/gl2.h"
#endif

#ifdef __WXMSW__
#define snprintf _snprintf
#endif  // __WXMSW__

using namespace std;

class Position;
class otcurrentUIDialog;
class PlugIn_ViewPort;
class wxBoundingBox;
class piDC;

#define NUM_CURRENT_ARROW_POINTS 9
static wxPoint CurrentArrowArray[NUM_CURRENT_ARROW_POINTS] = {
    wxPoint(0, 0),    wxPoint(0, -10), wxPoint(55, -10),
    wxPoint(55, -25), wxPoint(100, 0), wxPoint(55, 25),
    wxPoint(55, 10),  wxPoint(0, 10),  wxPoint(0, 0)};
static int texture_format;
static bool glQueried = false;

static GLboolean QueryExtension(const char *extName) {
  /*
   ** Search for extName in the extensions string. Use of strstr()
   ** is not sufficient because extension names can be prefixes of
   ** other extension names. Could use strtok() but the constant
   ** string returned by glGetString might be in read-only memory.
   */
  char *p;
  char *end;
  int extNameLen;

  extNameLen = strlen(extName);

  p = (char *)glGetString(GL_EXTENSIONS);
  if (NULL == p) {
    return GL_FALSE;
  }

  end = p + strlen(p);

  while (p < end) {
    int n = strcspn(p, " ");
    if ((extNameLen == n) && (strncmp(extName, p, n) == 0)) {
      return GL_TRUE;
    }
    p += (n + 1);
  }
  return GL_FALSE;
}

#if defined(__WXMSW__)
#define systemGetProcAddress(ADDR) wglGetProcAddress(ADDR)
#elif defined(__WXOSX__)
#include <dlfcn.h>
#define systemGetProcAddress(ADDR) dlsym(RTLD_DEFAULT, ADDR)
#elif defined(__OCPN__ANDROID__)
#define systemGetProcAddress(ADDR) eglGetProcAddress(ADDR)
#else
#define systemGetProcAddress(ADDR) glXGetProcAddress((const GLubyte *)ADDR)
#endif

//----------------------------------------------------------------------------------------------------------
//    otcurrent Overlay Factory Implementation
//----------------------------------------------------------------------------------------------------------
otcurrentOverlayFactory::otcurrentOverlayFactory(otcurrentUIDialog &dlg)
    : m_dlg(dlg) {
  m_last_vp_scale = 0.;
  m_bShowRate = m_dlg.m_bUseRate;
  m_bShowDirection = m_dlg.m_bUseDirection;
  m_bHighResolution = m_dlg.m_bUseHighRes;
  m_bShowFillColour = m_dlg.m_bUseFillColour;

  m_dtUseNew = m_dlg.m_dtNow;
}

otcurrentOverlayFactory::~otcurrentOverlayFactory() {}

void otcurrentOverlayFactory::Reset() {}

bool otcurrentOverlayFactory::RenderOverlay(piDC &dc, PlugIn_ViewPort &vp) {
  m_dc = &dc;

  if (!dc.GetDC()) {
    if (!glQueried) {
      glQueried = true;
    }
#ifndef USE_GLSL
    glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT);  // Save state

    //      Enable anti-aliased lines, at best quality
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
    glEnable(GL_BLEND);
  }

  wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
              wxFONTWEIGHT_NORMAL);
  m_dc->SetFont(font);

  wxColour myColour = wxColour("RED");

  /*
  std::vector<Position> mypoints =  m_dlg.my_points;

  for (std::vector<Position>::iterator it = mypoints.begin(); it !=
  mypoints.end(); it++) {


          wxPoint p, pn;
  GetCanvasPixLL( &vp, &p, (it)->myLat, (it)->myLon);
          GetCanvasPixLL( &vp, &pn, (it)->myNextLat, (it)->myNextLon);

  DrawLine(p.x, p.y, pn.x, pn.y, myColour, 4);
  }
  */

  DrawAllCurrentsInViewPort(&vp, false, false, false, m_dtUseNew);

  /*
  wxPen pen("RED", 2);

  m_dc->SetPen(pen);
        m_dc->DrawLine(140, 140, 220, 220);
  */

  return true;
}

wxColour otcurrentOverlayFactory::GetSpeedColour(double my_speed) {
  wxColour c_blue = wxColour(m_dlg.myUseColour[0]);           // 127, 0, 255);
  wxColour c_green = wxColour(m_dlg.myUseColour[1]);          // 0, 166, 80);
  wxColour c_yellow_orange = wxColour(m_dlg.myUseColour[2]);  // 253, 184, 19);
  wxColour c_orange = wxColour(m_dlg.myUseColour[3]);         // 248, 128, 64);
  wxColour c_red = wxColour(m_dlg.myUseColour[4]);            // 248, 0, 0);

  if (my_speed < 0.5) {
    return c_blue;
  }
  if ((my_speed >= 0.5) && (my_speed < 1.5)) {
    return c_green;
  }
  if ((my_speed >= 1.5) && (my_speed < 2.5)) {
    return c_yellow_orange;
  }
  if ((my_speed >= 2.5) && (my_speed < 3.5)) {
    return c_orange;
  }
  if ((my_speed >= 3.5)) {
    return c_red;
  }

  return wxColour(0, 0, 0);
}

bool otcurrentOverlayFactory::drawCurrentArrow(int x, int y, double rot_angle,
                                               double scale, double rate) {
  double m_rate = fabs(rate);

  wxColour colour;
  colour = GetSpeedColour(m_rate);

  c_GLcolour = colour;  // for filling GL arrows
  if (scale <= 1e-2) return false;

  wxBrush brush(colour);

  if (m_dc) {
    wxPen pen(colour, 2);

    m_dc->SetPen(pen);
    m_dc->SetBrush(brush);
  }

  float sin_rot = sin(rot_angle * PI / 180.);
  float cos_rot = cos(rot_angle * PI / 180.);

  // Move to the first point

  float xt = CurrentArrowArray[0].x;
  float yt = CurrentArrowArray[0].y;

  float xp = (xt * cos_rot) - (yt * sin_rot);
  float yp = (xt * sin_rot) + (yt * cos_rot);
  int x1 = (int)(xp * scale);
  int y1 = (int)(yp * scale);

  p[0].x = x1 + x;
  p[0].y = y1 + y;

  p_basic[0].x = 100;
  p_basic[0].y = 100;

  // Walk thru the point list
  for (int ip = 1; ip < NUM_CURRENT_ARROW_POINTS; ip++) {
    xt = CurrentArrowArray[ip].x;
    yt = CurrentArrowArray[ip].y;

    float xp = (xt * cos_rot) - (yt * sin_rot);
    float yp = (xt * sin_rot) + (yt * cos_rot);
    int x2 = (int)(xp * scale);
    int y2 = (int)(yp * scale);

    p_basic[ip].x = 100 + x2;
    p_basic[ip].y = 100 + y2;

    if (m_dc) {
      m_dc->DrawLine(x1 + x, y1 + y, x2 + x, y2 + y);
    }

    p[ip].x = x2 + x;
    p[ip].y = y2 + y;

    x1 = x2;
    y1 = y2;
  }

  if (m_bShowFillColour && m_dc) {
    /*
     *           4
     *          /\
     *         /  \
     *        /    \
     *     3 /      \ 5
     *      /_ 2   6_\
     *        |    |
     *        |    |
     *        |    |
     *        |____|
     *       1   0  7
     */

    polyPoints[0] = p[3];
    polyPoints[1] = p[4];
    polyPoints[2] = p[5];   

    rectPoints[0] = p[1];
    rectPoints[1] = p[2];
    rectPoints[2] = p[6];
    rectPoints[3] = p[7];

    // polyPoints[4] = p[8];

    brush.SetStyle(wxBRUSHSTYLE_SOLID);
    m_dc->SetBrush(brush);

    m_dc->DrawPolygon(3, polyPoints);
    m_dc->DrawPolygon(4, rectPoints);
  }
  return true;
}

wxImage &otcurrentOverlayFactory::DrawGLText(double value, int precision) {
  wxString labels;

  labels.Printf(_T("%.*f"), precision, value);
  return DrawGLTextString(labels);
}

wxImage &otcurrentOverlayFactory::DrawGLTextDir(double value, int precision) {
  wxString labels;

  labels.Printf(_T("%03.*f"), precision, value);
  return DrawGLTextString(labels);
}

wxImage &otcurrentOverlayFactory::DrawGLTextString(wxString myText) {
  wxString labels;
  labels = myText;
  std::map<wxString, wxImage>::iterator it;

  it = m_labelCacheText.find(labels);
  if (it != m_labelCacheText.end()) return it->second;

  wxMemoryDC mdc(wxNullBitmap);

  mdc.SetFont(*pTCFont);

  int w, h;
  mdc.GetTextExtent(labels, &w, &h);

  int label_offset = 10;  // 5

  wxBitmap bm(w + label_offset * 2, h + 1);
  mdc.SelectObject(bm);
  mdc.Clear();

  wxPen penText(m_text_color);
  mdc.SetPen(penText);

  mdc.SetBrush(*wxTRANSPARENT_BRUSH);
  mdc.SetTextForeground(m_text_color);
  mdc.SetTextBackground(wxTRANSPARENT);

  int xd = 0;
  int yd = 0;

  mdc.DrawText(labels, label_offset + xd, yd + 1);
  mdc.SelectObject(wxNullBitmap);

  m_labelCacheText[myText] = bm.ConvertToImage();

  m_labelCacheText[myText].InitAlpha();

  wxImage &image = m_labelCacheText[myText];

  unsigned char *d = image.GetData();
  unsigned char *a = image.GetAlpha();

  w = image.GetWidth(), h = image.GetHeight();
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int r, g, b;
      int ioff = (y * w + x);
      r = d[ioff * 3 + 0];
      g = d[ioff * 3 + 1];
      b = d[ioff * 3 + 2];

      a[ioff] = 255 - (r + g + b) / 3;
    }
  }
  return image;
}

void otcurrentOverlayFactory::DrawAllCurrentsInViewPort(
    PlugIn_ViewPort *BBox, bool bRebuildSelList, bool bforce_redraw_currents,
    bool bdraw_mono_for_mask, wxDateTime myTime) {
  if (BBox->chart_scale > 1000000) {
    return;
  }
  wxColour text_color;

  GetGlobalColor(_T ("UINFD" ), &text_color);
  if (text_color != m_text_color) {
    // color changed, invalid cache
    m_text_color = text_color;
    m_labelCacheText.clear();
  }

  double rot_vp = BBox->rotation * 180 / M_PI;

  // Set up the scaler
  double mmx = PlugInGetDisplaySizeMM();

  int sx, sy;
  wxDisplaySize(&sx, &sy);

  double m_pix_per_mm = ((double)sx) / (mmx);

  int mm_per_knot = 10;
  float current_draw_scaler = mm_per_knot * m_pix_per_mm * 100 / 100.0;

  // End setting up scaler

  float tcvalue, dir;
  bool bnew_val = true;
  double lon_last = 0.;

  double lat_last = 0.;
  TCMgr *ctcmgr = m_dlg.ptcmgr;

  wxDateTime yn = m_dlg.m_dtNow;
  time_t myTimeNow = yn.GetTicks();

  wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
              wxFONTWEIGHT_NORMAL);
#ifdef __WXMSW__
  double factor = (double)(GetOCPNCanvasWindow()->ToDIP(100)) / 100.;
  font.Scale(1. / factor);
#endif

  m_dc->SetFont(font);
  wxRect myRect = BBox->rv_rect;

  for (int i = 1; i < ctcmgr->Get_max_IDX() + 1; i++) {
    const IDX_entry *pIDX = ctcmgr->GetIDX_entry(i);
    double lon = pIDX->IDX_lon;
    double lat = pIDX->IDX_lat;
    bool b_dup = false;

    char type = pIDX->IDX_type;  // Entry "TCtcIUu" identifier

    if (((type == 'c') || (type == 'C')) && (1 /*pIDX->IDX_Useable*/)) {
      if ((lat == lat_last) && (lon == lon_last)) {
        b_dup = true;
        lon_last = lon;
        lat_last = lat;
        continue;
      }
      int pixxc, pixyc;
      wxPoint cpoint;
      GetCanvasPixLL(BBox, &cpoint, lat, lon);
      pixxc = cpoint.x;
      pixyc = cpoint.y;

      if (!b_dup && myRect.Contains(cpoint.x, cpoint.y)) {
        if (ctcmgr->GetTideOrCurrent15(myTimeNow, i, tcvalue, dir, bnew_val)) {
          if (type == 'c' || type == 'C') {
            //    Adjust drawing size using logarithmic scale
            double a1 = fabs(tcvalue) * 10;
            a1 = wxMax(1.0, a1);  // Current values less than 0.1 knot
                                  // will be displayed as 0
            double a2 = log10(a1);
            double scale = current_draw_scaler * a2;

            bool rendered = drawCurrentArrow(pixxc, pixyc, dir - 90 + rot_vp,
                                             scale / 100, tcvalue);

            int shift = 0;

            char sbuf[20];
            if (m_bShowRate) {
              snprintf(sbuf, 19, "%3.1f", fabs(tcvalue));
              m_dc->DrawText(wxString(sbuf, wxConvUTF8), pixxc, pixyc);
              if (!m_bHighResolution) {
                shift = 13;
              } else {
                shift = 35;
              }
            }

            if (m_bShowDirection) {
              snprintf(sbuf, 19, "%03.0f", dir);
              m_dc->DrawText(wxString(sbuf, wxConvUTF8), pixxc, pixyc + shift);
            }
          }
        }

        lon_last = lon;
        lat_last = lat;
      }
    }
  }
}

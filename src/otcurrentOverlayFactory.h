/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  otcurrent Plugin Freinds
 * Author:   David Register
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

#include <map>
#include <wx/string.h>
#include "bbox.h"
#include "tcmgr.h"


using namespace std;

class plugIn_Viewport;
class piDC;
class wxDC;

//----------------------------------------------------------------------------------------------------------
//    otcurrent Overlay Specification
//----------------------------------------------------------------------------------------------------------

class otcurrentOverlay {
public:
    otcurrentOverlay( void )
    {
        m_iTexture = 0;
        m_pDCBitmap = NULL, m_pRGBA = NULL;
    }

    ~otcurrentOverlay( void )

    
    {
       
        delete m_pDCBitmap, delete[] m_pRGBA;
    }

    double m_latoff, m_lonoff;

    unsigned int m_iTexture; /* opengl mode */

    wxBitmap *m_pDCBitmap; /* dc mode */
    unsigned char *m_pRGBA;

    int m_width;
    int m_height;
};

//----------------------------------------------------------------------------------------------------------
//    otcurrent Overlay Factory Specification
//----------------------------------------------------------------------------------------------------------

class otcurrentUIDialog;
class wxGLContext;
class otcurrentOverlayFactory;

class otcurrentOverlayFactory {
public:
    otcurrentOverlayFactory( otcurrentUIDialog &dlg );
    ~otcurrentOverlayFactory();

    void SetSettings( bool hiDefGraphics, bool GradualColors )
    {
      m_hiDefGraphics = hiDefGraphics;
      m_bGradualColors = GradualColors;
    }

    void SetMessage( wxString message ) { m_Message = message; }
    void SetParentSize( int w, int h ) { m_ParentSize.SetWidth(w) ; m_ParentSize.SetHeight(h) ;}
	bool RenderOverlay(piDC &dc, PlugIn_ViewPort &vp);

	void DrawAllCurrentsInViewPort(PlugIn_ViewPort *BBox, bool bRebuildSelList,
        bool bforce_redraw_currents, bool bdraw_mono_for_mask, wxDateTime myTime);

    void Reset();
	wxImage &DrawGLText( double value, int precision);
	wxImage &DrawGLTextDir( double value, int precision);
	wxImage &DrawGLTextString( wxString myText);

	void DrawLine(double x1, double y1, double x2, double y2, const wxColour &color, double width);


	bool              m_bShowRate;
    bool              m_bShowDirection;
	bool			  m_bHighResolution;
	bool              m_bShowFillColour;
	wxDateTime        m_dtUseNew;
	wxColour m_text_color;
	    std::map < double , wxImage > m_labelCache;
	std::map < wxString , wxImage > m_labelCacheText;
	
	piDC *m_dc;

private:

	bool inGL;
	
    bool DoRenderotcurrentOverlay( PlugIn_ViewPort *vp );
	void RenderMyArrows(PlugIn_ViewPort *vp );

    wxColour GetSpeedColour(double my_speed);

    bool drawCurrentArrow(int x, int y, double rot_angle, double scale, double rate );


    double m_last_vp_scale;

	//  for GL
	wxColour c_GLcolour;
	wxPoint p_basic[9];
	//
    wxString m_Message;
    wxString m_Message_Hiden;
    wxSize  m_ParentSize;

#if wxUSE_GRAPHICS_CONTEXT
    wxGraphicsContext *m_gdc;
#endif

    wxFont *m_dFont_map;
    wxFont *m_dFont_war;

    wxFont *pTCFont;

    bool m_hiDefGraphics;
    bool m_bGradualColors;

    
	wxImage m_fillImg;
	otcurrentUIDialog &m_dlg;
	void DrawGLLine( double x1, double y1, double x2, double y2, double width, wxColour myColour );


};

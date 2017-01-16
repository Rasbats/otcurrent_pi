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
        if(m_iTexture)
          glDeleteTextures( 1, &m_iTexture );
        delete m_pDCBitmap, delete[] m_pRGBA;
    }

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
    void SetTimeZone( int TimeZone ) { m_TimeZone = TimeZone; }
    void SetParentSize( int w, int h ) { m_ParentSize.SetWidth(w) ; m_ParentSize.SetHeight(h) ;}
	bool RenderGLotcurrentOverlay( wxGLContext *pcontext, PlugIn_ViewPort *vp );
    bool RenderotcurrentOverlay( wxDC &dc, PlugIn_ViewPort *vp );

	void DrawAllCurrentsInViewPort(PlugIn_ViewPort *BBox, bool bRebuildSelList,
        bool bforce_redraw_currents, bool bdraw_mono_for_mask, wxDateTime myTime);

    void Reset();
	wxImage &DrawGLText( double value, int precision);
	wxImage &DrawGLTextDir( double value, int precision);
	wxImage &DrawGLTextString( wxString myText);
	wxImage &DrawGLPolygon();

	void DrawGLLabels(otcurrentOverlayFactory *pof, wxDC *dc,
                               PlugIn_ViewPort *vp,
                               wxImage &imageLabel, double myLat, double myLon, int offset);

	void drawGLPolygons(otcurrentOverlayFactory *pof, wxDC *dc,
                                PlugIn_ViewPort *vp, 
                                wxImage &imageLabel, double myLat, double myLon, int offset );

	void DrawGLLine( double x1, double y1, double x2, double y2, double width, wxColour myColour );
    void DrawOLBitmap( const wxBitmap &bitmap, wxCoord x, wxCoord y, bool usemask );
	PlugIn_ViewPort *vp;
	bool              m_bShowRate;
    bool              m_bShowDirection;
	bool			  m_bHighResolution;
	bool              m_bShowFillColour;
	wxDateTime        m_dtUseNew;

private:

	bool inGL;
	
    bool DoRenderotcurrentOverlay( PlugIn_ViewPort *vp );
	void RenderMyArrows(PlugIn_ViewPort *vp );

    void DrawMessageWindow( wxString msg, int x, int y , wxFont *mfont);

    void drawWindArrowWithBarbs( int config, int x, int y, double vx, double vy,
                                 bool polar, bool south, wxColour arrowColor );
    void drawWaveArrow( int i, int j, double dir, wxColour arrowColor );

	wxColour GetSpeedColour(double my_speed);

	void drawCurrentArrow(int x, int y, double rot_angle, double scale, double rate );

    void drawSingleArrow( int i, int j, double dir, wxColour arrowColor, int width = 1 );

    void drawTransformedLine( wxPen pen, double si, double co, int di, int dj,
                              int i, int j, int k, int l );
    void drawPetiteBarbule( wxPen pen, bool south, double si, double co, int di, int dj, int b );
    void drawGrandeBarbule( wxPen pen, bool south, double si, double co, int di, int dj, int b );
    void drawTriangle( wxPen pen, bool south, double si, double co, int di, int dj, int b );

    double m_last_vp_scale;

	//  for GL
	wxColour c_GLcolour;
	wxPoint p_basic[9];
	//
    wxString m_Message;
    wxString m_Message_Hiden;
    int  m_TimeZone;
    wxSize  m_ParentSize;

    wxDC *m_pdc;
    wxGraphicsContext *m_gdc;

    wxFont *m_dFont_map;
    wxFont *m_dFont_war;

    bool m_hiDefGraphics;
    bool m_bGradualColors;

    std::map < double , wxImage > m_labelCache;
	std::map < wxString , wxImage > m_labelCacheText;

    otcurrentUIDialog &m_dlg;

    TCMgr *ctcmgr;
    wxBoundingBox *myBox;   
    LLBBox * myLLBox;

	wxString        *pTC_Dir;

};

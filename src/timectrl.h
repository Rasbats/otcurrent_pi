    /////////////////////////////////////////////////////////////////////////////
    // Name:        timectrl.h
    // Purpose:     TimePicker class
    // Author:      Christopher Hodges
    // Modified by:
    // Created:     6/15/2005
    // Copyright (c) Christopher Hodges
    /////////////////////////////////////////////////////////////////////////////
    #ifndef _TIME_CTRL_
    #define _TIME_CTRL_


    #include <wx/wx.h>
    #include <wx/wxprec.h>

    #ifdef __BORLANDC__
    #pragma hdrstop
    #endif

	

    #ifndef WX_PRECOMP
    // Include your minimal set of headers here, or wx.h
    #include <wx/wx.h>
    #endif

    #include<wx/spinbutt.h>
   // #include<wx/spinctrl.h>
    #include<wx/datetime.h>

    class wxTimeTextCtrl;

    class wxTimeTextCtrl : public wxTextCtrl
    {     
	public:

        wxTimeTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value = wxT(""),
             const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr);
        
		void OnArrowDown();
        void OnArrowUp();
        
		void spinUp(wxSpinEvent& event);
        void spinDown(wxSpinEvent& event);
        void OnKeyPress(wxKeyEvent&);

      protected:

        
        void ArrowLeft();
        void ArrowRight();

        DECLARE_EVENT_TABLE()
    };

    #endif

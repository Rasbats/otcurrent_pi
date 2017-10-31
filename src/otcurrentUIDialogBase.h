///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __otcurrentUIDIALOGBASE_H__
#define __otcurrentUIDIALOGBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/statline.h>
#include <wx/radiobox.h>
#include <wx/datectrl.h>
#include "wx/dateevt.h"
#include <wx/event.h>
#include <wx/listbox.h>
#include <wx/colordlg.h>
#include <wx/clrpicker.h>
#include <wx/filepicker.h>
#include <wx/event.h>


///////////////////////////////////////////////////////////////////////////

#define ID_TIMELINE 1000

///////////////////////////////////////////////////////////////////////////////
/// Class otcurrentUIDialogBase
///////////////////////////////////////////////////////////////////////////////
class otcurrentUIDialogBase : public wxDialog 
{
	private:
	
	protected:
		//wxBitmapButton* m_bpStart;
		wxBitmapButton* m_bpPrev;
		//wxStaticText* m_staticText24;
		wxChoice* m_choice2;
		
		wxDatePickerCtrl* m_datePicker1;

		
		wxBitmapButton* m_bpNext;
		wxBitmapButton* m_bpNow;

		wxButton* m_button33;
		wxBitmapButton* m_bpButton8;
		wxBitmapButton* m_bpButton9;
		
		wxTextCtrl* m_textCtrl1;
		 
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnDateSelChanged(wxDateEvent& event) { event.Skip(); }
		virtual void OnFolderSelChanged(wxFileDirPickerEvent& event) { event.Skip(); }
		
		
		virtual void OnNow( wxCommandEvent& event ) { event.Skip(); }
		virtual void About( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCalendarShow( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPrev( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNext( wxCommandEvent& event ) { event.Skip(); }
		virtual void SetInterval( wxCommandEvent& event ) { event.Skip(); }

		

	public:
		//virtual void ChangeColour1( wxCommandEvent& event ) { event.Skip(); }
		
		wxChoice* m_choice1;
		wxButton* m_button1;
		wxDirPickerCtrl* m_dirPicker1;
		
#ifdef __WXOSX__
		otcurrentUIDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Tidal Current Control"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxSTAY_ON_TOP );
#else
        otcurrentUIDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Tidal Current Control"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
#endif
		~otcurrentUIDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class otcurrentPreferencesDialogBase
///////////////////////////////////////////////////////////////////////////////
class otcurrentPreferencesDialogBase : public wxDialog 
{
	private:
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText41;
		wxStaticText* m_staticText42;
		wxStaticText* m_staticText43;
		wxStaticText* m_staticText44;
	
	protected:
		
		// Virtual event handlers, overide them in your derived class
		
	public:
		wxCheckBox* m_cbUseRate;
		wxCheckBox* m_cbUseDirection;
		wxCheckBox* m_cbUseHighRes;
		wxCheckBox* m_cbFillColour;

		wxButton* m_button111;
		wxButton* m_button2;
		wxButton* m_button3;

		wxColourPickerCtrl* myColourPicker0; 		
		wxColourPickerCtrl* myColourPicker1; 
		wxColourPickerCtrl* myColourPicker2; 
		wxColourPickerCtrl* myColourPicker3; 
		wxColourPickerCtrl* myColourPicker4; 

		otcurrentPreferencesDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
		~otcurrentPreferencesDialogBase();
	
};


#endif //__otcurrentUIDIALOGBASE_H__

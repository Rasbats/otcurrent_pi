///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb  6 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __OTCURRENTUIDIALOGBASE_H__
#define __OTCURRENTUIDIALOGBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/timectrl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/filepicker.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>

#ifdef __OCPN__ANDROID__
#include "android/wx/datectrl.h"

#else
class wxDatePickerCtrl;
#endif

#ifdef __OCPN__ANDROID__
#include "android/wx/timectrl.h"

#else
class wxTimePickerCtrl;
#endif

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class otcurrentUIDialogBase
///////////////////////////////////////////////////////////////////////////////
class otcurrentUIDialogBase : public wxDialog 
{
	private:
		wxButton* m_button1;
	
	protected:
		wxStaticText* m_staticTextDatetime;
		wxBitmapButton* m_bpNow;
		wxBitmapButton* m_bpPrev;
		wxBitmapButton* m_bpNext;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnDateTimeChanged( wxDateEvent& event ) { event.Skip(); }
		virtual void OnNow( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFolderSelChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void About( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPrev( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNext( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxDatePickerCtrl* m_datePickerDate;	    
		wxTimePickerCtrl* m_timePickerTime; 
		wxStaticText* m_stTimeZone;
		wxDirPickerCtrl* m_dirPicker1;
		wxChoice* m_choice1;
		
		otcurrentUIDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~otcurrentUIDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class otcurrentPreferencesDialogBase
///////////////////////////////////////////////////////////////////////////////
class otcurrentPreferencesDialogBase : public wxDialog 
{
	private:
		wxStaticText* staticTextLess05;
		wxStaticText* staticText0515;
		wxStaticText* staticText1525 ;
		wxStaticText* staticText2535;
		wxStaticText* m_staticTextMore35;
	
	protected:
		wxStaticText* m_staticText8;
		wxStdDialogButtonSizer* m_sdbSizerButtons;
		wxButton* m_sdbSizerButtonsOK;
		wxButton* m_sdbSizerButtonsCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChoice( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxCheckBox* m_cbUseRate;
		wxCheckBox* m_cbUseDirection;
		wxCheckBox* m_cbFillColour;
		wxCheckBox* m_cbUseHighRes;
		wxColourPickerCtrl* myColourPicker0;
		wxColourPickerCtrl* myColourPicker1;
		wxColourPickerCtrl* myColourPicker2;
		wxColourPickerCtrl* myColourPicker3;
		wxColourPickerCtrl* myColourPicker4;
		wxChoice* m_cScale;
		
		otcurrentPreferencesDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 410,440 ), long style = wxCAPTION|wxRESIZE_BORDER ); 
		~otcurrentPreferencesDialogBase();
	
};

#endif //__OTCURRENTUIDIALOGBASE_H__

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "otcurrentUIDialogBase.h"
#include "otcurrentUIDialog.h"

///////////////////////////////////////////////////////////////////////////

otcurrentUIDialogBase::otcurrentUIDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY|wxEXPAND, _("Date/Time") ), wxHORIZONTAL );

	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition,  wxSize( 170,20 ), wxTE_READONLY|wxSIMPLE_BORDER );
	sbSizer2->Add( m_textCtrl1, 100, wxALL|wxEXPAND, 5 );
	
	m_button33 = new wxButton( this, wxID_ANY , _("Select"), wxDefaultPosition, wxSize( 80,-1 ), 0 );
	m_button33->SetToolTip(_("Select date/time for current prediction"));	
	sbSizer2->Add( m_button33, 0, wxALL, 5 );

	m_bpNow = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_bpNow->SetToolTip( _("Now") );
	sbSizer2->Add( m_bpNow, 0, wxALL, 1 );
	bSizer1->Add( sbSizer2, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Tidal Data Folder") ), wxVERTICAL );

	m_dirPicker1 = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_CHANGE_DIR|wxDIRP_USE_TEXTCTRL );
	sbSizer4->Add( m_dirPicker1, 0, wxALL|wxEXPAND, 5 );

	bSizer1->Add( sbSizer4, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Step (Minutes)") ), wxHORIZONTAL );
	
	sbSizer3->SetMinSize( wxSize( -1,30 ) ); 
	
	wxArrayString m_choice1Choices;
	m_choice1Choices.Add(_T("60"));
	m_choice1Choices.Add(_T("30"));
	m_choice1Choices.Add(_T("15"));
	m_choice1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1Choices, 0);	
	m_choice1->SetSelection( 0 );

	sbSizer3->Add( m_choice1, 0, wxALL, 5 );

	sbSizer3->Add( 5, 0, 1, wxEXPAND, 5 );


	m_button1 = new wxButton( this, wxID_ANY, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer3->Add( m_button1, 0, wxALL, 5 );	

	m_bpPrev = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 30,-1 ), 0  );
	m_bpPrev->SetToolTip( _("Previous") );	
	sbSizer3->Add( m_bpPrev, 0, wxALL, 1 );

	m_bpNext = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 30,-1 ), 0  );
	m_bpNext->SetToolTip( _("Next") );
	sbSizer3->Add( m_bpNext, 0, wxALL, 1 );

	bSizer1->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( otcurrentUIDialogBase::OnClose ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( otcurrentUIDialogBase::OnSize ) );

	m_bpNow->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNow ), NULL, this );
	m_button33->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnCalendarShow ), NULL, this );

	m_bpPrev->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnPrev ), NULL, this );
	m_bpNext->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNext ), NULL, this );

	m_dirPicker1->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( otcurrentUIDialogBase::OnFolderSelChanged ), NULL, this );
 	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::About ), NULL, this );


}

otcurrentUIDialogBase::~otcurrentUIDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( otcurrentUIDialogBase::OnClose ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( otcurrentUIDialogBase::OnSize ) );
	
	m_bpNow->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNow ), NULL, this );
	m_button33->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnCalendarShow ), NULL, this );

	m_bpPrev->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnPrev ), NULL, this );
	m_bpNext->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNext ), NULL, this );

	m_dirPicker1->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( otcurrentUIDialogBase::OnFolderSelChanged ), NULL, this );
    m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::About ), NULL, this );
	
}

otcurrentPreferencesDialogBase::otcurrentPreferencesDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	 
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	m_cbUseRate = new wxCheckBox( this, wxID_ANY, _("Display tidal rates (knots)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbUseRate, 0, wxALL, 5 );
	
	m_cbUseDirection = new wxCheckBox( this, wxID_ANY, _("Display tidal direction"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbUseDirection, 0, wxALL, 5 );
	
	m_cbFillColour = new wxCheckBox( this, wxID_ANY, _("Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_cbFillColour, 0, wxALL, 5 );

	fgSizer8->Add( fgSizer6, 1, wxEXPAND, 5 );
		
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Tidal Current Arrow Colour") ), wxVERTICAL );
	
	myColourPicker0 = new wxColourPickerCtrl(this, wxID_ANY, *wxRED,
    wxDefaultPosition, wxDefaultSize, 0);
	
	myColourPicker1 = new wxColourPickerCtrl(this, wxID_ANY, *wxRED,
    wxDefaultPosition, wxDefaultSize, 0);

	myColourPicker2 = new wxColourPickerCtrl(this, wxID_ANY, *wxRED,
    wxDefaultPosition, wxDefaultSize, 0);

	myColourPicker3 = new wxColourPickerCtrl(this, wxID_ANY, *wxRED,
    wxDefaultPosition, wxDefaultSize, 0);

	myColourPicker4 = new wxColourPickerCtrl(this, wxID_ANY, *wxRED,
    wxDefaultPosition, wxDefaultSize, 0);
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 0, 2, 0, 0 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, _("      < 0.5 knots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	gSizer1->Add( m_staticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	gSizer1->Add( myColourPicker0, 0, wxALL, 5 );	
	
	m_staticText41 = new wxStaticText( this, wxID_ANY, _(">= 0.5 and < 1.5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	gSizer1->Add( m_staticText41, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	gSizer1->Add( myColourPicker1, 0, wxALL, 5 );
	
	m_staticText42 = new wxStaticText( this, wxID_ANY, _(">= 1.5 and < 2.5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText42->Wrap( -1 );
	gSizer1->Add( m_staticText42, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	gSizer1->Add( myColourPicker2, 0, wxALL, 5 );
	
	m_staticText43 = new wxStaticText( this, wxID_ANY, _(">= 2.5 and < 3.5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText43->Wrap( -1 );
    gSizer1->Add( m_staticText43, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
    gSizer1->Add( myColourPicker3, 0, wxALL, 5 );
	
	m_staticText44 = new wxStaticText( this, wxID_ANY, _(">= 3.5 knots    "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	gSizer1->Add( m_staticText44, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	gSizer1->Add( myColourPicker4, 0, wxALL, 5 );
	
    sbSizer1->Add( gSizer1, 1, wxEXPAND, 5 );
	bSizer2->Add( sbSizer1, 1, wxEXPAND, 5 );

	fgSizer8->Add(bSizer2, 1, wxALL|wxEXPAND, 5 );

	wxStdDialogButtonSizer* m_sdbSizer2;
	
	wxButton* m_sdbSizer2OK;
	wxButton* m_sdbSizer2Cancel;
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();
	
	fgSizer8->Add( m_sdbSizer2, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( fgSizer8 );
	this->Layout();
	fgSizer8->Fit( this );
	
	this->Centre( wxBOTH );

}

otcurrentPreferencesDialogBase::~otcurrentPreferencesDialogBase()
{


	
}

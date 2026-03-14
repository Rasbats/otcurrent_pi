///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "otcurrentUIDialogBase.h"

///////////////////////////////////////////////////////////////////////////

otcurrentUIDialogBase::otcurrentUIDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerDateTime;
	sbSizerDateTime = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Date/Time") ), wxHORIZONTAL );

	m_textCtrl1 = new wxTextCtrl( sbSizerDateTime->GetStaticBox(), wxID_ANY, _("Tue 15 Aug 2023 08:00"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerDateTime->Add( m_textCtrl1, 1, wxALIGN_CENTER_VERTICAL, 5 );

	m_bpPrev = new wxBitmapButton( sbSizerDateTime->GetStaticBox(), wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( -1,-1 ), wxBU_AUTODRAW|0 );
	m_bpPrev->SetToolTip( _("Previous") );
	m_bpPrev->SetMinSize( wxSize( 40,40 ) );

	sbSizerDateTime->Add( m_bpPrev, 0, wxALL|wxEXPAND, 5 );

	m_bpNext = new wxBitmapButton( sbSizerDateTime->GetStaticBox(), wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( -1,-1 ), wxBU_AUTODRAW|0 );
	m_bpNext->SetToolTip( _("Next") );
	m_bpNext->SetMinSize( wxSize( 40,40 ) );

	sbSizerDateTime->Add( m_bpNext, 0, wxALL|wxEXPAND, 5 );


	bSizerMain->Add( sbSizerDateTime, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );

	m_button33 = new wxButton( this, wxID_ANY, _(" Select Date/Time"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button33, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText10 = new wxStaticText( this, wxID_ANY, _("Step\n(Mins)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer6->Add( m_staticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

	wxString m_choice1Choices[] = { _("60"), _("30"), _("15") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 0 );
	bSizer6->Add( m_choice1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_bpNow = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	m_bpNow->SetToolTip( _("Now") );
	m_bpNow->SetMinSize( wxSize( 45,45 ) );

	bSizer6->Add( m_bpNow, 0, wxALL|wxEXPAND, 5 );


	bSizerMain->Add( bSizer6, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizerFolder;
	sbSizerFolder = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Tidal Data Folder") ), wxVERTICAL );

	m_dirPicker1 = new wxTextCtrl( sbSizerFolder->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerFolder->Add( m_dirPicker1, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_button2 = new wxButton( sbSizerFolder->GetStaticBox(), wxID_ANY, _("Select tidal current data"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button2, 0, wxALL, 5 );


	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	m_button1 = new wxButton( sbSizerFolder->GetStaticBox(), wxID_ANY, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	sbSizerFolder->Add( bSizer5, 0, wxEXPAND, 5 );


	bSizerMain->Add( sbSizerFolder, 0, wxEXPAND, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();
	bSizerMain->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( otcurrentUIDialogBase::OnClose ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( otcurrentUIDialogBase::OnSize ) );
	m_bpPrev->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnPrev ), NULL, this );
	m_bpNext->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNext ), NULL, this );
	m_button33->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnCalendarShow ), NULL, this );
	m_bpNow->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNow ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnSelectData ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::About ), NULL, this );
}

otcurrentUIDialogBase::~otcurrentUIDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( otcurrentUIDialogBase::OnClose ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( otcurrentUIDialogBase::OnSize ) );
	m_bpPrev->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnPrev ), NULL, this );
	m_bpNext->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNext ), NULL, this );
	m_button33->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnCalendarShow ), NULL, this );
	m_bpNow->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNow ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnSelectData ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::About ), NULL, this );

}

otcurrentPreferencesDialogBase::otcurrentPreferencesDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,400 ), wxDefaultSize );

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerCheckboxes;
	bSizerCheckboxes = new wxBoxSizer( wxVERTICAL );

	m_cbUseRate = new wxCheckBox( this, wxID_ANY, _("Display tidal rates (knots)"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerCheckboxes->Add( m_cbUseRate, 0, wxALL, 5 );

	m_cbUseDirection = new wxCheckBox( this, wxID_ANY, _("Display tidal direction"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerCheckboxes->Add( m_cbUseDirection, 0, wxALL, 5 );

	m_cbFillColour = new wxCheckBox( this, wxID_ANY, _("Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerCheckboxes->Add( m_cbFillColour, 0, wxALL, 5 );

	m_cbUseHighRes = new wxCheckBox( this, wxID_ANY, _("High Resolution Display"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerCheckboxes->Add( m_cbUseHighRes, 0, wxALL, 5 );


	bSizerMain->Add( bSizerCheckboxes, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* sbSizerColours;
	sbSizerColours = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Tidal Current Arrow Colour") ), wxVERTICAL );

	wxFlexGridSizer* fgSizerColours;
	fgSizerColours = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerColours->AddGrowableCol( 1 );
	fgSizerColours->SetFlexibleDirection( wxBOTH );
	fgSizerColours->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	staticTextLess05 = new wxStaticText( sbSizerColours->GetStaticBox(), wxID_ANY, _("      < 0.5 knots"), wxDefaultPosition, wxDefaultSize, 0 );
	staticTextLess05->Wrap( -1 );
	fgSizerColours->Add( staticTextLess05, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	myColourPicker0 = new wxColourPickerCtrl( sbSizerColours->GetStaticBox(), wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerColours->Add( myColourPicker0, 1, wxALL|wxEXPAND, 5 );

	staticText0515 = new wxStaticText( sbSizerColours->GetStaticBox(), wxID_ANY, _(">= 0.5 and < 1.5"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText0515->Wrap( -1 );
	fgSizerColours->Add( staticText0515, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	myColourPicker1 = new wxColourPickerCtrl( sbSizerColours->GetStaticBox(), wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerColours->Add( myColourPicker1, 1, wxALL|wxEXPAND, 5 );

	staticText1525  = new wxStaticText( sbSizerColours->GetStaticBox(), wxID_ANY, _(">= 1.5 and < 2.5"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText1525 ->Wrap( -1 );
	fgSizerColours->Add( staticText1525 , 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	myColourPicker2 = new wxColourPickerCtrl( sbSizerColours->GetStaticBox(), wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerColours->Add( myColourPicker2, 1, wxALL|wxEXPAND, 5 );

	staticText2535 = new wxStaticText( sbSizerColours->GetStaticBox(), wxID_ANY, _(">= 2.5 and < 3.5"), wxDefaultPosition, wxDefaultSize, 0 );
	staticText2535->Wrap( -1 );
	fgSizerColours->Add( staticText2535, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	myColourPicker3 = new wxColourPickerCtrl( sbSizerColours->GetStaticBox(), wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerColours->Add( myColourPicker3, 1, wxALL|wxEXPAND, 5 );

	m_staticTextMore35 = new wxStaticText( sbSizerColours->GetStaticBox(), wxID_ANY, _(">= 3.5 knots    "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMore35->Wrap( -1 );
	fgSizerColours->Add( m_staticTextMore35, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	myColourPicker4 = new wxColourPickerCtrl( sbSizerColours->GetStaticBox(), wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerColours->Add( myColourPicker4, 1, wxALL|wxEXPAND, 5 );


	sbSizerColours->Add( fgSizerColours, 1, wxEXPAND, 5 );


	bSizer4->Add( sbSizerColours, 1, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Arrow Scaling") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText8 = new wxStaticText( sbSizer5->GetStaticBox(), wxID_ANY, _("Scale   x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer2->Add( m_staticText8, 0, wxALL, 5 );

	wxString m_cScaleChoices[] = { _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9"), _("10") };
	int m_cScaleNChoices = sizeof( m_cScaleChoices ) / sizeof( wxString );
	m_cScale = new wxChoice( sbSizer5->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cScaleNChoices, m_cScaleChoices, 0 );
	m_cScale->SetSelection( 0 );
	fgSizer2->Add( m_cScale, 0, wxALL|wxEXPAND, 5 );


	sbSizer5->Add( fgSizer2, 1, wxEXPAND, 5 );


	bSizer4->Add( sbSizer5, 1, wxALL|wxEXPAND, 5 );


	bSizerMain->Add( bSizer4, 1, wxEXPAND, 5 );

	m_sdbSizerButtons = new wxStdDialogButtonSizer();
	m_sdbSizerButtonsOK = new wxButton( this, wxID_OK );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsOK );
	m_sdbSizerButtonsCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizerButtons->AddButton( m_sdbSizerButtonsCancel );
	m_sdbSizerButtons->Realize();

	bSizerMain->Add( m_sdbSizerButtons, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_cScale->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( otcurrentPreferencesDialogBase::OnChoice ), NULL, this );
}

otcurrentPreferencesDialogBase::~otcurrentPreferencesDialogBase()
{
	// Disconnect Events
	m_cScale->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( otcurrentPreferencesDialogBase::OnChoice ), NULL, this );

}

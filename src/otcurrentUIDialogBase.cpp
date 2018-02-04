///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 25 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "otcurrentUIDialogBase.h"

///////////////////////////////////////////////////////////////////////////

otcurrentUIDialogBase::otcurrentUIDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,300 ), wxDefaultSize );
	
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizerDateTime;
	sbSizerDateTime = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Date/Time") ), wxHORIZONTAL );
	
	m_staticTextDatetime = new wxStaticText( sbSizerDateTime->GetStaticBox(), wxID_ANY, _("Day"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDatetime->Wrap( -1 );
	sbSizerDateTime->Add( m_staticTextDatetime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_datePickerDate = new wxDatePickerCtrl( sbSizerDateTime->GetStaticBox(), wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	sbSizerDateTime->Add( m_datePickerDate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_timePickerTime = new wxTimePickerCtrl( sbSizerDateTime->GetStaticBox(), wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerDateTime->Add( m_timePickerTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sbSizerDateTime->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_bpNow = new wxBitmapButton( sbSizerDateTime->GetStaticBox(), wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_bpNow->SetToolTip( _("Now") );
	m_bpNow->SetMinSize( wxSize( 45,45 ) );
	
	sbSizerDateTime->Add( m_bpNow, 0, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	
	bSizerMain->Add( sbSizerDateTime, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizerFolder;
	sbSizerFolder = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Tidal Data Folder") ), wxVERTICAL );
	
	m_dirPicker1 = new wxDirPickerCtrl( sbSizerFolder->GetStaticBox(), wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_CHANGE_DIR|wxDIRP_USE_TEXTCTRL );
	sbSizerFolder->Add( m_dirPicker1, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizerMain->Add( sbSizerFolder, 0, wxEXPAND, 5 );
	
	
	bSizerMain->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizerControls;
	sbSizerControls = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Step (Minutes)") ), wxHORIZONTAL );
	
	sbSizerControls->SetMinSize( wxSize( -1,30 ) ); 
	wxString m_choice1Choices[] = { _("60"), _("30"), _("15") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( sbSizerControls->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 0 );
	sbSizerControls->Add( m_choice1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	sbSizerControls->Add( 5, 0, 1, wxEXPAND, 5 );
	
	m_button1 = new wxButton( sbSizerControls->GetStaticBox(), wxID_ANY, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerControls->Add( m_button1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_bpPrev = new wxBitmapButton( sbSizerControls->GetStaticBox(), wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( -1,-1 ), wxBU_AUTODRAW );
	m_bpPrev->SetToolTip( _("Previous") );
	m_bpPrev->SetMinSize( wxSize( 40,40 ) );
	
	sbSizerControls->Add( m_bpPrev, 0, wxALL|wxEXPAND, 5 );
	
	m_bpNext = new wxBitmapButton( sbSizerControls->GetStaticBox(), wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( -1,-1 ), wxBU_AUTODRAW );
	m_bpNext->SetToolTip( _("Next") );
	m_bpNext->SetMinSize( wxSize( 40,40 ) );
	
	sbSizerControls->Add( m_bpNext, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizerMain->Add( sbSizerControls, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( otcurrentUIDialogBase::OnClose ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( otcurrentUIDialogBase::OnSize ) );
	m_datePickerDate->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( otcurrentUIDialogBase::OnDateTimeChanged ), NULL, this );
	m_bpNow->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNow ), NULL, this );
	m_dirPicker1->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( otcurrentUIDialogBase::OnFolderSelChanged ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::About ), NULL, this );
	m_bpPrev->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnPrev ), NULL, this );
	m_bpNext->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNext ), NULL, this );
}

otcurrentUIDialogBase::~otcurrentUIDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( otcurrentUIDialogBase::OnClose ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( otcurrentUIDialogBase::OnSize ) );
	m_datePickerDate->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( otcurrentUIDialogBase::OnDateTimeChanged ), NULL, this );
	m_bpNow->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNow ), NULL, this );
	m_dirPicker1->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( otcurrentUIDialogBase::OnFolderSelChanged ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::About ), NULL, this );
	m_bpPrev->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnPrev ), NULL, this );
	m_bpNext->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( otcurrentUIDialogBase::OnNext ), NULL, this );
	
}

otcurrentPreferencesDialogBase::otcurrentPreferencesDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 280,480 ), wxDefaultSize );
	
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
	
	
	bSizerMain->Add( sbSizerColours, 1, wxALL|wxEXPAND, 5 );
	
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
}

otcurrentPreferencesDialogBase::~otcurrentPreferencesDialogBase()
{
}

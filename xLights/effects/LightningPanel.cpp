#include "LightningPanel.h"
#include "../../include/padlock16x16-blue.xpm"
#include "EffectPanelUtils.h"

//(*InternalHeaders(LightningPanel)
#include <wx/bmpbuttn.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/slider.h>
#include <wx/intl.h>
#include <wx/stattext.h>
#include <wx/bitmap.h>
#include <wx/textctrl.h>
#include <wx/image.h>
//*)

//(*IdInit(LightningPanel)
const long LightningPanel::ID_STATICTEXT25 = wxNewId();
const long LightningPanel::ID_SLIDER_Number_Bolts = wxNewId();
const long LightningPanel::IDD_TEXTCTRL_Number_Bolts = wxNewId();
const long LightningPanel::ID_BITMAPBUTTON38 = wxNewId();
const long LightningPanel::ID_STATICTEXT26 = wxNewId();
const long LightningPanel::ID_SLIDER_Number_Segments = wxNewId();
const long LightningPanel::IDD_TEXTCTRL_Number_Segments = wxNewId();
const long LightningPanel::ID_BITMAPBUTTON40 = wxNewId();
const long LightningPanel::ID_STATICTEXT60 = wxNewId();
const long LightningPanel::ID_CHECKBOX_ForkedLightning = wxNewId();
const long LightningPanel::ID_BITMAPBUTTON41 = wxNewId();
const long LightningPanel::ID_STATICTEXT64 = wxNewId();
const long LightningPanel::ID_SLIDER_Lightning_TopX = wxNewId();
const long LightningPanel::IDD_TEXTCTRL_Lightning_TopX = wxNewId();
const long LightningPanel::ID_BITMAPBUTTON42 = wxNewId();
const long LightningPanel::ID_STATICTEXT67 = wxNewId();
const long LightningPanel::ID_SLIDER_Lightning_TopY = wxNewId();
const long LightningPanel::IDD_TEXTCTRL_Lightning_TopY = wxNewId();
const long LightningPanel::ID_BITMAPBUTTON43 = wxNewId();
const long LightningPanel::ID_STATICTEXT70 = wxNewId();
const long LightningPanel::ID_SLIDER_Lightning_BOTX = wxNewId();
const long LightningPanel::IDD_TEXTCTRL_Lightning_BOTX = wxNewId();
const long LightningPanel::ID_BITMAPBUTTON45 = wxNewId();
const long LightningPanel::ID_STATICTEXT73 = wxNewId();
const long LightningPanel::ID_SLIDER_Lightning_BOTY = wxNewId();
const long LightningPanel::IDD_TEXTCTRL_Lightning_BOTY = wxNewId();
const long LightningPanel::ID_BITMAPBUTTON52 = wxNewId();
//*)

BEGIN_EVENT_TABLE(LightningPanel,wxPanel)
	//(*EventTable(LightningPanel)
	//*)
END_EVENT_TABLE()

LightningPanel::LightningPanel(wxWindow* parent)
{
	//(*Initialize(LightningPanel)
	wxTextCtrl* TextCtrl_Number_Segments;
	wxSlider* Number_Bolts;
	wxTextCtrl* TextCtrl_Number_Bolts;
	wxSlider* Number_Segments;
	wxFlexGridSizer* FlexGridSizer140;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer140 = new wxFlexGridSizer(0, 4, 0, 0);
	FlexGridSizer140->AddGrowableCol(1);
	StaticText203 = new wxStaticText(this, ID_STATICTEXT25, _("Number of Segments"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT25"));
	FlexGridSizer140->Add(StaticText203, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Number_Bolts = new wxSlider(this, ID_SLIDER_Number_Bolts, 10, 1, 50, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Number_Bolts"));
	FlexGridSizer140->Add(Number_Bolts, 1, wxALL|wxEXPAND, 5);
	TextCtrl_Number_Bolts = new wxTextCtrl(this, IDD_TEXTCTRL_Number_Bolts, _("10"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Number_Bolts"));
	TextCtrl_Number_Bolts->SetMaxLength(3);
	FlexGridSizer140->Add(TextCtrl_Number_Bolts, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton20 = new wxBitmapButton(this, ID_BITMAPBUTTON38, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON38"));
	BitmapButton20->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer140->Add(BitmapButton20, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText205 = new wxStaticText(this, ID_STATICTEXT26, _("Width of Bolt"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT26"));
	FlexGridSizer140->Add(StaticText205, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Number_Segments = new wxSlider(this, ID_SLIDER_Number_Segments, 5, 1, 20, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Number_Segments"));
	FlexGridSizer140->Add(Number_Segments, 1, wxALL|wxEXPAND, 5);
	TextCtrl_Number_Segments = new wxTextCtrl(this, IDD_TEXTCTRL_Number_Segments, _("5"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Number_Segments"));
	TextCtrl_Number_Segments->SetMaxLength(4);
	FlexGridSizer140->Add(TextCtrl_Number_Segments, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton22 = new wxBitmapButton(this, ID_BITMAPBUTTON40, padlock16x16_blue_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON40"));
	BitmapButton22->SetMinSize(wxSize(13,13));
	BitmapButton22->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer140->Add(BitmapButton22, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText206 = new wxStaticText(this, ID_STATICTEXT60, _("Forked Lightning"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT60"));
	FlexGridSizer140->Add(StaticText206, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ForkedLightning = new wxCheckBox(this, ID_CHECKBOX_ForkedLightning, _("Yes"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_ForkedLightning"));
	ForkedLightning->SetValue(false);
	FlexGridSizer140->Add(ForkedLightning, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer140->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton23 = new wxBitmapButton(this, ID_BITMAPBUTTON41, padlock16x16_blue_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON41"));
	BitmapButton23->SetMinSize(wxSize(13,13));
	BitmapButton23->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer140->Add(BitmapButton23, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText207 = new wxStaticText(this, ID_STATICTEXT64, _("Top X Location"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT64"));
	FlexGridSizer140->Add(StaticText207, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Lightning_TopX = new wxSlider(this, ID_SLIDER_Lightning_TopX, 0, -50, 50, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Lightning_TopX"));
	FlexGridSizer140->Add(Lightning_TopX, 1, wxALL|wxEXPAND, 5);
	TextCtrl86 = new wxTextCtrl(this, IDD_TEXTCTRL_Lightning_TopX, _("0"), wxDefaultPosition, wxSize(20,-1), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Lightning_TopX"));
	FlexGridSizer140->Add(TextCtrl86, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton25 = new wxBitmapButton(this, ID_BITMAPBUTTON42, padlock16x16_blue_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON42"));
	BitmapButton25->SetMinSize(wxSize(13,13));
	BitmapButton25->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer140->Add(BitmapButton25, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText208 = new wxStaticText(this, ID_STATICTEXT67, _("Top Y Location"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT67"));
	FlexGridSizer140->Add(StaticText208, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Lightning_TopY = new wxSlider(this, ID_SLIDER_Lightning_TopY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Lightning_TopY"));
	FlexGridSizer140->Add(Slider_Lightning_TopY, 1, wxALL|wxEXPAND, 5);
	TextCtrl88 = new wxTextCtrl(this, IDD_TEXTCTRL_Lightning_TopY, _("0"), wxDefaultPosition, wxSize(20,-1), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Lightning_TopY"));
	FlexGridSizer140->Add(TextCtrl88, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton26 = new wxBitmapButton(this, ID_BITMAPBUTTON43, padlock16x16_blue_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON43"));
	BitmapButton26->SetMinSize(wxSize(13,13));
	BitmapButton26->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer140->Add(BitmapButton26, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText209 = new wxStaticText(this, ID_STATICTEXT70, _("X Movement"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT70"));
	FlexGridSizer140->Add(StaticText209, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Slider_Lightning_BOTX = new wxSlider(this, ID_SLIDER_Lightning_BOTX, 0, -50, 50, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Lightning_BOTX"));
	FlexGridSizer140->Add(Slider_Lightning_BOTX, 1, wxALL|wxEXPAND, 5);
	TextCtrl89 = new wxTextCtrl(this, IDD_TEXTCTRL_Lightning_BOTX, _("0"), wxDefaultPosition, wxSize(20,-1), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Lightning_BOTX"));
	FlexGridSizer140->Add(TextCtrl89, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton27 = new wxBitmapButton(this, ID_BITMAPBUTTON45, padlock16x16_blue_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON45"));
	BitmapButton27->SetMinSize(wxSize(13,13));
	BitmapButton27->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer140->Add(BitmapButton27, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText210 = new wxStaticText(this, ID_STATICTEXT73, _("(Not Used yet)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT73"));
	FlexGridSizer140->Add(StaticText210, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	Lightning_BOTX = new wxSlider(this, ID_SLIDER_Lightning_BOTY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_Lightning_BOTY"));
	FlexGridSizer140->Add(Lightning_BOTX, 1, wxALL|wxEXPAND, 5);
	TextCtrl90 = new wxTextCtrl(this, IDD_TEXTCTRL_Lightning_BOTY, _("0"), wxDefaultPosition, wxSize(20,-1), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_Lightning_BOTY"));
	FlexGridSizer140->Add(TextCtrl90, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton28 = new wxBitmapButton(this, ID_BITMAPBUTTON52, padlock16x16_blue_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON52"));
	BitmapButton28->SetMinSize(wxSize(13,13));
	BitmapButton28->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	FlexGridSizer140->Add(BitmapButton28, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer140);
	FlexGridSizer140->Fit(this);
	FlexGridSizer140->SetSizeHints(this);

	Connect(ID_SLIDER_Number_Bolts,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_Number_Bolts,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON38,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LightningPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Number_Segments,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_Number_Segments,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON40,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LightningPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON41,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LightningPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Lightning_TopX,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_Lightning_TopX,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON42,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LightningPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Lightning_TopY,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_Lightning_TopY,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON43,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LightningPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Lightning_BOTX,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_Lightning_BOTX,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON45,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LightningPanel::OnLockButtonClick);
	Connect(ID_SLIDER_Lightning_BOTY,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_Lightning_BOTY,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&LightningPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON52,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&LightningPanel::OnLockButtonClick);
	//*)
    SetName("ID_PANEL_LIGHTNING");
}

LightningPanel::~LightningPanel()
{
	//(*Destroy(LightningPanel)
	//*)
}

PANEL_EVENT_HANDLERS(LightningPanel)
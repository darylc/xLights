#include "BufferPanel.h"

//(*InternalHeaders(BufferPanel)
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/intl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
//*)

#include <vector>
#include "models/Model.h"
#include "effects/EffectPanelUtils.h"
#include "../include/padlock16x16-blue.xpm" //-DJ

#include "SubBufferPanel.h"

//(*IdInit(BufferPanel)
const long BufferPanel::ID_CHOICE_BufferStyle = wxNewId();
const long BufferPanel::ID_BITMAPBUTTON_CHOICE_BufferStyle = wxNewId();
const long BufferPanel::ID_CHOICE_BufferTransform = wxNewId();
const long BufferPanel::ID_BITMAPBUTTON_CHOICE_BufferTransform = wxNewId();
const long BufferPanel::ID_STATICTEXT2 = wxNewId();
const long BufferPanel::ID_SLIDER_EffectBlur = wxNewId();
const long BufferPanel::IDD_TEXTCTRL_EffectBlur = wxNewId();
const long BufferPanel::ID_BITMAPBUTTON_SLIDER_EffectBlur = wxNewId();
const long BufferPanel::ID_CHECKBOX_OverlayBkg = wxNewId();
const long BufferPanel::ID_BITMAPBUTTON_OverlayBkg = wxNewId();
const long BufferPanel::ID_SCROLLED_ColorScroll = wxNewId();
const long BufferPanel::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(BufferPanel,wxPanel)
	//(*EventTable(BufferPanel)
	//*)
END_EVENT_TABLE()

BufferPanel::BufferPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(BufferPanel)
	wxBitmapButton* BitmapButtonBufferStyle;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer3;
	wxStaticText* StaticText4;
	wxStaticText* StaticText2;
	wxBitmapButton* BitmapButton1;

	Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(1, 1, 0, 0);
	Panel_Sizer = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	FlexGridSizer3 = new wxFlexGridSizer(0, 1, 0, 0);
	BufferScrollWindow = new wxScrolledWindow(Panel_Sizer, ID_SCROLLED_ColorScroll, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxHSCROLL, _T("ID_SCROLLED_ColorScroll"));
	FlexGridSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer4->AddGrowableCol(0);
	FlexGridSizer4->AddGrowableRow(1);
	Sizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	Sizer2->AddGrowableCol(1);
	StaticText4 = new wxStaticText(BufferScrollWindow, wxID_ANY, _("Render Style"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	Sizer2->Add(StaticText4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BufferStyleChoice = new wxChoice(BufferScrollWindow, ID_CHOICE_BufferStyle, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_BufferStyle"));
	BufferStyleChoice->SetSelection( BufferStyleChoice->Append(_("Default")) );
	BufferStyleChoice->Append(_("Per Preview"));
	Sizer2->Add(BufferStyleChoice, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButtonBufferStyle = new wxBitmapButton(BufferScrollWindow, ID_BITMAPBUTTON_CHOICE_BufferStyle, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHOICE_BufferStyle"));
	BitmapButtonBufferStyle->SetDefault();
	BitmapButtonBufferStyle->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	BitmapButtonBufferStyle->SetToolTip(_("Lock/Unlock. If Locked then a \"Create Random Effects\" will NOT change this value."));
	Sizer2->Add(BitmapButtonBufferStyle, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(BufferScrollWindow, wxID_ANY, _("Transformation"), wxDefaultPosition, wxDefaultSize, 0, _T("wxID_ANY"));
	Sizer2->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BufferTransform = new wxChoice(BufferScrollWindow, ID_CHOICE_BufferTransform, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE_BufferTransform"));
	BufferTransform->SetSelection( BufferTransform->Append(_("None")) );
	BufferTransform->Append(_("Rotate CC 90"));
	BufferTransform->Append(_("Rotate CW 90"));
	BufferTransform->Append(_("Rotate 180"));
	BufferTransform->Append(_("Flip Vertical"));
	BufferTransform->Append(_("Flip Horizontal"));
	Sizer2->Add(BufferTransform, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BitmapButton1 = new wxBitmapButton(BufferScrollWindow, ID_BITMAPBUTTON_CHOICE_BufferTransform, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_CHOICE_BufferTransform"));
	BitmapButton1->SetDefault();
	BitmapButton1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	BitmapButton1->SetToolTip(_("Lock/Unlock. If Locked then a \"Create Random Effects\" will NOT change this value."));
	Sizer2->Add(BitmapButton1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(BufferScrollWindow, ID_STATICTEXT2, _("Blur"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	Sizer2->Add(StaticText5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer6 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer6->AddGrowableCol(0);
	Slider_EffectBlur = new wxSlider(BufferScrollWindow, ID_SLIDER_EffectBlur, 1, 1, 15, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SLIDER_EffectBlur"));
	FlexGridSizer6->Add(Slider_EffectBlur, 1, wxALL|wxEXPAND, 1);
	TextCtrl_EffectBlur = new wxTextCtrl(BufferScrollWindow, IDD_TEXTCTRL_EffectBlur, _("1"), wxDefaultPosition, wxDLG_UNIT(BufferScrollWindow,wxSize(20,-1)), 0, wxDefaultValidator, _T("IDD_TEXTCTRL_EffectBlur"));
	TextCtrl_EffectBlur->SetMaxLength(2);
	FlexGridSizer6->Add(TextCtrl_EffectBlur, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	Sizer2->Add(FlexGridSizer6, 1, wxALL|wxEXPAND, 2);
	BitmapButton_EffectBlur = new wxBitmapButton(BufferScrollWindow, ID_BITMAPBUTTON_SLIDER_EffectBlur, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_SLIDER_EffectBlur"));
	BitmapButton_EffectBlur->SetDefault();
	BitmapButton_EffectBlur->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	BitmapButton_EffectBlur->SetToolTip(_("Lock/Unlock. If Locked then a \"Create Random Effects\" will NOT change this value."));
	Sizer2->Add(BitmapButton_EffectBlur, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	CheckBox_OverlayBkg = new wxCheckBox(BufferScrollWindow, ID_CHECKBOX_OverlayBkg, _("Persistent"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX_OverlayBkg"));
	CheckBox_OverlayBkg->SetValue(false);
	Sizer2->Add(CheckBox_OverlayBkg, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Sizer2->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BitmapButton_OverlayBkg = new wxBitmapButton(BufferScrollWindow, ID_BITMAPBUTTON_OverlayBkg, padlock16x16_blue_xpm, wxDefaultPosition, wxSize(13,13), wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_BITMAPBUTTON_OverlayBkg"));
	BitmapButton_OverlayBkg->SetDefault();
	BitmapButton_OverlayBkg->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
	BitmapButton_OverlayBkg->SetToolTip(_("Lock/Unlock. If Locked then a \"Create Random Effects\" will NOT change this value."));
	Sizer2->Add(BitmapButton_OverlayBkg, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(Sizer2, 0, wxEXPAND, 10);
	SubBufferPanelSizer = new wxFlexGridSizer(0, 1, 0, 0);
	SubBufferPanelSizer->AddGrowableCol(0);
	SubBufferPanelSizer->AddGrowableRow(0);
	FlexGridSizer4->Add(SubBufferPanelSizer, 1, wxALL|wxEXPAND, 0);
	BufferScrollWindow->SetSizer(FlexGridSizer4);
	FlexGridSizer4->Fit(BufferScrollWindow);
	FlexGridSizer4->SetSizeHints(BufferScrollWindow);
	FlexGridSizer3->Add(BufferScrollWindow, 0, wxALIGN_LEFT, 0);
	Panel_Sizer->SetSizer(FlexGridSizer3);
	FlexGridSizer3->Fit(Panel_Sizer);
	FlexGridSizer3->SetSizeHints(Panel_Sizer);
	FlexGridSizer1->Add(Panel_Sizer, 0, wxALIGN_LEFT, 0);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_BITMAPBUTTON_CHOICE_BufferStyle,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&BufferPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_CHOICE_BufferTransform,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&BufferPanel::OnLockButtonClick);
	Connect(ID_SLIDER_EffectBlur,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&BufferPanel::UpdateLinkedTextCtrl);
	Connect(IDD_TEXTCTRL_EffectBlur,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&BufferPanel::UpdateLinkedSlider);
	Connect(ID_BITMAPBUTTON_SLIDER_EffectBlur,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&BufferPanel::OnLockButtonClick);
	Connect(ID_BITMAPBUTTON_OverlayBkg,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&BufferPanel::OnLockButtonClick);
	Panel_Sizer->Connect(wxEVT_SIZE,(wxObjectEventFunction)&BufferPanel::OnResize,0,this);
	//*)
    
    
    subBufferPanel = new SubBufferPanel(BufferScrollWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    SubBufferPanelSizer->Add(subBufferPanel, 1, wxALL|wxEXPAND, 2);
}

BufferPanel::~BufferPanel()
{
	//(*Destroy(BufferPanel)
	//*)
}


PANEL_EVENT_HANDLERS(BufferPanel)

wxString BufferPanel::GetBufferString() {
    wxString s;
    // Blur
    if (Slider_EffectBlur->GetValue() > 1) {
        s += wxString::Format("B_SLIDER_EffectBlur=%d,",Slider_EffectBlur->GetValue());
    }
    // Persistent
    if (CheckBox_OverlayBkg->GetValue()) {
        s += "B_CHECKBOX_OverlayBkg=1,";
    }
    if (BufferStyleChoice->GetSelection() != 0) {
        s += "B_CHOICE_BufferStyle=";
        s += BufferStyleChoice->GetStringSelection();
        s += ",";
    }
    if (BufferTransform->GetSelection() != 0) {
        s += "B_CHOICE_BufferTransform=";
        s += BufferTransform->GetStringSelection();
        s += ",";
    }
    
    wxString subB = subBufferPanel->GetValue();
    if (subB.size() > 0) {
        s += "B_CUSTOM_SubBuffer=";
        s += subB;
        s += ",";
    }
    return s;
}

void BufferPanel::SetDefaultControls(const Model *model) {
    Slider_EffectBlur->SetValue(1);
    CheckBox_OverlayBkg->SetValue(false);
    BufferStyleChoice->Clear();
    if (model != nullptr) {
        const std::vector<std::string> &types = model->GetBufferStyles();
        for (auto it = types.begin(); it != types.end(); it++) {
            BufferStyleChoice->Append(*it);
        }
    }
    if (BufferStyleChoice->IsEmpty()) {
        BufferStyleChoice->Append("Default");
    }
    subBufferPanel->SetDefaults();

    Slider_EffectBlur->SetValue(1);
    TextCtrl_EffectBlur->SetValue("1");
    BufferStyleChoice->SetSelection(0);
    BufferTransform->SetSelection(0);
    wxSizeEvent evt;
    OnResize(evt);
}


void BufferPanel::OnResize(wxSizeEvent& event)
{
    wxSize s = GetSize();
    Panel_Sizer->SetSize(s);
    Panel_Sizer->SetMinSize(s);
    Panel_Sizer->SetMaxSize(s);
    Panel_Sizer->Refresh();

    BufferScrollWindow->SetSize(s);
    BufferScrollWindow->SetMinSize(s);
    BufferScrollWindow->SetMaxSize(s);

    BufferScrollWindow->FitInside();
    BufferScrollWindow->SetScrollRate(5, 5);
    BufferScrollWindow->Refresh();
}

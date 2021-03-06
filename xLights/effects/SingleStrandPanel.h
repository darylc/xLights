#ifndef SINGLESTRANDPANEL_H
#define SINGLESTRANDPANEL_H

//(*Headers(SingleStrandPanel)
#include <wx/panel.h>
class wxTextCtrl;
class wxChoice;
class wxNotebook;
class wxFlexGridSizer;
class wxBitmapButton;
class wxSlider;
class wxNotebookEvent;
class wxStaticText;
class wxCheckBox;
//*)

class SingleStrandPanel: public wxPanel
{
	public:

		SingleStrandPanel(wxWindow* parent);
		virtual ~SingleStrandPanel();

		//(*Declarations(SingleStrandPanel)
		wxBitmapButton* BitmapButton_Chase_3dFade1;
		wxBitmapButton* BitmapButton_Skips_Direction;
		wxSlider* Slider_Color_Mix1;
		wxStaticText* StaticText60;
		wxPanel* Panel1;
		wxBitmapButton* BitmapButton3;
		wxStaticText* StaticText13;
		wxBitmapButton* BitmapButton_Color_Mix1;
		wxStaticText* StaticText109;
		wxChoice* Choice_SingleStrand_Colors;
		wxPanel* Panel2;
		wxChoice* Choice_Skips_Direction;
		wxBitmapButton* BitmapButton_Chase_Type1;
		wxNotebook* SingleStrandEffectType;
		wxCheckBox* CheckBox_Chase_Group_All;
		wxStaticText* StaticText12;
		wxBitmapButton* BitmapButton4;
		wxBitmapButton* BitmapButton_Chase_Group_All;
		wxBitmapButton* BitmapButton_SingleStrand_Colors;
		wxSlider* Slider_Number_Chases;
		wxStaticText* StaticText61;
		wxCheckBox* CheckBox_Chase_3dFade1;
		wxBitmapButton* BitmapButton_Skips_BandSize;
		wxStaticText* StaticText9;
		wxStaticText* StaticText11;
		wxBitmapButton* BitmapButton_Skips_StartingPosition;
		wxBitmapButton* BitmapButton_Skips_SkipSize;
		wxBitmapButton* BitmapButton_Number_Chases;
		//*)

	protected:

		//(*Identifiers(SingleStrandPanel)
		static const long ID_STATICTEXT63;
		static const long ID_CHOICE_SingleStrand_Colors;
		static const long ID_BITMAPBUTTON_SingleStrand_Colors;
		static const long ID_STATICTEXT62;
		static const long ID_SLIDER_Number_Chases;
		static const long IDD_TEXTCTRL_Number_Chases;
		static const long ID_BITMAPBUTTON_Number_Chases;
		static const long ID_STATICTEXT8;
		static const long ID_SLIDER_Color_Mix1;
		static const long IDD_TEXTCTRL_Color_Mix1;
		static const long ID_BITMAPBUTTON_SLIDER_Color_Mix1;
		static const long IDD_SLIDER_Chase_Rotations;
		static const long ID_TEXTCTRL_Chase_Rotations;
		static const long ID_BITMAPBUTTON23;
		static const long ID_STATICTEXT10;
		static const long ID_CHOICE_Chase_Type1;
		static const long ID_BITMAPBUTTON_CHOICE_Chase_Type1;
		static const long ID_STATICTEXT11;
		static const long ID_CHECKBOX_Chase_3dFade1;
		static const long ID_BITMAPBUTTON_CHECKBOX_Chase_3dFade1;
		static const long ID_STATICTEXT12;
		static const long ID_CHECKBOX_Chase_Group_All;
		static const long ID_BITMAPBUTTON_Chase_Group_All;
		static const long ID_PANEL3;
		static const long ID_SLIDER_Skips_BandSize;
		static const long IDD_TEXTCTRL_Skips_BandSize;
		static const long ID_BITMAPBUTTON44;
		static const long ID_SLIDER_Skips_SkipSize;
		static const long IDD_TEXTCTRL_Skips_SkipSize;
		static const long ID_BITMAPBUTTON46;
		static const long ID_SLIDER_Skips_StartPos;
		static const long IDD_TEXTCTRL_Skips_StartPos;
		static const long ID_BITMAPBUTTON47;
		static const long ID_SLIDER_Skips_Advance;
		static const long IDD_TEXTCTRL_Skips_Advance;
		static const long ID_BITMAPBUTTON22;
		static const long ID_STATICTEXT111;
		static const long ID_CHOICE_Skips_Direction;
		static const long ID_BITMAPBUTTON48;
		static const long ID_PANEL21;
		static const long ID_NOTEBOOK_SSEFFECT_TYPE;
		//*)

	public:

		//(*Handlers(SingleStrandPanel)
    void UpdateLinkedSliderFloat(wxCommandEvent& event);
    void UpdateLinkedTextCtrlFloat(wxScrollEvent& event);
    void UpdateLinkedTextCtrl360(wxScrollEvent& event);
    void UpdateLinkedSlider360(wxCommandEvent& event);
    void UpdateLinkedTextCtrl(wxScrollEvent& event);
    void UpdateLinkedSlider(wxCommandEvent& event);
    void OnLockButtonClick(wxCommandEvent& event);

		//*)

		DECLARE_EVENT_TABLE()
};

#endif

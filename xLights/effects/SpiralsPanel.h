#ifndef SPIRALSPANEL_H
#define SPIRALSPANEL_H

//(*Headers(SpiralsPanel)
#include <wx/panel.h>
class wxCheckBox;
class wxTextCtrl;
class wxStaticText;
class wxSlider;
class wxBitmapButton;
class wxFlexGridSizer;
//*)

class SpiralsPanel: public wxPanel
{
	public:

		SpiralsPanel(wxWindow* parent);
		virtual ~SpiralsPanel();

		//(*Declarations(SpiralsPanel)
		wxCheckBox* CheckBox_Spirals_Blend;
		wxCheckBox* CheckBox_Spirals_3D;
		wxSlider* Slider_Spirals_Count;
		wxStaticText* StaticText40;
		wxBitmapButton* BitmapButton_Spirals3D;
		wxStaticText* StaticText38;
		wxBitmapButton* BitmapButton_SpiralsShrink;
		wxBitmapButton* BitmapButton_SpiralsGrow;
		wxStaticText* StaticText34;
		wxBitmapButton* BitmapButton_SpiralsBlend;
		wxBitmapButton* BitmapButton_SpiralsThickness;
		wxSlider* Slider_Spirals_Thickness;
		wxCheckBox* CheckBox_Spirals_Grow;
		wxBitmapButton* BitmapButton_SpiralsDirection;
		wxBitmapButton* BitmapButton_SpiralsCount;
		wxBitmapButton* BitmapButton_SpiralsRotation;
		wxCheckBox* CheckBox_Spirlas_Shrink;
		//*)

	protected:

		//(*Identifiers(SpiralsPanel)
		static const long ID_STATICTEXT38;
		static const long ID_SLIDER_Spirals_Count;
		static const long IDD_TEXTCTRL_Spirals_Count;
		static const long ID_BITMAPBUTTON_SLIDER_Spirals_Count;
		static const long ID_SLIDER_Spirals_Rotation;
		static const long IDD_TEXTCTRL_Spirals_Rotation;
		static const long ID_BITMAPBUTTON_SLIDER_Spirals_Rotation;
		static const long ID_STATICTEXT42;
		static const long ID_SLIDER_Spirals_Thickness;
		static const long IDD_TEXTCTRL_Spirals_Thickness;
		static const long ID_BITMAPBUTTON_SLIDER_Spirals_Thickness;
		static const long ID_STATICTEXT44;
		static const long IDD_SLIDER_Spirals_Movement;
		static const long ID_TEXTCTRL_Spirals_Movement;
		static const long ID_BITMAPBUTTON_SLIDER_Spirals_Direction;
		static const long ID_CHECKBOX_Spirals_Blend;
		static const long ID_BITMAPBUTTON_CHECKBOX_Spirals_Blend;
		static const long ID_CHECKBOX_Spirals_3D;
		static const long ID_BITMAPBUTTON_CHECKBOX_Spirals_3D;
		static const long ID_CHECKBOX_Spirals_Grow;
		static const long ID_BITMAPBUTTON_CHECKBOX_Spirals_Grow;
		static const long ID_CHECKBOX_Spirals_Shrink;
		static const long ID_BITMAPBUTTON_CHECKBOX_Spirals_Shrink;
		//*)

	public:

		//(*Handlers(SpiralsPanel)
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

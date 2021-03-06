#ifndef COLORWASHPANEL_H
#define COLORWASHPANEL_H

//(*Headers(ColorWashPanel)
#include <wx/panel.h>
class wxTextCtrl;
class wxNotebook;
class wxFlexGridSizer;
class wxBitmapButton;
class wxSlider;
class wxGridBagSizer;
class wxNotebookEvent;
class wxStaticText;
class wxCheckBox;
//*)

class ColorWashPanel: public wxPanel
{
	public:

		ColorWashPanel(wxWindow* parent);
		virtual ~ColorWashPanel();

		//(*Declarations(ColorWashPanel)
		wxCheckBox* ShimmerCheckBox;
		wxSlider* SliderCycles;
		wxSlider* C2SliderX;
		wxCheckBox* CircularPaletteCheckBox;
		wxSlider* C1SliderY;
		wxSlider* C1SliderX;
		wxCheckBox* VFadeCheckBox;
		wxBitmapButton* BitmapButton_ColorWashVFade;
		wxCheckBox* HFadeCheckBox;
		wxNotebook* CornersNotebook;
		wxCheckBox* EntireModelCheckbox;
		wxStaticText* StaticText5;
		wxTextCtrl* CyclesTextCtrl;
		wxBitmapButton* BitmapButton_ColorWashHFade;
		wxBitmapButton* BitmapButton_ColorWashCount;
		wxSlider* C2SliderY;
		//*)


		//(*Identifiers(ColorWashPanel)
		static const long ID_STATICTEXT21;
		static const long IDD_SLIDER_ColorWash_Cycles;
		static const long ID_TEXTCTRL_ColorWash_Cycles;
		static const long ID_BITMAPBUTTON_SLIDER_ColorWash_Count;
		static const long ID_CHECKBOX_ColorWash_VFade;
		static const long ID_BITMAPBUTTON_CHECKBOX_ColorWash_VFade;
		static const long ID_CHECKBOX_ColorWash_HFade;
		static const long ID_BITMAPBUTTON_CHECKBOX_ColorWash_HFade;
		static const long ID_CHECKBOX_ColorWash_EntireModel;
		static const long ID_CHECKBOX_ColorWash_Shimmer;
		static const long ID_CHECKBOX_ColorWash_CircularPalette;
		static const long ID_SLIDER_ColorWash_X1;
		static const long IDD_TEXTCTRL_ColorWash_X1;
		static const long IDD_TEXTCTRL_ColorWash_Y1;
		static const long ID_SLIDER_ColorWash_Y1;
		static const long ID_PANEL46;
		static const long ID_SLIDER_ColorWash_X2;
		static const long IDD_TEXTCTRL_ColorWash_X2;
		static const long IDD_TEXTCTRL_ColorWash_Y2;
		static const long ID_SLIDER_ColorWash_Y2;
		static const long ID_PANEL47;
		static const long IDD_NOTEBOOK_ColorWashRect;
		//*)

    
		//(*Handlers(ColorWashPanel)
    void UpdateLinkedSliderFloat(wxCommandEvent& event);
    void UpdateLinkedTextCtrlFloat(wxScrollEvent& event);
    void UpdateLinkedTextCtrl360(wxScrollEvent& event);
    void UpdateLinkedSlider360(wxCommandEvent& event);
    void UpdateLinkedTextCtrl(wxScrollEvent& event);
    void UpdateLinkedSlider(wxCommandEvent& event);
    void OnLockButtonClick(wxCommandEvent& event);
    
    void OnColorWashEntireModelClicked(wxCommandEvent& event);

		//*)

		DECLARE_EVENT_TABLE()
};

#endif

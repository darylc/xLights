#ifndef COLORPANEL_H
#define COLORPANEL_H

//(*Headers(ColorPanel)
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
//*)

#include <wx/colordlg.h>
#include <wx/checkbox.h>
#include <unordered_map>

#include <vector>
#include <map>

wxDECLARE_EVENT(EVT_EFFECT_PALETTE_UPDATED, wxCommandEvent);

class ColorPanel: public wxPanel
{
	public:

		ColorPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ColorPanel();

        void SetDefaultSettings();
		wxString GetColorString();
        wxString GetRandomColorString();

        bool PaletteChanged;
        bool EffectChanged;

    
        void SetDefaultPalette();
private:
        void ValidateWindow();
        wxColour GetPaletteColor(int idx);
public:
        void SetButtonColor(wxBitmapButton* btn, const std::string &v);
        static const int PALETTE_SIZE = 8;

		//(*Declarations(ColorPanel)
		wxStaticText* StaticText22;
		wxBitmapButton* BitmapButton_Contrast;
		wxBitmapButton* BitmapButton_Brightness;
		wxFlexGridSizer* FlexGridSizer_Palette;
		wxBitmapButton* BitmapButton_random;
		wxBitmapButton* BitmapButton_normal;
		wxPanel* Panel_Sizer;
		wxBitmapButton* BitmapButton_SparkleFrequency;
		wxStaticText* StaticText127;
		wxTextCtrl* txtCtlBrightness;
		wxTextCtrl* txtCtlContrast;
		wxBitmapButton* BitmapButton_locked;
		wxBitmapButton* BitmapButton_MusicSparkles;
		wxSlider* Slider_Brightness;
		wxStaticText* StaticText126;
		wxSlider* Slider_SparkleFrequency;
		wxScrolledWindow* ColorScrollWindow;
		wxTextCtrl* txtCtrlSparkleFreq;
		wxSlider* Slider_Contrast;
		wxCheckBox* CheckBox_MusicSparkles;
		//*)

	protected:

		//(*Identifiers(ColorPanel)
		static const long ID_BUTTON1;
		static const long ID_STATICTEXT24;
		static const long ID_SLIDER_SparkleFrequency;
		static const long IDD_TEXTCTRL_SparkleFrequency;
		static const long ID_BITMAPBUTTON_SLIDER_SparkleFrequency;
		static const long ID_CHECKBOX_MusicSparkles;
		static const long ID_BITMAPBUTTON_MusicSparkles;
		static const long ID_STATICTEXT127;
		static const long ID_SLIDER_Brightness;
		static const long IDD_TEXTCTRL_Brightness;
		static const long ID_BITMAPBUTTON_SLIDER_Brightness;
		static const long ID_STATICTEXT128;
		static const long ID_SLIDER_Contrast;
		static const long IDD_TEXTCTRL_Contrast;
		static const long ID_BITMAPBUTTON_SLIDER_Contrast;
		static const long ID_BITMAPBUTTON87;
		static const long ID_BITMAPBUTTON1;
		static const long ID_BITMAPBUTTON88;
		static const long ID_SCROLLED_ColorScroll;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(ColorPanel)
		void OnCheckBox_PaletteClick(wxCommandEvent& event);
		void OnButton_PaletteNumberClick(wxCommandEvent& event);
		void OnResize(wxSizeEvent& event);
		void OnUpdateColorClick(wxCommandEvent& event);


		void UpdateLinkedSliderFloat(wxCommandEvent& event);
		void UpdateLinkedTextCtrlFloat(wxScrollEvent& event);
		void UpdateLinkedTextCtrl360(wxScrollEvent& event);
		void UpdateLinkedSlider360(wxCommandEvent& event);
		void UpdateLinkedTextCtrl(wxScrollEvent& event);
		void UpdateLinkedSlider(wxCommandEvent& event);
		void OnLockButtonClick(wxCommandEvent& event);
		void OnCheckBox_MusicSparklesClick(wxCommandEvent& event);
		//*)
        wxCheckBox* GetPaletteCheckbox(int idx);
        wxButton* GetPaletteButton(int idx);
        wxColourData colorData;
    
        std::vector<wxBitmapButton*> buttons;
        std::vector<wxCheckBox*> checkBoxes;
        std::map<int, std::string> lastColors;

		DECLARE_EVENT_TABLE()

};

#endif

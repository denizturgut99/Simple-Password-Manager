#include <wx/wx.h>
#include <wx/textfile.h>
#include "PasswordManager.h"
#include <string>

using namespace std;

class MainFrame : public wxFrame {
private:
	// necessary global variables
	PasswordManager pm;
	string temp_username = ""; // hold input value for username temporarily
	string temp_pwd = ""; // hold input value for password temporarily
	// initialise the panels and parent sizer as nullptrs since they have to be pointers as recommended by wxwidgets
	// panels are basically the containers for widgets such as buttons text fields, etc.
	wxPanel* parentPanel = nullptr;
	wxPanel* mainMenuPanel = nullptr;
	wxPanel* masterPwdPanel = nullptr;
	wxPanel* addCredentialsPanel = nullptr;
	wxPanel* getCredentialsPanel = nullptr;
	wxPanel* deleteCredentialsPanel = nullptr;
	wxBoxSizer* parentSizer = nullptr; // BoxSizers are used for layouts and design, the parent sizer is required to keep the child sizers in check
	/*
	* create flags which will be used when the widgets (buttons, etc.) are added into the sizer
	* 25 pixel border is set around all widgets to have some space between all of them
	* CenterHorizontal() is just to center the widgets horizontally so that it is easier for the user to see
	*/
	wxSizerFlags horizontalCenter = wxSizerFlags().CenterHorizontal();
	wxSizerFlags centerWithBorder25 = wxSizerFlags().CenterHorizontal().Border(wxTOP, 25);

	/*
	* =====================EVENT HANDLERS=====================
	* these functions are triggered when a specific button that is binded to these functions is clicked
	* wxCommandEvent& is required by wxWidgets
	* the functions that have wxTextCtrl as params are for the certain situations where the input data has to be read
	*/
	void submitMasterPwd(wxCommandEvent& event, wxTextCtrl* masterPwdField);
	void clickCredentialsPanels(wxCommandEvent& event, wxPanel* credentialsPanel);
	void addCredentials(wxCommandEvent& event, wxTextCtrl* nameField, wxTextCtrl* pwdField);
	void getCredentials(wxCommandEvent& event, wxTextCtrl* inputField);
	void deleteCredentials(wxCommandEvent& event, wxTextCtrl* inputField);
	void randomPwd(wxCommandEvent& event, wxTextCtrl* pwdField);
	void displayPwds(wxCommandEvent& event);

	/*
	* =====================PANEL HANDLERS=====================
	* these functions create the panels, these are basically what is being shown in the screen
	* by default all of the panels except the masterPwd panel are hidden
	*/
	void masterPwdPanelHandler();
	void mainMenuPanelHandler();
	void addCredentialPanelHandler();
	void deleteCredentialPanelHandler();
	void getCredentialsPanelHandler();
	
public:
	MainFrame();
};



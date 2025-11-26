#include "MainFrame.h"
#include <fstream>

using namespace std;

MainFrame::MainFrame(): wxFrame(nullptr, wxID_ANY, "Password Manager") {
    // wxwidget library takes care of deallocating the memory once the application shuts down
    // create the panels and hide them, this way when the event handlers kick in then the correct panel can be shown
    masterPwdPanelHandler();
    addCredentialPanelHandler();
    getCredentialsPanelHandler();
    deleteCredentialPanelHandler();
    mainMenuPanelHandler();
}

void MainFrame::submitMasterPwd(wxCommandEvent& event, wxTextCtrl* masterPwdField) {
    if (pm.masterPassword(masterPwdField->GetValue().ToStdString())) {
        /*
        * if master password check succeeds, then the master pwd panel is hidden
        * mainMenu panel is shown to the user
        * the Layout function that is part of the parentSizer (wxBoxSizer) is triggered to update the view
        * without the Layout function, the view won't be updated so it is required
        */
        masterPwdPanel->Show(false);
        mainMenuPanel->Show();
        parentSizer->Layout();
    }
}

void MainFrame::masterPwdPanelHandler() {
    /*
    * initialise the parentPanel that contains all of the child panels
    * "this" keyword is passed as the MainFrame class, which is a frame, has to handle this panel
    * a frame is basically a top level window which contains the application
    */
    parentPanel = new wxPanel(this);
    parentSizer = new wxBoxSizer(wxVERTICAL);
    masterPwdPanel = new wxPanel(parentPanel, wxID_ANY);

    // Sizer for masterPwdPanel, the layout for this specific panel will be handled by this sizer
    wxBoxSizer* localSizer = new wxBoxSizer(wxVERTICAL);

    // create the necessary widgets (i.e.: buttons, text fields, etc.)
    // these widgets are attached to the masterPwdPanel
    wxTextCtrl* masterPwdInput = new wxTextCtrl(masterPwdPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));
    wxButton* submit = new wxButton(masterPwdPanel, wxID_ANY, "Submit");
    // the label is shown right above the input field, the text for it depends on if there already is a master pasword or not
    wxString labelText = wxFileExists("masterpwd.txt") ? "Master password" : "Create a master password";
    wxStaticText* label = new wxStaticText(masterPwdPanel, wxID_ANY, labelText);

    localSizer->AddStretchSpacer(1); // adds proportional space above, in this case it basically vertically aligns it to center
    // attach widgets to the sizer with specified design flags, these flags specify if there should be extra space, etc.
    localSizer->Add(label, horizontalCenter);
    localSizer->Add(masterPwdInput, horizontalCenter);
    localSizer->Add(submit, centerWithBorder25);
    localSizer->AddStretchSpacer(1); // adds proportional space below, in this case it basically vertically aligns it to center

    masterPwdPanel->SetSizer(localSizer); // attach the local sizer to this local panel

    // attach the master password panel to the parent sizer to have proper design control once the panel is hidden and another one is shown
    parentSizer->Add(masterPwdPanel, 1, wxEXPAND | wxALL, 10);
    // parentSizer is attached to the parentPanel, this is only done once
    parentPanel->SetSizer(parentSizer); 
    // this will respect the borders of the app, user shouldnt be able to make the window smaller than what is being shown
    parentSizer->SetSizeHints(this);
    /*
    * placeholders::_1 is required due to the wxwidgets event handler passing it's own argument which is wxCommandEvent
    * it is just a placeholder until the event actually passes a value into it, without it the code wont compile 
    */
    submit->Bind(wxEVT_BUTTON, bind(& MainFrame::submitMasterPwd, this, placeholders::_1, masterPwdInput));
}

void MainFrame::mainMenuPanelHandler() {
    // create the mainMenu panel and attach it to the parent panel
    mainMenuPanel = new wxPanel(parentPanel, wxID_ANY);
    wxBoxSizer* localSizer = new wxBoxSizer(wxVERTICAL);
    // create buttons and attach them to the local panel which is mainMenuPanel
    // the string passed in are used as labels which will be shown within the buttons
    wxButton* addCredentials = new wxButton(mainMenuPanel, wxID_ANY, "Add Credentials");
    wxButton* getCredentials = new wxButton(mainMenuPanel, wxID_ANY, "Get Credentials");
    wxButton* deleteCredentials = new wxButton(mainMenuPanel, wxID_ANY, "Delete Credentials");
    wxButton* loadFromFile = new wxButton(mainMenuPanel, wxID_ANY, "Load from file");

    localSizer->AddStretchSpacer(1); // adds proportional space above, in this case it basically vertically aligns it to center
    // attach widgets to the sizer with specified design flags
    localSizer->Add(addCredentials, centerWithBorder25);
    localSizer->Add(getCredentials, centerWithBorder25);
    localSizer->Add(deleteCredentials, centerWithBorder25);
    localSizer->Add(loadFromFile, centerWithBorder25);
    localSizer->AddStretchSpacer(1); // adds proportional space below, in this case it basically vertically aligns it to center

    mainMenuPanel->SetSizer(localSizer); // attach local sizer to mainMenu panel

    parentSizer->Add(mainMenuPanel, 1, wxEXPAND | wxALL, 10);
    // this time SetSizeHints is not required as it was already set in the first function

    // since this function only runs in the beginning, this panel is supposed to be hidden until a specific button has been clicked
    mainMenuPanel->Show(false);

    /*
    * add events to buttons, the keyword "this" is used to have these events tied to the current frame
    * placeholders::_1 is required due to the wxwidgets event handler passing it's own argument which is wxCommandEvent
    * it is just a placeholder until the event actually passes a value into it, without it the code wont compile
    */
    addCredentials->Bind(wxEVT_BUTTON, bind(& MainFrame::clickCredentialsPanels, this, placeholders::_1, addCredentialsPanel));
    getCredentials->Bind(wxEVT_BUTTON, bind(&MainFrame::clickCredentialsPanels, this, placeholders::_1, getCredentialsPanel));
    deleteCredentials->Bind(wxEVT_BUTTON, bind(&MainFrame::clickCredentialsPanels, this, placeholders::_1, deleteCredentialsPanel));
    loadFromFile->Bind(wxEVT_BUTTON, &MainFrame::displayPwds, this);
}

void MainFrame::addCredentialPanelHandler() {
    // create the addCredentials panel and attach it to the parent panel
    addCredentialsPanel = new wxPanel(parentPanel, wxID_ANY);
    // createthe required local sizer, wxVERTICAL is for vertical alignment
    wxBoxSizer* localSizer = new wxBoxSizer(wxVERTICAL);
    // create the required widgets and attach them to the addCredentials panel
    wxStaticText* usrnLabel = new wxStaticText(addCredentialsPanel, wxID_ANY, "Username");
    wxTextCtrl* usrnField = new wxTextCtrl(addCredentialsPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));
    wxStaticText* pwdLabel = new wxStaticText(addCredentialsPanel, wxID_ANY, "Password");
    wxTextCtrl* pwdField = new wxTextCtrl(addCredentialsPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));
    wxButton* submit = new wxButton(addCredentialsPanel, wxID_ANY, "Submit");
    wxButton* generateRandPwd = new wxButton(addCredentialsPanel, wxID_ANY, "Generate random password");

    localSizer->AddStretchSpacer(1); // adds proportional space above, in this case it basically vertically aligns it to center
    // attach widgets to the sizer with specified design flags, these flags specify if there should be extra space, etc.
    localSizer->Add(usrnLabel, horizontalCenter);
    localSizer->Add(usrnField, horizontalCenter);
    localSizer->Add(pwdLabel, centerWithBorder25);
    localSizer->Add(pwdField, horizontalCenter);
    localSizer->Add(generateRandPwd, centerWithBorder25);
    localSizer->Add(submit, centerWithBorder25);
    localSizer->AddStretchSpacer(1); // adds proportional space bloew, in this case it basically vertically aligns it to center

    addCredentialsPanel->SetSizer(localSizer); // attach local sizer to mainMenu panel
    parentSizer->Add(addCredentialsPanel, 1, wxEXPAND | wxALL, 10); // attach the local panel to the parentSizer
    addCredentialsPanel->Show(false);

    /*
    * placeholders::_1 is required due to the wxwidgets event handler passing it's own argument which is wxCommandEvent
    * it is just a placeholder until the event actually passes a value into it, without it the code wont compile
    */
    submit->Bind(wxEVT_BUTTON, bind(&MainFrame::addCredentials, this, placeholders::_1, usrnField, pwdField));
    generateRandPwd->Bind(wxEVT_BUTTON, bind(&MainFrame::randomPwd, this, placeholders::_1, pwdField));
}

void MainFrame::clickCredentialsPanels(wxCommandEvent& event, wxPanel* credentialsPanel) {
    // hide mainMenu panel, show add/get/delete credentials panel and update the layout to display the changes
    mainMenuPanel->Show(false);
    credentialsPanel->Show();
    parentSizer->Layout();
}

void MainFrame::addCredentials(wxCommandEvent& event, wxTextCtrl* nameField, wxTextCtrl* pwdField) {
    // get the current value from the input fields and convert them into a std string, otherwise it will be wxString
    string username = nameField->GetValue().ToStdString();
    string password = pwdField->GetValue().ToStdString();
    if (pm.addCredentials(username, password)) {
        /*
        * if the credentials are successfully created:
        * clear the input fields
        * hide the addCredentials panel
        * show mainMenu panel
        * update the sizer to display the changes
        */
        nameField->Clear();
        pwdField->Clear();
        addCredentialsPanel->Show(false);
        mainMenuPanel->Show();
        parentSizer->Layout();
    }
}

void MainFrame::randomPwd(wxCommandEvent& event, wxTextCtrl* pwdField) {
    // generates a random password and then converts it into wxString
    wxString generatedPwd(pm.generateRandomPwd());
    pwdField->SetValue(generatedPwd);
}

void MainFrame::displayPwds(wxCommandEvent& event) {
    pm.displayAllPwds();
}

void MainFrame::getCredentialsPanelHandler() {
    // create the getCredentials panel and attach it to the parent panel
    getCredentialsPanel = new wxPanel(parentPanel, wxID_ANY);
    // create the required local sizer, wxVERTICAL is for vertical alignment
    wxBoxSizer* localSizer = new wxBoxSizer(wxVERTICAL);
    // create the required widgets and attach them to the getCredentials panel
    wxTextCtrl* inputField = new wxTextCtrl(getCredentialsPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));
    wxStaticText* inputLabel = new wxStaticText(getCredentialsPanel, wxID_ANY, "Username");
    wxButton* fetch = new wxButton(getCredentialsPanel, wxID_ANY, "Get credentials");

    localSizer->AddStretchSpacer(1); // adds proportional space above, in this case it basically vertically aligns it to center
    // attach widgets to the sizer with specified design flags, these flags specify if there should be extra space, etc.
    localSizer->Add(inputLabel, horizontalCenter);
    localSizer->Add(inputField, horizontalCenter);
    localSizer->Add(fetch, centerWithBorder25);
    localSizer->AddStretchSpacer(1); // adds proportional space below, in this case it basically vertically aligns it to center

    getCredentialsPanel->SetSizer(localSizer);
    parentSizer->Add(getCredentialsPanel, 1, wxEXPAND | wxALL, 10);
    getCredentialsPanel->Show(false); // Should be hidden to begin with as it is not necessary upon creation

    /*
    * placeholders::_1 is required due to the wxwidgets event handler passing it's own argument which is wxCommandEvent
    * it is just a placeholder until the event actually passes a value into it, without it the code wont compile
    */
    fetch->Bind(wxEVT_BUTTON, bind(& MainFrame::getCredentials, this, placeholders::_1, inputField));
}

void MainFrame::getCredentials(wxCommandEvent& event, wxTextCtrl* inputField) {
    // findUserData looks for the inpunt in the pwd file and tries to match it, if no results are found then an empty string is returned
    wxString credentials = pm.findUserData(inputField->GetValue().ToStdString());
    // if the credentials is not empty, then display the credentials, else display a message saying this username doesn't exist
    credentials.length() ? wxMessageBox(credentials) : wxMessageBox("Credentials with this username doesn't exist.");
    inputField->Clear(); // clear the input field, otherwise it will still have a value next time the user navigates to this view
    getCredentialsPanel->Show(false); // hide getCredentials panel
    mainMenuPanel->Show(); // show mainMenu panel
    parentSizer->Layout(); // update the layout to display the changes
}

void MainFrame::deleteCredentialPanelHandler() {
    // create the deleteCredentials panel and attach it to the parent panel
    deleteCredentialsPanel = new wxPanel(parentPanel, wxID_ANY);
    // create the required local sizer, wxVERTICAL is for vertical alignment
    wxBoxSizer* localSizer = new wxBoxSizer(wxVERTICAL);
    // create the required widgets and attach them to the deleteCredentials panel
    wxTextCtrl* inputField = new wxTextCtrl(deleteCredentialsPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));
    wxStaticText* inputLabel = new wxStaticText(deleteCredentialsPanel, wxID_ANY, "Username");
    wxButton* fetch = new wxButton(deleteCredentialsPanel, wxID_ANY, "Delete credentials");

    localSizer->AddStretchSpacer(1); // adds proportional space above, in this case it basically vertically aligns it to center
    // attach widgets to the sizer with specified design flags, these flags specify if there should be extra space, etc.
    localSizer->Add(inputLabel, horizontalCenter);
    localSizer->Add(inputField, horizontalCenter);
    localSizer->Add(fetch, centerWithBorder25);
    localSizer->AddStretchSpacer(1); // adds proportional space below, in this case it basically vertically aligns it to center

    deleteCredentialsPanel->SetSizer(localSizer);
    parentSizer->Add(deleteCredentialsPanel, 1, wxEXPAND | wxALL, 10);
    deleteCredentialsPanel->Show(false); // Should be hidden to begin with as it is not necessary upon creation

    /*
    * placeholders::_1 is required due to the wxwidgets event handler passing it's own argument which is wxCommandEvent
    * it is just a placeholder until the event actually passes a value into it, without it the code wont compile
    */
    fetch->Bind(wxEVT_BUTTON, bind(&MainFrame::deleteCredentials, this, placeholders::_1, inputField));
}

void MainFrame::deleteCredentials(wxCommandEvent& event, wxTextCtrl* inputField) {
    /*
    * get the value from the input field, convert it to a regular c++ string
    * call the deleteUser function with the input value as parameter
    */
    pm.deleteUser(inputField->GetValue().ToStdString());
    inputField->Clear(); // clear the input field so that next time this view is accessed, the input field will be empty
    deleteCredentialsPanel->Show(false); // hide the deleteCredentials panel
    mainMenuPanel->Show(); // show the mainMenu panel
    parentSizer->Layout(); // update the layout so that the changes are displayed properly
}


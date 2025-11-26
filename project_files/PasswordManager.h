#include <wx/wx.h>
#include <string>
#include "FileManager.h"

using namespace std;

class PasswordManager : public FileManager {
private:
    bool checkPassword(string pwd);

public:
    void displayAllPwds();
    wxString findUserData(string input);
    bool masterPassword(string value);
    bool addCredentials(string name, string pwd);
    string generateRandomPwd();

};

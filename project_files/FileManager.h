#include <wx/wx.h>
#include <wx/textfile.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class FileManager {
public:
    void saveToFile(string username, string pwd);
    vector<string> readPwdFile();
    void deleteUser(string input);
    string encrypt(string data);
    string decrypt(string data);
};

#include "FileManager.h"

#define PWD_FILE "pwd.txt"
#define TEMP_FILE "temp.txt"

using namespace std;

string FileManager::encrypt(string data) {
    string encrypted = "";
    // Caesar cypher, usernames and passwords will be encrypted here
    // the values are only shifted by 3
    for (char c : data) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            encrypted += (c - base + 3) % 26 + base;
        }
        else if (isdigit(c)) {
            // if the char is a digit, then use 0 as the base for calculations
            encrypted += (c - '0' + 3) % 10 + '0';
        }
        else {
            encrypted += c;
        }
    }

    return encrypted; // return the final encrypted string to be used by the caller
}

string FileManager::decrypt(string data) {
    string decrypted = "";
    // Caesar cypher, usernames and passwords will be decrypted here
    // the shift (3, just like in encryption) is removed from the modulo (26 or 10) and rest of the calculation is the same, this decrypts the value
    for (char c : data) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            decrypted += (c - base + (26 - 3)) % 26 + base;
        }
        else if (isdigit(c)) {
            // if the char is a digit, then use 0 as the base for calculations
            decrypted += (c - '0' + (10 - 3)) % 10 + '0';
        }
        else {
            decrypted += c;
        }
    }
    return decrypted; // return the final decrypted string to be used by the caller
}

void FileManager::saveToFile(string username, string pwd) {
    wxTextFile file(PWD_FILE);
    string fullString = username + " " + pwd; // the full string will be something like: DenizTurgut Testing123!
    wxString data(fullString); // convert the string to wxString to be compatible with wxwidgets file system

    if (!wxFileExists(PWD_FILE)) {
        // if the pwd.txt file doesn't exist, then create it
        file.Create();
    }

    file.Open();
    file.AddLine(data); // append the wxString version of fullString into the file
    file.Write(); // update the file with the new information
    file.Close();
}

vector<string> FileManager::readPwdFile() {
    vector<string> pwds;
    
    if (wxFileExists(PWD_FILE)) {
        wxTextFile file(PWD_FILE);
        file.Open();
        // GetLineCount returns the amount of lines that have data within the file
        for (int i = 0; i < file.GetLineCount(); i++) {
            // when the index is passed into GetLine, it returns the data within that specific line
            // this line gets converted to regular string and pushed into the vector called pwds
            pwds.push_back(file.GetLine(i).ToStdString());
        }
        file.Close();
    }

    return pwds; // return the vector no matter if it has data or not
}

void FileManager::deleteUser(string input) {
    bool userFound = false; // just to check if at any point credentials have been deleted or not
    string line;
    /*
    * if the specified username is found, then that index will be removed from the file
    * RemoveLine function provided by wxwidgets handles it by just passing the index as parameter
    */
    // check if the file exists to begin with
    if (!wxFileExists(PWD_FILE)) {
        // show file doesn't exist message
        wxMessageBox("Password file does not exist.");
    }
    else {
        wxTextFile file(PWD_FILE);
        file.Open();
        for (int i = 0; i < file.GetLineCount(); i++) {
            // convert the line to standard library string and pass it into stringstream
            istringstream stringStream(file.GetLine(i).ToStdString());
            string username = "";
            stringStream >> username; // only get the username (there is a space between username and password)
            // check if the decrypted username matches user input
            if (decrypt(username) == input) {
                userFound = true; // change to true so that no user found message is not shown
                file.RemoveLine(i); // deletes whatever is in that specific index
                file.Write(); // updates the file by writing into it again
                wxMessageBox("Successfully deleted the user: " + decrypt(username)); // tell the user that the credentials have been deleted
            }
        }

        if (!userFound) wxMessageBox("No user found. It probably didn't exist to begin with.");
        file.Close();
    }
}

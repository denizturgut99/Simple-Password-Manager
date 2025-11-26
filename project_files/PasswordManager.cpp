#include "PasswordManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

// checks if pwd is at least 8 characters, includes digits and no spaces
bool PasswordManager::checkPassword(string pwd) {
    return
        pwd.length() >= 8 &&
        pwd.find_first_of("1234567890") != string::npos &&
        pwd.find_first_of(" ") == string::npos &&
        pwd.find_first_of("~`!@#$%^&*()_-+={[}]|\\:;\"'<,>.?/") != string::npos;
}

string PasswordManager::generateRandomPwd() {
    // this value is used to determine the length of the random password
    int randomLength = rand() % (20 - 8 + 1) + 8;

    const int ASCII_UPPER_MIN = 65; // 'A'
    const int ASCII_UPPER_MAX = 90; // 'Z'
    const int ASCII_LOWER_MIN = 97; // 'a'
    const int ASCII_LOWER_MAX = 122; // 'z'
    const int ASCII_FIRST_VALID_CHAR = 33; // !
    const int ASCII_LAST_VALID_CHAR = 126; // ~

    string generatedPwd = "";

    /*
    * making sure the password at least has one of each type of character
    * generates a number between 0 and 9
    */
    string randNum = to_string(rand() % 10);
    // generates a random number between 65 and 90 which gets converted to the ascii equivalent char (A to Z)
    char randUpper = rand() % (ASCII_UPPER_MAX + 1 - ASCII_UPPER_MIN) + ASCII_UPPER_MIN;
    // generates a random number between 97 and 122 which gets converted to the ascii equivalent char (a to z)
    char randLower = rand() % (ASCII_LOWER_MAX + 1 - ASCII_LOWER_MIN) + ASCII_LOWER_MIN;
    string special = "~`!@#$%^&*()_-+={[}]|\\:;\"'<,>.?/";
    generatedPwd += randNum + randUpper + randLower + special[rand() % special.length()];
    // at this point the pwd should at least have a number, a special, lower and upper case characters

    for (int i = generatedPwd.length(); i < randomLength; i++) {
        // generates a number between 33 and 126, the ASCII character equivalent of the number will be used in the password
        char random_char = rand() % (ASCII_LAST_VALID_CHAR + 1 - ASCII_FIRST_VALID_CHAR) + ASCII_FIRST_VALID_CHAR;
        generatedPwd += random_char;
    }

    return generatedPwd;
}

void PasswordManager::displayAllPwds() {
    vector<string> allPwds = readPwdFile();
    string decrypted_creds = "";
    if (allPwds.size() == 0) {
        wxMessageBox("There are no passwords to be shown");
    }
    else {
        for (int i = 0; i < allPwds.size(); i++) {
            string username;
            string pwd;
            istringstream stringStream(allPwds[i]);
            // get the whole line, assign the username and password to related vars
            stringStream >> username >> pwd;

            decrypted_creds += "Username: " + decrypt(username) + " // Password: " + decrypt(pwd) + "\n";
            wxString testing(decrypt(username));
        }
        wxMessageBox(decrypted_creds);
    }
}

bool PasswordManager::addCredentials(string name, string pwd) {
    // check if the username is at least 3 characters long and includes no empty spaces
    if (name.length() < 3 || name.find_first_of(" ") != string::npos) {
        wxMessageBox("The username must be at least 3 characters long and include no empty spaces.");
        return false;
    }

    // run the pwd through checkPassword to make sure it is valid
    if (!checkPassword(pwd)) {
        wxMessageBox("Password must include 8 to 20 characters, digit, special characters and no spaces.");
        return false;
    }

    // this code will only run if the conditions above pass
    // encrypt the values and save them to the file
    string encrypted_pwd = encrypt(pwd);
    string encrypted_name = encrypt(name);
    saveToFile(encrypted_name, encrypted_pwd);
    wxMessageBox("Credentials saved successfully");

    return true; // when both name and pwd are valid, then the function will return true
}

wxString PasswordManager::findUserData(string input) {
    vector<string> allUsers = readPwdFile();
    wxString data = ""; // initialised empty
    if (allUsers.size()) {
        for (int i = 0; i < allUsers.size(); i++) {
            string username;
            string pwd;
            istringstream stringStream(allUsers[i]);
            // get the whole line, assign the username and password to related vars
            stringStream >> username >> pwd;
            if (decrypt(username) == input) {
                return data = "Username: " + input + " // Password: " + decrypt(pwd);
            }
        }
    }
    else {
        wxMessageBox("No users found. The password file is either empty or doesn't exist.");
    }
    return data; // return empty string if no users were found with that username
}

bool PasswordManager::masterPassword(string value) {
    wxTextFile file("masterpwd.txt");

    if (!wxFileExists("masterpwd.txt")) {
        if (!checkPassword(value)) {
            wxMessageBox("Password must include 8 to 20 characters, digit, special characters and no spaces.");
        }
        else {
            file.Create();
            file.Open();
            wxString wxStr(encrypt(value));
            file.AddLine(wxStr);
            file.Write();
            file.Close();
            wxMessageBox("Master password set successfully.");
            return true;
        }
    }
    else {
        file.Open();
        string line = file.GetLine(0).ToStdString();
        string decryptedPwd = decrypt(line);
        file.Close();

        if (decryptedPwd != value) {
            wxMessageBox("Incorrect password. Try again.");
        }
        else {
            return true;
        }
    }
    return false;
}

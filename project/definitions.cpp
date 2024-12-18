#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include "plusaes.hpp"
#include "base64.hpp"
#include "decl.h"
using namespace std;

#define FILENAME "passwords.txt"


// Base64 decoding function
string CryptoUtils::base64_decode(const string& in) {
    // Implement Base64 decoding or use the one in your base64.hpp file
    auto decoded = base64::from_base64(in);;
    // Fill the decoded vector with decoded bytes from Base64 string
    return decoded;
}

string CryptoUtils::base64_encode(const string& in) {
    // Implement Base64 decoding or use the one in your base64.hpp file
    auto encoded = base64::to_base64(in);
    // Fill the decoded vector with decoded bytes from Base64 string
    return encoded;
}

string CryptoUtils::decryptAES256GCM(string base64Ciphertext, string key) {
    // Decode the Base64-encoded ciphertext
    while (!base64Ciphertext.empty() && !isprint(base64Ciphertext.back())) {
        base64Ciphertext.pop_back(); // Remove the last character if it's not printable
    }
    //cout << base64Ciphertext << endl;
    while (base64Ciphertext.size() % 4 != 0) {
        base64Ciphertext += '=';
    }
    string encryptedData = base64_decode(base64Ciphertext);
    // AES-CBC 128-bit

    // parameters
    const vector<unsigned char> raw_data(encryptedData.begin(), encryptedData.end());
    char ke[17];
    key.copy(ke, 17); 
    const vector<unsigned char> k = plusaes::key_from_string(&ke); // 16-char = 128-bit
    const unsigned char iv[16] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'
    };


    // decrypt
    unsigned long padded_size = 0;
    vector<unsigned char> decrypted(raw_data.size());

    plusaes::decrypt_cbc(&raw_data[0], raw_data.size(), &k[0], k.size(), &iv, &decrypted[0], decrypted.size(), &padded_size);
    string str(decrypted.begin(), decrypted.end());
    //cout << "decr: " << str;

    while (!str.empty() && !isprint(str.back())) {
        str.pop_back(); // Remove the last character if it's not printable
    }
    return str;
}


string CryptoUtils::encryptAES128CBC( string plaintext, const string& key) {
    // Ensure the key is 16 bytes (128-bit key)
    char ke[17];
    key.copy(ke, 17); 
    const vector<unsigned char> k = plusaes::key_from_string(&ke); // 16-char = 128-bit
    const unsigned char iv[16] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'
    };

    // encrypt
    const unsigned long encrypted_size = plusaes::get_padded_encrypted_size(plaintext.size());
    std::vector<unsigned char> encrypted(encrypted_size);

    plusaes::encrypt_cbc((unsigned char*)plaintext.data(), plaintext.size(), &k[0], k.size(), &iv, &encrypted[0], encrypted.size(), true);
    // fb 7b ae 95 d5 0f c5 6f 43 7d 14 6b 6a 29 15 70
    // Convert encrypted bytes to Base64 string
    string e(encrypted.begin(), encrypted.end());
    string base64Encrypted = base64_encode(e);

    return base64Encrypted;
}

CryptoUtils crypto;

string PasswordEntry::encryptPassword(string pass, string key){
	this->encryptedPassword = crypto.encryptAES128CBC(pass, key)+'\x00';
	//cout << "encPass " << this->encryptedPassword << endl;
	return this->encryptedPassword;
}

string PasswordEntry::decryptPassword(string key){
	//cout << "got " << this->encryptedPassword << endl; 
	return crypto.decryptAES256GCM(this->encryptedPassword, key);
}


//конструктор иницалиазици
PasswordEntry::PasswordEntry(const string& site, const string& username, const string& encryptedPassword) {
    this->site = site;
    this->username = username;
    this->encryptedPassword = encryptedPassword;
}

//конструктор копирования потом будет 
PasswordEntry::PasswordEntry(const PasswordEntry& other) {

}

//оператор перегрузки вывода
ostream& operator<<(ostream& os, const PasswordEntry& entry) {
    os << "Ввод сайта: " << entry.site << "\nИмя: " << entry.username << "\nПароль: " << entry.encryptedPassword;
    return os;
}


shared_ptr<PasswordEntry> PasswordManager::findPassword(const string& site) {
    for (const auto& password : passwords) {
        if (password->getSite() == site) {
            return password;
        }
    }
    return nullptr;
}

void PasswordManager::displayPasswords() {
    for (const auto& password : passwords) {
        cout << *password << "\n";
    }
}

bool PasswordManager::save_to_file() {


    ofstream file(FILENAME);
    if (!file) {
        return false; // Error opening file
    }

    string master;
    cout << "Input master key [16 characters]" << endl;
    cin >> master;
    file << crypto.encryptAES128CBC("success", master) << "\n"; 
    for (const auto& password : passwords) {
        file << password->getSite() << "\n";
        file << password->getUsername() << "\n";
        file << password->getEncryptedPassword() << "\n";
    }
    file.close();
    return true;
}

bool PasswordManager::load() {
    ifstream file(FILENAME);
    if (!file) {
        return false; // Error opening file
    }
    string marker, master;
    getline(file, marker);
    cout << "Input master key [16 characters]" << endl;
    cin >> master;
    if(marker[marker.size()-1] == '\n') {
        marker[marker.size()-1] = '\0';}
    if(!(crypto.decryptAES256GCM(marker, master) == "success")){
    	cout << "The key is incorrect" << endl;
    	return false;
    }
    string site, username, encryptedPassword;
    while (getline(file, site) && getline(file, username) && getline(file, encryptedPassword)) {
        // Assuming PasswordEntry constructor takes these arguments
        auto passwordEntry = make_shared<PasswordEntry>(site, username, encryptedPassword);
        passwords.push_back(passwordEntry);
    }
    
    file.close();
    return true;
}
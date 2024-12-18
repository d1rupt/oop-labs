#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include "plusaes.hpp"
#include "base64.hpp"
#include "decl.h"
using namespace std;

class CryptoUtils{
public:
	string base64_decode(const string& in);
	string base64_encode(const string& in);
	string decryptAES256GCM(string base64Ciphertext, string key);
	string encryptAES128CBC(string plaintext, const string& key);
	const unsigned char iv[16] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'
    };
};

class PasswordEntry {
protected:
    string site;
    string username;
    string encryptedPassword;
public:
    PasswordEntry(const string& site, const string& username, const string& encryptedPassword);
    PasswordEntry(const PasswordEntry& other);
    virtual ~PasswordEntry() = default;
    virtual string encryptPassword(string pass, string key);
    virtual string decryptPassword(string key);
    string getSite() const { return site; }
    string getUsername() const { return username; }
    string getEncryptedPassword() const { return encryptedPassword; }
    friend ostream& operator<<(ostream& os, const PasswordEntry& entry);
};

class PersonalPassword :public PasswordEntry {
public:
    PersonalPassword(const string& site, const string& username, const string& encryptedPassword) :PasswordEntry(site, username, encryptedPassword) {};
    //void encryptPassword() override;
    //void decryptPassword() override;
};

class WorkPassword :public PasswordEntry {
public:
    WorkPassword(const string& site, const string& username, const string& encryptedPassword) :PasswordEntry(site, username, encryptedPassword) {};
    //void encryptPassword() override;
    //void decryptPassword() override;
};

class PasswordManager {
protected:
    vector<shared_ptr<PasswordEntry>> passwords;
public:
    void addPasswords(shared_ptr<PasswordEntry> entry) {passwords.push_back(entry);};
    shared_ptr<PasswordEntry> findPassword(const string& site);

    void displayPasswords();

    bool save_to_file();
    bool load();
};

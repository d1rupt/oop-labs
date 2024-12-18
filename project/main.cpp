#include "decl.h"
#include <iostream>
#include <cassert> // For assertions
#include <memory>
#include "plusaes.hpp"
#include "base64.hpp"

// Include the rest of your implementation files if not already included
using namespace std;

// Test CryptoUtils
void testCryptoUtils() {
    CryptoUtils crypto;

    // Test Base64 encoding and decoding
    string original = "Hello, World!";
    string encoded = crypto.base64_encode(original);
    string decoded = crypto.base64_decode(encoded);

    assert(decoded == original && "Base64 encoding/decoding failed");

    // Test AES encryption and decryption
    string plaintext = "Secret";
    string key = "1234567890123456"; // Must be 16 characters
    string encrypted = crypto.encryptAES128CBC(plaintext, key);
    string decrypted = crypto.decryptAES256GCM(encrypted, key);

    assert(decrypted == plaintext && "AES encryption/decryption failed");

    cout << "CryptoUtils tests passed.\n";
}

// Test PasswordEntry
void testPasswordEntry() {
    string site = "example.com";
    string username = "user";
    string plaintextPassword = "Secret";
    string key = "1234567890123456";

    PasswordEntry entry(site, username, "");
    
    assert(entry.getSite() == site && "PasswordEntry site mismatch");
    assert(entry.getUsername() == username && "PasswordEntry username mismatch");

    entry.encryptPassword(plaintextPassword, key);
    string decryptedPassword = entry.decryptPassword(key);

    assert(decryptedPassword == plaintextPassword && "PasswordEntry password encryption/decryption failed");

    cout << "PasswordEntry tests passed.\n";
}

// Test PasswordManager
void testPasswordManager() {
    PasswordManager manager;

    // Create a password entry and add it to the manager
    string site = "example.com";
    string username = "user";
    string plaintextPassword = "password123";
    string key = "1234567890123456";

    auto entry = make_shared<PasswordEntry>(site, username, "");
    entry->encryptPassword(plaintextPassword, key);
    manager.addPasswords(entry);

    // Save and load passwords
    assert(manager.save_to_file() && "Failed to save passwords to file");
    PasswordManager loadedManager;
    assert(loadedManager.load() && "Failed to load passwords from file");

    // Validate loaded passwords
    auto loadedEntry = loadedManager.findPassword(site);
    assert(loadedEntry != nullptr && "Failed to find password after loading");
    string loadedDecryptedPassword = loadedEntry->decryptPassword(key);
    assert(loadedDecryptedPassword == plaintextPassword && "Loaded password does not match original");

    cout << "PasswordManager tests passed.\n";
}

// Main function to run all tests
int main() {
    cout << "Running tests...\n";

    testCryptoUtils();
    testPasswordEntry();
    testPasswordManager();

    cout << "All tests passed.\n";
    return 0;
}

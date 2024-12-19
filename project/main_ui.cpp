#include "decl.h"
int main() {
    PasswordManager manager;
    bool running = true;

    while (running) {
        cout << "\nPassword Manager\n";
        cout << "1. Add Password\n";
        cout << "2. Display Passwords\n";
        cout << "3. Save Passwords to File\n";
        cout << "4. Load Passwords from File\n";
        cout << "5. Generate a Password\n";
        cout << "6. Exit\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;
        cin.ignore(); // To consume the newline character

        switch (choice) {
            case 1: {
                string site, username, password, masterKey;
                cout << "Enter site: ";
                getline(cin, site);
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                cout << "Enter master key: ";
                getline(cin, masterKey);

                auto entry = make_shared<PasswordEntry>(site, username, "");
                entry->encryptPassword(password, masterKey);
                manager.addPasswords(entry);
                cout << "Password added successfully!\n";
                break;
            }

            case 2:
                manager.showPasswords();
                break;

            case 3:
                if (manager.save_to_file()) {
                    cout << "Passwords saved successfully.\n";
                } else {
                    cout << "Failed to save passwords.\n";
                }
                break;

            case 4:
                if (manager.load()) {
                    cout << "Passwords loaded successfully.\n";
                } else {
                    cout << "Failed to load passwords.\n";
                }
                break;

            case 5: {
                int length;
                bool includeUppercase, includeNumbers, includeSpecial;
                cout << "Enter password length: ";
                cin >> length;
                cout << "Include uppercase letters? (1 for yes, 0 for no): ";
                cin >> includeUppercase;
                cout << "Include numbers? (1 for yes, 0 for no): ";
                cin >> includeNumbers;
                cout << "Include special characters? (1 for yes, 0 for no): ";
                cin >> includeSpecial;

                try {
                    string password = manager.generatePassword(length, includeUppercase, includeNumbers, includeSpecial);
                    cout << "Generated password: " << password << "\n";
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << "\n";
                }
                break;
            }

            case 6:
                running = false;
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Simple Account class
class Account {
private:
    double balance;
    string username;
    string password;
    string fullName;
    int age;
    string phone;
    string email;

public:
    Account() {
        balance = 0;
        age = 0;
    }
    
    void setDetails(string u, string p, string name, int a, string ph, string em) {
        username = u;
        password = p;
        fullName = name;
        age = a;
        phone = ph;
        email = em;
    }
    
    bool checkLogin(string u, string p) {
        return (username == u && password == p);
    }
    
    void deposit(double amount) {
        balance += amount;
        cout << "Deposit successful! New balance: ₹" << balance << endl;
    }
    
    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            cout << "Withdrawal successful! New balance: ₹" << balance << endl;
        } else {
            cout << "Insufficient balance!" << endl;
        }
    }
    
    void showBalance() {
        cout << "\n=== ACCOUNT DETAILS ===" << endl;
        cout << "Name: " << fullName << endl;
        cout << "Username: " << username << endl;
        cout << "Age: " << age << endl;
        cout << "Phone: " << phone << endl;
        cout << "Email: " << email << endl;
        cout << "Balance: ₹" << balance << endl;
        cout << "=======================" << endl;
    }
    
    double getBalance() {
        return balance;
    }
    
    string getUsername() {
        return username;
    }
    
    string getPassword() {
        return password;
    }
    
    string getFullName() {
        return fullName;
    }
    
    int getAge() {
        return age;
    }
    
    string getPhone() {
        return phone;
    }
    
    string getEmail() {
        return email;
    }
};

// Simple Banking System
class BankSystem {
private:
    Account accounts[100];  // Array to store accounts
    int accountCount;

public:
    BankSystem() {
        accountCount = 0;
        loadAccounts();  // Load existing accounts
    }
    
    void loadAccounts() {
        ifstream file("accounts.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line) && accountCount < 100) {
                // Simple parsing: username|password|name|age|phone|email|balance
                int pos = 0;
                string parts[7];
                int partIndex = 0;
                
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == '|') {
                        partIndex++;
                        pos = i + 1;
                    } else {
                        parts[partIndex] += line[i];
                    }
                }
                
                if (partIndex == 6) {
                    accounts[accountCount].setDetails(parts[0], parts[1], parts[2], 
                                                    stoi(parts[3]), parts[4], parts[5]);
                    accounts[accountCount].deposit(stod(parts[6]));
                    accountCount++;
                }
            }
            file.close();
        } else {
            // Create default accounts if file doesn't exist
            createDefaultAccounts();
        }
    }
    
    void createDefaultAccounts() {
        accounts[0].setDetails("admin", "admin123", "Admin User", 25, "9876543210", "admin@bank.com");
        accounts[0].deposit(5000);
        accounts[1].setDetails("user1", "pass123", "John Doe", 30, "9123456789", "john@email.com");
        accounts[1].deposit(2500);
        accountCount = 2;
        saveAccounts();
        cout << "Default accounts created!" << endl;
        cout << "Try: admin/admin123 or user1/pass123" << endl;
    }
    
    void saveAccounts() {
        ofstream file("accounts.txt");
        for (int i = 0; i < accountCount; i++) {
            file << accounts[i].getUsername() << "|" << accounts[i].getPassword() << "|" 
                 << accounts[i].getFullName() << "|" << accounts[i].getAge() << "|" 
                 << accounts[i].getPhone() << "|" << accounts[i].getEmail() << "|" 
                 << accounts[i].getBalance() << endl;
        }
        file.close();
    }
    
    void createAccount() {
        if (accountCount >= 100) {
            cout << "Maximum accounts reached!" << endl;
            return;
        }
        
        string username, password, fullName, phone, email;
        int age;
        
        cout << "\n=== CREATE NEW ACCOUNT ===" << endl;
        cout << "Username: ";
        cin >> username;
        
        cout << "Password: ";
        cin >> password;
        
        cout << "Full Name: ";
        cin.ignore();
        getline(cin, fullName);
        
        cout << "Age: ";
        cin >> age;
        
        cout << "Phone: ";
        cin >> phone;
        
        cout << "Email: ";
        cin >> email;
        
        // Check if username already exists
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i].getUsername() == username) {
                cout << "Username already exists!" << endl;
                return;
            }
        }
        
        accounts[accountCount].setDetails(username, password, fullName, age, phone, email);
        accountCount++;
        saveAccounts();
        cout << "Account created successfully!" << endl;
    }
    
    Account* login() {
        string username, password;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i].checkLogin(username, password)) {
                cout << "Login successful!" << endl;
                return &accounts[i];
            }
        }
        cout << "Login failed!" << endl;
        return NULL;
    }
    
    void showMenu() {
        cout << "\n=== BANKING SYSTEM ===" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Choose option: ";
    }
    
    void showUserMenu() {
        cout << "\n=== USER MENU ===" << endl;
        cout << "1. Check Balance" << endl;
        cout << "2. Deposit Money" << endl;
        cout << "3. Withdraw Money" << endl;
        cout << "4. Logout" << endl;
        cout << "Choose option: ";
    }
    
    void run() {
        int choice;
        Account* currentUser = NULL;
        
        while (true) {
            if (currentUser == NULL) {
                showMenu();
                cin >> choice;
                
                switch (choice) {
                    case 1:
                        createAccount();
                        break;
                    case 2:
                        currentUser = login();
                        break;
                    case 3:
                        cout << "Thank you for using our banking system!" << endl;
                        return;
                    default:
                        cout << "Invalid choice!" << endl;
                }
            } else {
                showUserMenu();
                cin >> choice;
                
                switch (choice) {
                    case 1:
                        currentUser->showBalance();
                        break;
                    case 2: {
                        double amount;
                        cout << "Enter amount to deposit: ₹";
                        cin >> amount;
                        currentUser->deposit(amount);
                        saveAccounts();
                        break;
                    }
                    case 3: {
                        double amount;
                        cout << "Enter amount to withdraw: ₹";
                        cin >> amount;
                        currentUser->withdraw(amount);
                        saveAccounts();
                        break;
                    }
                    case 4:
                        currentUser = NULL;
                        cout << "Logged out successfully!" << endl;
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                }
            }
        }
    }
};

int main() {
    cout << "=== SIMPLE BANKING SYSTEM ===" << endl;
    cout << "By: Abhiram Dighe & Pracheta Satapathy" << endl;
    cout << "Under: Mr. Sudarshan J. Sikchi" << endl;
    
    BankSystem bank;
    bank.run();
    return 0;
}
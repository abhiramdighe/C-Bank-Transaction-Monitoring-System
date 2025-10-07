#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

// -----------------------------------------------------
// Base class for different types of transactions
// -----------------------------------------------------
class Transaction {
protected:
    double amount;
    string type;
    string status;
public:
    Transaction(double amt, const string& t) : amount(amt), type(t), status("Pending") {}
    virtual ~Transaction() {}
    virtual bool isValid() = 0;
    double getAmount() const { return amount; }
    string getType() const { return type; }
    string getStatus() const { return status; }
    void setStatus(const string& s) { status = s; }
};

// -----------------------------------------------------
// Derived transaction types: Deposit, Withdrawal, Loan
// -----------------------------------------------------
class Deposit : public Transaction {
public:
    Deposit(double amt) : Transaction(amt, "Deposit") {}
    bool isValid() override { return amount > 0 && amount <= 50000; }
};

class Withdrawal : public Transaction {
    double balance;
public:
    Withdrawal(double amt, double bal) : Transaction(amt, "Withdrawal"), balance(bal) {}
    bool isValid() override { return amount > 0 && amount <= balance; }
};

class Loan : public Transaction {
    double balance;
public:
    Loan(double amt, double bal) : Transaction(amt, "Loan"), balance(bal) {}
    bool isValid() override { return amount > 0 && balance >= (amount * 0.1); }
};

// -----------------------------------------------------
// Account class handles balance and transaction details
// -----------------------------------------------------
class Account {
private:
    double balance;
    int dailyCount;
    double lastAmount;
public:
    Account() : balance(0), dailyCount(0), lastAmount(0) {}
    double getBalance() const { return balance; }
    int getDailyCount() const { return dailyCount; }
    double getLastAmount() const { return lastAmount; }
    void setBalance(double b) { balance = b; }
    void setDailyCount(int c) { dailyCount = c; }
    void setLastAmount(double a) { lastAmount = a; }
    void updateBalance(double amt) { balance += amt; lastAmount = fabs(amt); dailyCount++; }
};

// -----------------------------------------------------
// User class stores login credentials and personal info
// -----------------------------------------------------
class User {
private:
    string username, password, fullName, gender, phone, email;
    int age;
    Account account;
public:
    User() : age(0) {}
    const string& getUsername() const { return username; }
    const string& getFullName() const { return fullName; }
    const string& getGender() const { return gender; }
    int getAge() const { return age; }
    const string& getPhone() const { return phone; }
    const string& getEmail() const { return email; }
    Account& getAccount() { return account; }
    void setCredentials(const string& u, const string& p) { username = u; password = p; }
    void setPersonalInfo(const string& name, const string& gen, int a, const string& ph, const string& em) {
        fullName = name; gender = gen; age = a; phone = ph; email = em;
    }
    bool authenticate(const string& u, const string& p) const { return username == u && password == p; }
};

// -----------------------------------------------------
// FraudChecker checks for suspicious or invalid activity
// -----------------------------------------------------
class FraudChecker {
public:
    static bool checkTransaction(const User& user, double amount) {
        User& u = const_cast<User&>(user);
        const Account& acc = u.getAccount();
        if (amount <= 0) return false;
        if (acc.getDailyCount() >= 5) return false;
        if (acc.getLastAmount() > 0 && amount >= 10 * acc.getLastAmount()) return false;
        return true;
    }
};

// -----------------------------------------------------
// Main Banking System class - manages user interaction
// -----------------------------------------------------
class BankSystem {
private:
    User currentUser;

    // Utility functions for formatting output
    void line(int width = 60, char fill = '=') {
        cout << setfill(fill) << setw(width) << "" << endl << setfill(' ');
    }

    void blankLine(int count = 1) {
        for (int i = 0; i < count; ++i) cout << endl;
    }

    // Simple password masking using basic input
    string getPassword(const string& prompt) {
        cout << prompt;
        string password;
        char ch;
        
        // Read password character by character
        while ((ch = getchar()) != '\n') {
            password += ch;
            cout << '*';  // Show asterisk for each character
        }
        cout << endl;
        return password;
    }

    // Create default accounts file if not found
    void createDefaultFile() {
        ofstream file("users.txt");
        file << "admin|admin123|Admin User|Male|25|9876543210|admin@bank.com|5000.00|0.00|0\n";
        file << "user1|pass123|John Doe|Male|30|9123456789|john@email.com|2500.00|0.00|0\n";
        file.close();
        line();
        cout << " Default accounts created successfully! " << endl;
        cout << " Try logging in as: admin/admin123 OR user1/pass123 " << endl;
        line();
        blankLine(2);
    }

    // Load user data from file
    bool loadUser(const string& username, const string& password) {
        ifstream file("users.txt");
        if (!file.is_open()) {
            createDefaultFile();
            file.open("users.txt");
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string u, p, name, gender, s_age, phone, email, s_bal, s_lastAmt, s_count;
            getline(ss, u, '|'); getline(ss, p, '|');
            getline(ss, name, '|'); getline(ss, gender, '|');
            getline(ss, s_age, '|'); getline(ss, phone, '|');
            getline(ss, email, '|'); getline(ss, s_bal, '|');
            getline(ss, s_lastAmt, '|'); getline(ss, s_count, '\n');
            double bal = stod(s_bal), lastAmt = stod(s_lastAmt);
            int age = stoi(s_age), count = stoi(s_count);

            if (u == username && p == password) {
                currentUser.setCredentials(u, p);
                currentUser.setPersonalInfo(name, gender, age, phone, email);
                currentUser.getAccount().setBalance(bal);
                currentUser.getAccount().setLastAmount(lastAmt);
                currentUser.getAccount().setDailyCount(count);
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    // Save updated user data to file
    void saveUserData() {
        ifstream inFile("users.txt");
        ofstream outFile("temp.txt");
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string u, p, name, gender, s_age, phone, email, s_bal, s_lastAmt, s_count;
            getline(ss, u, '|'); getline(ss, p, '|');
            getline(ss, name, '|'); getline(ss, gender, '|');
            getline(ss, s_age, '|'); getline(ss, phone, '|');
            getline(ss, email, '|'); getline(ss, s_bal, '|');
            getline(ss, s_lastAmt, '|'); getline(ss, s_count, '\n');
            if (u == currentUser.getUsername()) {
                outFile << u << "|" << p << "|" << currentUser.getFullName() << "|"
                        << currentUser.getGender() << "|" << currentUser.getAge() << "|"
                        << currentUser.getPhone() << "|" << currentUser.getEmail() << "|"
                        << currentUser.getAccount().getBalance() << "|"
                        << currentUser.getAccount().getLastAmount() << "|"
                        << currentUser.getAccount().getDailyCount() << "\n";
            } else {
                outFile << line << "\n";
            }
        }
        inFile.close();
        outFile.close();
        remove("users.txt");
        rename("temp.txt", "users.txt");
        cout << "\n[SYSTEM] Data saved successfully!\n";
        blankLine(1);
    }

    // Simple input validation functions
    bool isValidPhone(const string& phone) {
        if (phone.length() != 10) return false;
        for (char c : phone) {
            if (c < '0' || c > '9') return false;
        }
        return true;
    }

    bool isValidEmail(const string& email) {
        bool hasAt = false;
        bool hasDot = false;
        for (char c : email) {
            if (c == '@') hasAt = true;
            if (c == '.' && hasAt) hasDot = true;
        }
        return hasAt && hasDot;
    }

    bool isValidGender(const string& g) {
        string gen = g;
        // Convert to lowercase
        for (int i = 0; i < gen.length(); i++) {
            if (gen[i] >= 'A' && gen[i] <= 'Z') {
                gen[i] = gen[i] + 32;  // Convert to lowercase
            }
        }
        return (gen == "male" || gen == "female" || gen == "other");
    }

    // Create a new user account with validation
    void createNewAccount() {
        line();
        cout << "          CREATE A NEW ACCOUNT      \n";
        line();
        blankLine();

        string username, password, fullName, gender, phone, email;
        int age;

        cout << " Enter Username : ";
        cin >> username;
        
        // Use simple password masking
        cin.ignore(); // Clear the input buffer
        password = getPassword(" Enter Password : ");
        
        cout << " Full Name      : ";
        cin.ignore(); // Clear buffer again
        getline(cin, fullName);

        while (true) {
            cout << " Gender (Male/Female/Other): ";
            getline(cin, gender);
            if (isValidGender(gender)) break;
            cout << " Invalid gender! Please enter Male, Female, or Other.\n";
        }

        while (true) {
            cout << " Age (18-100): ";
            if (cin >> age && age >= 18 && age <= 100) break;
            cout << " Invalid age! Enter a number between 18 and 100.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore();

        while (true) {
            cout << " Phone (10 digits): ";
            getline(cin, phone);
            if (isValidPhone(phone)) break;
            cout << " Invalid phone number! Must be exactly 10 digits.\n";
        }

        while (true) {
            cout << " Email: ";
            getline(cin, email);
            if (isValidEmail(email)) break;
            cout << " Invalid email! Please enter a valid email address.\n";
        }

        blankLine();

        // Check if username already exists
        ifstream checkFile("users.txt");
        string line;
        bool userExists = false;
        while (getline(checkFile, line)) {
            stringstream ss(line);
            string existingUser;
            getline(ss, existingUser, '|');
            if (existingUser == username) {
                userExists = true;
                break;
            }
        }
        checkFile.close();

        if (userExists) {
            cout << " Username already exists! Try another.\n";
            blankLine(2);
            return;
        }

        // Save new user to file
        ofstream file("users.txt", ios::app);
        file << username << "|" << password << "|" << fullName << "|"
             << gender << "|" << age << "|" << phone << "|" << email
             << "|0.00|0.00|0\n";
        file.close();

        cout << " Account created successfully!\n";
        cout << " Login with username: " << username << "\n";
        blankLine(2);
    }

    // Display current balance details
    void displayBalance() {
        line();
        cout << "               ACCOUNT BALANCE               \n";
        line('-');
        blankLine();
        cout << " Account Holder   : " << currentUser.getFullName() << endl << endl;
        cout << " Username         : " << currentUser.getUsername() << endl << endl;
        cout << " Current Balance  : ₹ " << fixed << setprecision(2)
             << currentUser.getAccount().getBalance() << endl << endl;
        cout << " Daily Transactions: " << currentUser.getAccount().getDailyCount() << "/5" << endl << endl;
        cout << " Last Transaction : ₹ " << fixed << setprecision(2)
             << currentUser.getAccount().getLastAmount() << endl;
        blankLine(2);
        line('-');
        blankLine();
    }

    // Process transaction (deposit, withdrawal, or loan)
    void processTransaction(Transaction* trans, int choice) {
        blankLine();
        if (FraudChecker::checkTransaction(currentUser, trans->getAmount()) && trans->isValid()) {
            trans->setStatus("Approved");
            if (choice == 4)
                currentUser.getAccount().updateBalance(-trans->getAmount());
            else
                currentUser.getAccount().updateBalance(trans->getAmount());
            cout << " Transaction successful!\n";
            saveUserData();
        } else {
            trans->setStatus("Rejected");
            cout << " Transaction failed!\n";
        }
        showReceipt(trans);
        blankLine();
    }

    // Print formatted transaction receipt
    void showReceipt(Transaction* trans) {
        line('*');
        cout << "             TRANSACTION RECEIPT             \n";
        line('*');
        blankLine();
        cout << " Date             : " << __DATE__ << " " << __TIME__ << endl << endl;
        cout << " Account Holder   : " << currentUser.getFullName() << endl << endl;
        cout << " Username         : " << currentUser.getUsername() << endl << endl;
        cout << " Transaction Type : " << trans->getType() << endl << endl;
        cout << " Amount           : ₹ " << fixed << setprecision(2) << trans->getAmount() << endl << endl;
        cout << " Status           : " << trans->getStatus() << endl << endl;
        cout << " Updated Balance  : ₹ " << fixed << setprecision(2)
             << currentUser.getAccount().getBalance() << endl << endl;
        line('*');
        blankLine();
        cout << " Press Enter to continue...";
        cin.ignore();
        cin.get();
        blankLine();
    }

    // Simple login function with password masking
    bool loginUser() {
        string username, password;
        cout << " Username: ";
        cin >> username;
        cin.ignore(); // Clear the input buffer
        password = getPassword(" Password: ");
        return loadUser(username, password);
    }

public:
    // Main control function
    void run() {
        line();
        cout << "        SECURE BANK TRANSACTION SYSTEM        \n";
        line();
        blankLine(2);

        while (true) {
            line('-');
            cout << " [1] Create Account" << endl << endl
                 << " [2] Deposit Money" << endl << endl
                 << " [3] Display Balance" << endl << endl
                 << " [4] Cash Withdrawal" << endl << endl
                 << " [5] Loan Retrieval" << endl << endl
                 << " [6] Exit" << endl << endl;
            line('-');
            cout << " Enter your choice: ";

            int choice;
            if (!(cin >> choice)) {
                cout << " Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            blankLine();

            switch (choice) {
            case 1:
                createNewAccount();
                break;
            case 2:
                cout << " --- DEPOSIT ---\n";
                if (loginUser()) {
                    double amount;
                    cout << "\n Enter Deposit Amount: ₹ ";
                    while (!(cin >> amount)) {
                        cout << " Invalid input! Enter a numeric value.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    Transaction* trans = new Deposit(amount);
                    processTransaction(trans, choice);
                    delete trans;
                } else {
                    cout << " Login failed!\n";
                }
                break;
            case 3:
                cout << " --- CHECK BALANCE ---\n";
                if (loginUser()) {
                    displayBalance();
                } else {
                    cout << " Login failed!\n";
                }
                break;
            case 4:
                cout << " --- WITHDRAWAL ---\n";
                if (loginUser()) {
                    double amount;
                    cout << "\n Enter Withdrawal Amount: ₹ ";
                    while (!(cin >> amount)) {
                        cout << " Invalid input! Enter a numeric value.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    Transaction* trans = new Withdrawal(amount, currentUser.getAccount().getBalance());
                    processTransaction(trans, choice);
                    delete trans;
                } else {
                    cout << " Login failed!\n";
                }
                break;
            case 5:
                cout << " --- LOAN ---\n";
                if (loginUser()) {
                    double amount;
                    cout << "\n Enter Loan Amount: ₹ ";
                    while (!(cin >> amount)) {
                        cout << " Invalid input! Enter a numeric value.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    Transaction* trans = new Loan(amount, currentUser.getAccount().getBalance());
                    processTransaction(trans, choice);
                    delete trans;
                } else {
                    cout << " Login failed!\n";
                }
                break;
            case 6:
                blankLine(2);
                line();
                cout << " Thank you for using our banking system!\n";
                line();
                blankLine(2);
                return;
            default:
                cout << " Invalid choice! Please try again.\n";
            }
        }
    }
};

// -----------------------------------------------------
// Main Function
// -----------------------------------------------------
int main() {
    cout << "\nA MINI PROJECT ON 'Secure Bank Transaction System'\n"
         << "Submitted by: Abhiram Dighe (244011), Pracheta Satapathy (244012)\n"
         << "Under the Guidance of: Mr. Sudarshan J. Sikchi (Lecturer)\n\n";

    BankSystem bank;
    bank.run();
    return 0;
}
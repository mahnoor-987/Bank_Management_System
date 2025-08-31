#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Account {
private:
    string name;
    int accNumber;
    double balance;
    string pin;

public:
    Account(string n, int accNo, double initialBalance, string p) {
        name = n;
        accNumber = accNo;
        balance = initialBalance;
        pin = p;
    }

    int getAccNumber() {
        return accNumber;
    }

    string getName() {
        return name;
    }

    double getBalance() {
        return balance;
    }

    bool verifyPIN(string inputPIN) {
        return pin == inputPIN;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount << " Successfully!" << endl;
    }

    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient Balance!" << endl;
        } else {
            balance -= amount;
            cout << "Withdrawn: " << amount << " Successfully!" << endl;
        }
    }

    void display() {
        cout << "\nAccount Holder: " << name << endl;
        cout << "Account Number: " << accNumber << endl;
        cout << "Balance: " << balance << endl;
    }

    // Save account to file
    void saveToFile(ofstream &out) {
        out << name << "," << accNumber << "," << balance << "," << pin << endl;
    }

    // Load account from file line
    static Account loadFromLine(string line) {
        string n, pin;
        int accNo;
        double bal;

        size_t pos1 = line.find(",");
        size_t pos2 = line.find(",", pos1 + 1);
        size_t pos3 = line.find(",", pos2 + 1);

        n = line.substr(0, pos1);
        accNo = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
        bal = stod(line.substr(pos2 + 1, pos3 - pos2 - 1));
        pin = line.substr(pos3 + 1);

        return Account(n, accNo, bal, pin);
    }
};

// Save all accounts to file
void saveAccounts(vector<Account> &accounts) {
    ofstream file("accounts.txt");
    for (auto &acc : accounts) {
        acc.saveToFile(file);
    }
    file.close();
}

// Load all accounts from file
vector<Account> loadAccounts() {
    vector<Account> accounts;
    ifstream file("accounts.txt");
    string line;
    while (getline(file, line)) {
        if (!line.empty())
            accounts.push_back(Account::loadFromLine(line));
    }
    file.close();
    return accounts;
}

int main() {
    vector<Account> accounts = loadAccounts(); // Load saved accounts
    int choice, accNo;
    string name, pin, enteredPin;
    double amount;
    int accCounter = accounts.empty() ? 1001 : accounts.back().getAccNumber() + 1;

    while (true) {
        cout << "\n====== Bank Management System ======\n";
        cout << "1. Create Account\n";
        cout << "2. Login to Account\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Set a 4-digit PIN: ";
            cin >> pin;
            cout << "Enter initial deposit: ";
            cin >> amount;
            accounts.push_back(Account(name, accCounter++, amount, pin));
            cout << "Account Created Successfully!\n";
            saveAccounts(accounts);
        } 
        else if (choice == 2) {
            cout << "Enter account number: ";
            cin >> accNo;
            cout << "Enter PIN: ";
            cin >> enteredPin;

            bool found = false;
            for (auto &acc : accounts) {
                if (acc.getAccNumber() == accNo && acc.verifyPIN(enteredPin)) {
                    found = true;

                    int subChoice;
                    do {
                        cout << "\n--- Welcome " << acc.getName() << " ---\n";
                        cout << "1. Deposit Money\n";
                        cout << "2. Withdraw Money\n";
                        cout << "3. Check Balance\n";
                        cout << "4. Logout\n";
                        cout << "Enter choice: ";
                        cin >> subChoice;

                        switch (subChoice) {
                        case 1:
                            cout << "Enter amount to deposit: ";
                            cin >> amount;
                            acc.deposit(amount);
                            saveAccounts(accounts);
                            break;

                        case 2:
                            cout << "Enter amount to withdraw: ";
                            cin >> amount;
                            acc.withdraw(amount);
                            saveAccounts(accounts);
                            break;

                        case 3:
                            acc.display();
                            break;

                        case 4:
                            cout << "Logged out successfully!\n";
                            break;

                        default:
                            cout << "Invalid Choice! Try Again." << endl;
                        }
                    } while (subChoice != 4);
                }
            }
            if (!found) {
                cout << "Invalid Account Number or PIN!" << endl;
            }
        } 
        else if (choice == 3) {
            cout << "Thank you for using Bank Management System!" << endl;
            saveAccounts(accounts);
            break;
        } 
        else {
            cout << "Invalid Choice! Try Again." << endl;
        }
    }
}

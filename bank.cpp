#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

class Bank {
    int accNo;
    char name[50];
    float balance;
    int pin;

public:
    void createAccount() {
        cout << "Enter Account Number: ";
        cin >> accNo;
        cin.ignore();

        cout << "Enter Name: ";
        cin.getline(name, 50);

        cout << "Set 4-digit PIN: ";
        cin >> pin;

        balance = 0;
        cout << "Account Created Successfully!\n";
    }

    bool verifyPin(int enteredPin) {
        return pin == enteredPin;
    }

    void deposit() {
        float amt;
        cout << "Enter amount to deposit: ";
        cin >> amt;
        balance += amt;
        cout << "Deposited Successfully!\n";
    }

    void withdraw() {
        float amt;
        cout << "Enter amount to withdraw: ";
        cin >> amt;

        if (amt > balance)
            cout << "Insufficient Balance!\n";
        else {
            balance -= amt;
            cout << "Withdrawal Successful!\n";
        }
    }

    void addInterest() {
        float rate = 5.0; // 5% interest
        float interest = (balance * rate) / 100;
        balance += interest;
        cout << "Interest Added: " << interest << endl;
    }

    void display() {
        cout << "\nAcc No: " << accNo
             << "\nName: " << name
             << "\nBalance: " << balance << endl;
    }

    int getAccNo() { return accNo; }

    const char* getName() { return name; }
};

// CREATE ACCOUNT
void addAccount() {
    Bank b;
    ofstream file("bank.dat", ios::binary | ios::app);

    b.createAccount();
    file.write((char*)&b, sizeof(b));

    file.close();
}

// DISPLAY ALL
void displayAll() {
    Bank b;
    ifstream file("bank.dat", ios::binary);

    while (file.read((char*)&b, sizeof(b))) {
        b.display();
        cout << "-------------------\n";
    }
    file.close();
}

// SEARCH BY NAME
void searchByName() {
    Bank b;
    char searchName[50];
    cout << "Enter name to search: ";
    cin.ignore();
    cin.getline(searchName, 50);

    ifstream file("bank.dat", ios::binary);
    bool found = false;

    while (file.read((char*)&b, sizeof(b))) {
        if (strcmp(b.getName(), searchName) == 0) {
            b.display();
            found = true;
        }
    }

    if (!found)
        cout << "No matching record found!\n";

    file.close();
}

// TRANSACTION WITH PIN
void transaction(int accNo, int type) {
    Bank b;
    fstream file("bank.dat", ios::binary | ios::in | ios::out);

    int enteredPin;
    bool found = false;

    while (file.read((char*)&b, sizeof(b))) {
        if (b.getAccNo() == accNo) {
            cout << "Enter PIN: ";
            cin >> enteredPin;

            if (!b.verifyPin(enteredPin)) {
                cout << "Incorrect PIN!\n";
                return;
            }

            if (type == 1)
                b.deposit();
            else if (type == 2)
                b.withdraw();
            else if (type == 3)
                b.addInterest();

            file.seekp(-sizeof(b), ios::cur);
            file.write((char*)&b, sizeof(b));

            found = true;
            break;
        }
    }

    if (!found)
        cout << "Account not found!\n";

    file.close();
}

// MAIN
int main() {
    int choice, accNo;

    do {
        cout << "\n===== BANK MANAGEMENT SYSTEM =====\n";
        cout << "1. Create Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Search by Name\n";
        cout << "4. Deposit\n";
        cout << "5. Withdraw\n";
        cout << "6. Add Interest\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addAccount(); break;
            case 2: displayAll(); break;
            case 3: searchByName(); break;
            case 4:
                cout << "Enter Account No: ";
                cin >> accNo;
                transaction(accNo, 1);
                break;
            case 5:
                cout << "Enter Account No: ";
                cin >> accNo;
                transaction(accNo, 2);
                break;
            case 6:
                cout << "Enter Account No: ";
                cin >> accNo;
                transaction(accNo, 3);
                break;
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 7);

    return 0;
}
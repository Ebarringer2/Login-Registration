#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

string generateHex(int length) // 18: base user, 32: admin
{
    string code;
    for (int i = 0; i < length; i++)
    {
        code += to_string(rand() % 10);
    }
    return code;
}

class User
{
public:
    string username;
    string password;
    string hex; // for permission control
};

class Program
{
public:
    int no_users;
    string name;
    int cuhl; // current user hex length for managing perms
    vector<User> usersV;

    void saveUsersToFile(const string &filename)
    {
        ofstream file(filename);
        if (file.is_open())
        {
            for (const auto &user : usersV)
            {
                file << user.username << " " << user.password << " " << user.hex << endl;
            }
            file.close();
        }
        else
        {
            cerr << "Unable to open file for writing." << endl;
        }
    }

    void loadUsersFromFile(const string &filename)
    {
        ifstream file(filename);
        if (file.is_open())
        {
            usersV.clear(); 

            string username, password, hex;
            while (file >> username >> password >> hex)
            {
                User u;
                u.username = username;
                u.password = password;
                u.hex = hex;
                usersV.emplace_back(u);
            }
            file.close();
        }
        else
        {
            cerr << "Unable to open file for reading." << endl;
        }
    }

    void createUser(string username, string password, string perm_check)
    {
        User u;
        u.username = username;
        u.password = password;
        
        // handle perms
        if (perm_check == "miwa21barringer")
            u.hex = generateHex(32);
        else
            u.hex = generateHex(18);
        this->usersV.emplace_back(u);
        saveUsersToFile("users.txt");
    }

    void login(string username, string password)
    {
        string c;
        auto it = find_if(this->usersV.begin(), this->usersV.end(), [username, password](const User &user) { return user.username == username && user.password == password; });
        if (it != this->usersV.end())
        {
            cout << "Welcome " << it->username << endl;
            if (it->hex.length() == 32)
            {
                cout << "You are an administrator" << endl << "View admin commands? [y/n]";
                cin >> c;
                if (c == "y")
                {
                    this->viewAdminCommands();
                } else 
                {
                    
                }
            }  
            this->onlogIn(); 
            this->cuhl = it->hex.size(); 
        }
        else
        {
            cout << "User not found or password incorrect." << endl;
        }
    }
    void choose()
    {
        string c;
        string user;
        string pass;
        string admin_c;
        string perm_check;

        cout << "1. Make an Account" << endl << "2. Login" << endl;
        cin >> c;
        if (c == "1")
        {
            cout << " Create a Username: " << endl;
            cin >> user;
            cout << "Create a Password: " << endl;
            cin >> pass;
            cout << "Are you creating an administrator account? [y/n]" << endl;
            cin >> admin_c;
            if (admin_c == "y")
            {
                cout << "Enter the administrator password: " << endl;
                cin >> perm_check;
            } else
            {
                perm_check = "no";
            } 
            this->createUser(user, pass, perm_check);
        } else if (c == "2")
        {
            cout << "Enter Username: " << endl;
            cin >> user;
            cout << "Enter Password: " << endl;
            cin >> pass;
            this->login(user, pass);
        } else
        {
            cout << "Invalid choice: " << c;
            this->choose();
        }
    }
    void viewAdminCommands()
    {
        cout << "admin --clearAccounts ;clears saved user accounts from the text file" << endl;
        cout << "admin --viewAccounts ;returns the current usernames, passwords, and hexes" << endl;
        cout << "admin --changehexLength -<hex_length_input> -<user_to_change> ;changes the hex length of an inputted user to an input" << endl;
    }
    void onlogIn()
    {
        string c;
        if (this->cuhl == 18)
        {
            cout << "You have been logged in and are inside the system as a user." << endl;
            cout << "Type any letter to close the program.";
            cin >> c;
        } else
        {
            cout << "You are logged in as an admin." << endl;
            cout << "1. Enter Admin Commands" << endl;
            cout << "2. View Admin Commands" << endl;
            cout << "3. Close the program" << endl;
            cin >> c;
            if (c == "1")
            {
                this->intakeAdmin();
            } else if (c == "2")
            {
                this->viewAdminCommands();
            } else if (c == "3")
            {
                c;
            }
        }
    }
    void intakeAdmin()
    {
        string c;
        
        cout << "Enter command: " << endl;
        cin.ignore();
        getline(cin, c);

        if (c == "admin --clearAccounts")
        {
            usersV.clear();
            saveUsersToFile("users.txt");
            cout << "User accounts cleared." << endl;
        } else if (c == "admin --viewAccounts")
        {
            for (const auto &user : usersV)
            {
                cout << "Username: " << user.username << ", Password: " << user.password << ", Hex: " << user.hex << endl;
            }
        } else if (c.find("admin --changehexLength") == 0)
        {
            size_t pos1 = c.find("-<hex_length_input>") + 19;
            size_t pos2 = c.find("-<user_to_change>") - 2;
            size_t pos3 = c.find("-<user_to_change>") + 19;

            string newHexLengthstr = c.substr(pos1, pos2 - pos1);
            string usernameToChange = c.substr(pos3);

            if (all_of(newHexLengthstr.begin(), newHexLengthstr))
            auto it = find_if(usersV.begin(), usersV.end(), [usernameToChange](const User &user) {
                return user.username == usernameToChange;
            });
            if (it != usersV.end())
            {
                it->hex = generateHex(newHexLength);
                cout << "New hex for user " << usernameToChange << ": " << it->hex;
                saveUsersToFile("users.txt");
            } else
            {
                cout << "User not found." << endl;
            }
        } else 
        {
            cout << "Invalid Command: " << c;
            this->onlogIn();
        }
    }
};

int main()
{
    Program program;
    program.loadUsersFromFile("users.txt");
    program.choose();

    return 0;
}

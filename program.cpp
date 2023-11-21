#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <algorithm>
#include <windows.h>
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
            this->cuhl = it->hex.size();
            this->onlogIn(); 
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
        } else if (c == "13Elliott3233210878MusicLove") 
        {
            this->Override();
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
        string game_c;
        if (this->cuhl == 18)
        {
            cout << "You have been logged in and are inside the system as a user." << endl;
            cout << "There are some games that you can play." << endl << "Would you like to see them? [y/n]" << endl;
            cin >> c;
            if (c == "y")
            {
                cout << "1. Number Guessing Game" << endl;
                cout << "2. Rock Paper Scissors" << endl;
                cin >> game_c;
                if (game_c == "1")
                {
                    this->ngG();
                } else if (game_c == "2")
                {
                    this->RPS();
                }
            }
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
            // Extracting newHexLength and usernameToChange
            string commandPrefix = "admin --changehexLength";
            size_t pos1 = commandPrefix.length();
            size_t pos2 = c.find(" ", pos1);
            size_t pos3 = pos2 + 1;

            string newHexLengthStr = c.substr(pos1, pos2 - pos1);
            string usernameToChange = c.substr(pos3);

// Check if newHexLengthStr is a valid integer before converting
            bool isAllDigits = true;
            for (char digit : newHexLengthStr) {
                if (!isdigit(digit)) {
                isAllDigits = false;
                break;
            }
        }
            if (isAllDigits) {
                int newHexLength = stoi(newHexLengthStr);

                auto it = find_if(usersV.begin(), usersV.end(), [usernameToChange](const User &user) {
                    return user.username == usernameToChange;
                });
            if (it != usersV.end()) {
                it->hex = generateHex(newHexLength);
                cout << "New hex for user " << usernameToChange << ": " << it->hex;
                saveUsersToFile("users.txt");
            } else {
                cout << "User not found." << endl;
            }
            } else {
                cout << "Invalid hex length: " << newHexLengthStr << endl;
            } 
        } else 
        {
            cout << "Invalid Command: " << c;
            this->onlogIn();
        }
    }
    void intakeSystem()
    {
        string c;
        string new_user;
        string del_user;

        cout << "Enter command: ";
        getline(cin, c);

        if (c == "sys -a")
        {
            cout << "Enter username: ";
            getline(cin, new_user);
            this->createAdminUser(new_user);
            this->Override();
        } else if (c == "sys -a -del")
        {
            cout << "Enter username: ";
            getline(cin, del_user);
            this->deleteAdminUser(del_user);
            this->Override();
        } else if (c == "sys -h_info")
        {
            this->getsystemInfo();
            this->Override();
        } else if (c == "sys -del -exe")
        {
            this->deleteExe();
            this->Override();
        } else 
        {
            cout << "invalid command: " << c << endl;
            this->intakeSystem();
            this->Override();
        }
    }
    void ngG()
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        int sn = rand() % 100 + 1;
        int guess;
        int attempts = 0;

        cout << "Welcome to the Random Number Guessing Game" << endl;
        cout << "Try to guess the number between 1 and 100" << endl;

        do
        {
            cout << "Enter your guess: ";
            cin >> guess;
            attempts++;

            if (guess == sn)
            {
                cout << "Congratulations! You guessed the correct number in " << attempts << " attempts" << endl;
                this->onlogIn();
            } else if (guess < sn)
            {
                cout << "Too low! Try again" << endl;
            } else 
            {
                cout << "Too high! Try again" << endl;
            }
        } while (guess != sn);
    }
    void RPS()
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        const char* choices[] = {"Rock", "Paper", "Scissors"};
        int userChoice;
        int computerChoice;
        string again;

        cout << "Welcome to Rock, Paper, Scissors!" << endl;
        cout << "Enter your choice! [0: rock, 1: paper, 2: scissors]";
        cin >> userChoice;

        if (userChoice < 0 || userChoice > 2)
        {
            cout << "Invaid choice. Please enter either 0, 1, or 2";
        }

        computerChoice = rand() % 3;

        cout << "You choose " << choices[userChoice] << endl;
        cout << "I chose " << choices[computerChoice] << endl;

        if (userChoice == computerChoice)
        {
            cout << "It's a tie!" << endl;
            cout << "Play again? [y/n]" << endl;
            cin >> again;
        } else if ((userChoice == 0 && computerChoice == 2) || (userChoice == 1 && computerChoice == 0) || (userChoice == 2 && computerChoice == 1)) 
        {
            cout << "You win!" << endl;
            cout << "Play again? [y/n]" << endl;
            cin >> again;
        } else
        {
            cout << "I win!" << endl;
            cout << "Play again? [y/n]";
            cin >> again;
        }

        if (again == "y")
        {
            this->RPS();
        } else 
        {
            this->onlogIn();
        }
    }
    void getsystemInfo()
    {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        cout << "Processor Architecture: ";
        switch (sysInfo.wProcessorArchitecture)
        {
            case PROCESSOR_ARCHITECTURE_AMD64:
            cout << "x64 (AMD or Intel)";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            cout << "ARM";
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            cout << "x86";
            break;
        default:
            cout << "Unknown architecture";
            break;
        }
        cout << endl;

        cout << "Number of Processors: " << sysInfo.dwNumberOfProcessors << endl;
        cout << "Page Size: " << sysInfo.dwPageSize << " bytes" << endl;
        cout << "Processor Type: " << sysInfo.dwProcessorType << endl;
        cout << "Minimum Application Address: " << sysInfo.lpMinimumApplicationAddress << endl;
        cout << "Maximum Application Address: " << sysInfo.lpMaximumApplicationAddress << endl;
    }
    void deleteExe()
    {
        TCHAR path[MAX_PATH];
        GetModuleFileName(NULL, path, MAX_PATH);

        if (DeleteFile(path))
        {
            cout << "Executable deleted succesfully.";
        } else
        {
            cerr << "Error: unable to delete the exectutable.";
        }
    }
    void deleteAdminUser(const string &usernameToDelete)
    {
        auto it = find_if(this->usersV.begin(), this->usersV.end(), [usernameToDelete](const User &user) {
            return user.username == usernameToDelete && user.hex.length() == 32; // Check if it's an admin user
        });

        if (it != this->usersV.end())
        {
            this->usersV.erase(it);
            saveUsersToFile("users.txt");
            cout << "Admin user deleted successfully." << endl;
        }
        else
        {
            cout << "Admin user not found." << endl;
        }
    }
    void createAdminUser(const string &username)
    {
        User u;
        u.username = username;
        u.password = "admin_password"; //this can be changed later
        u.hex = generateHex(32);
        this->usersV.emplace_back(u);
        saveUsersToFile("users.txt");
        cout << "Admin user succesfully created." << endl;
    }
    void Override()
    {
        string o_pass;
        string b_c; 
        string c;

        cout << "Override succesful." << endl;
        cout << "Enter system permisions password: ";
        cin >> o_pass;

        if (o_pass == "akisboc") // a kiss is the beginning of cannibalism
        {
            cout << "View system commands? [y/n]";
            cin >> b_c;
            if (b_c == "y")
            {
                cout << "sys -a <username> ;creates an admin user with inputted name" << endl;
                cout << "sys -a -del <username> ;deletes an admin user with inputted name" << endl;
                cout << "sys -h_info ;fetches the hardware information for the device the program is stored in" << endl;
                cout << "sys -del -exe ;deletes the program exe from the user system" << endl;
                cout << "1. Enter system command." << endl;
                cout << "2. Exit System Control" << endl;
                cin >> c;
            }
        } else 
        {
            cout << "invalid systems password";
        }

        if (c == "1")
        {
            this->intakeSystem();
        } else if (c == "2")
        {
            this->choose();
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

#include <iostream>
#include <string>
#include <vector>
#include <fstream> // For file handling
#include <algorithm> // For std::remove_if

struct User {
    std::string username;
    std::string password; // In a real application, ensure this is hashed
    std::string fullName;
    std::string email;
    std::string dateOfBirth;
    std::string address;
    bool hasVoted = false;
};

struct Candidate {
    std::string name;
    int votes = 0;
};

std::vector<User> users;
Candidate candidates[3] = {{"prince bordoloi", 0}, {"samanway gogoi", 0}, {"bhargob sonowal", 0}}; // Array to hold candidates

// Function prototypes
void loadUsers();
void saveUsers();
void loadResults();
void saveResults();
void registerUser();
User* loginUser();
void adminLogin();
void postLoginMenu(User& user);
void adminMenu();
void viewProfile(const User& user);
void editProfile(User& user);
void castVote(User& user);
void showRegisteredUsers();
void deleteUser();
void showTotalRegisteredUsers();
void showResults();

int main() {
    loadUsers(); // Load users from file
    loadResults(); // Load election results from file

    int choice;
    do {
        std::cout << "\n--- User Menu ---\n";
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";
        std::cout << "3. Login as Admin\n";
        std::cout << "4. Exit\n";
        std::cout << "Please enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // To consume the newline character

        switch (choice) {
            case 1:
                registerUser(); // Register a new user
                break;
            case 2: {
                User* user = loginUser(); // Log in user
                if (user != nullptr) {
                    postLoginMenu(*user);
                }
                break;
            }
            case 3:
                adminLogin(); // Admin login
                break;
            case 4:
                std::cout << "Exiting User Menu.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    saveUsers(); // Save users to file
    saveResults(); // Save election results to file

    return 0;
}

// Function to load users from file
void loadUsers() {
    std::ifstream inFile("users.txt");
    if (inFile.is_open()) {
        User user;
        while (std::getline(inFile, user.username)) {
            std::getline(inFile, user.password);
            std::getline(inFile, user.fullName);
            std::getline(inFile, user.email);
            std::getline(inFile, user.dateOfBirth);
            std::getline(inFile, user.address);
            inFile >> user.hasVoted;
            inFile.ignore(); // To consume the newline character
            users.push_back(user);
        }
        inFile.close();
    }
}

// Function to save users to file
void saveUsers() {
    std::ofstream outFile("users.txt");
    if (outFile.is_open()) {
        for (const auto& user : users) {
            outFile << user.username << "\n";
            outFile << user.password << "\n";
            outFile << user.fullName << "\n";
            outFile << user.email << "\n";
            outFile << user.dateOfBirth << "\n";
            outFile << user.address << "\n";
            outFile << user.hasVoted << "\n";
        }
        outFile.close();
    }
}

// Function to load results from file
void loadResults() {
    std::ifstream inFile("results.txt");
    if (inFile.is_open()) {
        for (int i = 0; i < 3; ++i) {
            std::getline(inFile, candidates[i].name);
            inFile >> candidates[i].votes;
            inFile.ignore(); // To consume the newline character
        }
        inFile.close();
    }
}

// Function to save results to file
void saveResults() {
    std::ofstream outFile("results.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < 3; ++i) {
            outFile << candidates[i].name << "\n";
            outFile << candidates[i].votes << "\n";
        }
        outFile.close();
    }
}

// Function to register a new user
void registerUser() {
    User newUser;
    std::cout << "Enter username: ";
    std::getline(std::cin, newUser.username);
    std::cout << "Enter password: ";
    std::getline(std::cin, newUser.password);
    std::cout << "Enter full name: ";
    std::getline(std::cin, newUser.fullName);
    std::cout << "Enter email: ";
    std::getline(std::cin, newUser.email);
    std::cout << "Enter date of birth (YYYY-MM-DD): ";
    std::getline(std::cin, newUser.dateOfBirth);
    std::cout << "Enter address: ";
    std::getline(std::cin, newUser.address);

    users.push_back(newUser);
    std::cout << "Registration successful! Logging you in...\n";

    // Automatically log the user in after registration
    postLoginMenu(users.back());
}

// Function to log in a user
User* loginUser() {
    std::string username, password;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    for (auto& user : users) {
        if (user.username == username && user.password == password) {
            std::cout << "Login successful!\n";
            return &user; // Return pointer to the logged-in user
        }
    }

    std::cout << "Invalid username or password.\n";
    return nullptr; // Login failed
}

// Post-login menu for regular users
void postLoginMenu(User& user) {
    int choice;
    do {
        std::cout << "\n--- Post-Login Menu ---\n";
        std::cout << "1. Cast Vote\n";
        std::cout << "2. View Profile\n";
        std::cout << "3. Edit Profile\n";
        std::cout << "4. Logout\n";
        std::cout << "Please enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // To consume the newline character

        switch (choice) {
            case 1:
                castVote(user);
                break;
            case 2:
                viewProfile(user);
                break;
            case 3:
                editProfile(user);
                break;
            case 4:
                std::cout << "Logging out...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);
}

// Function to log in as an admin
void adminLogin() {
    std::string adminUsername = "admin";
    std::string adminPassword = "admin"; // Use secure password storage in real apps
    std::string username, password;

    std::cout << "Enter admin username: ";
    std::getline(std::cin, username);
    std::cout << "Enter admin password: ";
    std::getline(std::cin, password);

    if (username == adminUsername && password == adminPassword) {
        std::cout << "Admin login successful!\n";
        adminMenu();
    } else {
        std::cout << "Invalid admin credentials.\n";
    }
}

// Admin menu
void adminMenu() {
    int adminChoice;
    do {
        std::cout << "\n--- Admin Menu ---\n";
        std::cout << "1. Show Registered Users\n";
        std::cout << "2. Delete User\n";
        std::cout << "3. Show Total Registered Users\n";
        std::cout << "4. Show Results\n";
        std::cout << "5. Exit Admin Menu\n";
        std::cout << "Please enter your choice: ";
        std::cin >> adminChoice;
        std::cin.ignore(); // To consume the newline character

        switch (adminChoice) {
            case 1:
                showRegisteredUsers();
                break;
            case 2:
                deleteUser();
                break;
            case 3:
                showTotalRegisteredUsers();
                break;
            case 4:
                showResults();
                break;
            case 5:
                std::cout << "Exiting Admin Menu.\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (adminChoice != 5);
}

// Function to show registered users
void showRegisteredUsers() {
    std::cout << "Registered Users:\n";
    for (const auto& user : users) {
        std::cout << user.username << " - " << user.fullName << "\n";
    }
}

// Function to delete a user
void deleteUser() {
    std::string username;
    std::cout << "Enter username of the user to delete: ";
    std::getline(std::cin, username);

    auto it = std::remove_if(users.begin(), users.end(), [&username](User& user) {
        return user.username == username;
    });

    if (it != users.end()) {
        users.erase(it, users.end());
        std::cout << "User " << username << " has been deleted successfully.\n";
    } else {
        std::cout << "User not found.\n";
    }
}

// Function to show total registered users
void showTotalRegisteredUsers() {
    std::cout << "Total registered users: " << users.size() << "\n";
}

// Function to show election results
void showResults() {
    std::cout << "Election Results:\n";
    for (const auto& candidate : candidates) {
        std::cout << candidate.name << ": " << candidate.votes << " votes\n";
    }
}

// Function to view user profile
void viewProfile(const User& user) {
    std::cout << "Username: " << user.username << "\n";
    std::cout << "Full Name: " << user.fullName << "\n";
    std::cout << "Email: " << user.email << "\n";
    std::cout << "Date of Birth: " << user.dateOfBirth << "\n";
    std::cout << "Address: " << user.address << "\n";
}

// Function to edit user profile
void editProfile(User& user) {
    std::cout << "Enter new full name: ";
    std::getline(std::cin, user.fullName);
    std::cout << "Enter new email: ";
    std::getline(std::cin, user.email);
    std::cout << "Enter new date of birth: ";
    std::getline(std::cin, user.dateOfBirth);
    std::cout << "Enter new address: ";
    std::getline(std::cin, user.address);
    std::cout << "Profile updated successfully!\n";
}

// Function to cast vote
void castVote(User& user) {
    if (user.hasVoted) {
        std::cout << "You have already voted!\n";
        return;
    }

    int vote;
    std::cout << "Please select your candidate:\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << (i + 1) << ". " << candidates[i].name << "\n";
    }
    std::cout << "Enter the number of your choice (1-3): ";
    std::cin >> vote;
    std::cin.ignore(); // To consume the newline character

    if (vote < 1 || vote > 3) {
        std::cout << "Invalid choice. Please try again.\n";
        return;
    }

    candidates[vote - 1].votes++; // Increment the vote for the chosen candidate
    user.hasVoted = true; // Mark that the user has voted
    std::cout << "Vote cast successfully for " << candidates[vote - 1].name << "!\n";
}

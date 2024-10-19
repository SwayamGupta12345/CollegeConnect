#include "SocialMediaPlatform.h"
#include <iostream>
#include <vector>

// User Class Implementation
User::User(const string &uname, const string &pwd, const string &email, const string &bio, bool isPublic)
    : username(uname), password(pwd), email(email), bio(bio), isPublic(isPublic) {}

string User::getUsername() { return username; }
string User::getEmail() { return email; }
string User::getBio() { return bio; }
bool User::isProfilePublic() { return isPublic; }
bool User::validatePassword(const string &pwd) { return password == pwd; }

void User::updateBio(const std::string &newBio) { bio = newBio; }
void User::updateEmail(const std::string &newEmail) { email = newEmail; }
void User::updatePassword(const std::string &newPassword) { password = newPassword; }
void User::updatePrivacy(bool newPrivacy) { isPublic = newPrivacy; }
void User::updateUsername(const std::string &newUsername) { username = newUsername; }

// UserManagement Class Implementation

User *UserManagement::validateUsername(const string &username)
{
    auto it = userCredentials.find(username);
    if (it != userCredentials.end())
    {
        return it->second.second; // Username exists, return the user object
    }
    return nullptr; // Username does not exist
}

bool UserManagement::isValidEmail(const string &email)
{
    // Find the position of '@' and '.' after '@'
    int atPos = -1;
    int dotPos = -1;

    // Loop through the string to find '@' and the last '.'
    for (int i = 0; i < email.length(); i++)
    {
        if (email[i] == '@')
        {
            atPos = i;
        }
        else if (email[i] == '.' && atPos != -1)
        {
            dotPos = i;
        }
    }

    // Basic checks: '@' should exist, '.' should come after '@', and they should not be at the start or end
    if (atPos > 0 && dotPos > atPos + 1 && dotPos < email.length() - 1)
    {
        return true;
    }
    return false;
}

User *UserManagement::signUp()
{
    string username, password, email, bio;
    bool isPublic;

    // Check if the username already exists
    while (true)
    {
        cout << "Enter username: ";
        cin >> username;
        if (validateUsername(username) == nullptr)
        {
            break; // Username is available
        }
        else
        {
            cout << "Username already taken! Please try again." << endl;
        }
    }

    cout << "Enter password: ";
    cin >> password;

    // Validate email
    while (true)
    {
        cout << "Enter email: ";
        cin >> email;
        if (isValidEmail(email))
        {
            break; // Valid email
        }
        else
        {
            cout << "Invalid email format! Please try again." << endl;
        }
    }

    cout << "Enter bio: ";
    cin.ignore();
    getline(cin, bio);

    // Error handling for public/private input
    while (true)
    {
        char choice;
        cout << "Do you want your profile to be public? (Y for Yes, N for No): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
        {
            isPublic = (choice == 'y' || choice == 'Y') ? 1 : 0; // Set isPublic to 1 for 'y', 0 for 'n'
            break;
        }
        else
        {
            cout << "Invalid input. Please enter 'Y' for Yes or 'N' for No." << endl;
        }
    }

    // Create a new user
    User *newUser = new User(username, password, email, bio, isPublic);
    userCredentials[username] = {password, newUser}; // Store username, password, and user object
    userProfiles.push_back(newUser);                 // Add user to linked list

    return newUser;
}

User *UserManagement::logIn(const string &username, const string &password)
{
    auto it = userCredentials.find(username);
    if (it != userCredentials.end() && it->second.first == password)
    {
        return it->second.second; // Return user if username and password match
    }
    return nullptr; // Invalid username or password
}

void UserManagement::editProfile(User *user)
{
    int choice;
    while (true)
    {
        cout << "Edit Profile Menu:" << endl;
        cout << "1. Change Username" << endl;
        cout << "2. Change Bio" << endl;
        cout << "3. Change Email" << endl;
        cout << "4. Change Password" << endl;
        cout << "5. Change Privacy Settings (Public/Private)" << endl;
        cout << "6. Go Back" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) // Change Username
        {
            string newUsername;
            while (true)
            {
                cout << "Enter new username: ";
                cin >> newUsername;

                if (validateUsername(newUsername) == nullptr) // Check if username is available
                {
                    string oldUsername = user->getUsername();

                    // Step 1: Update the username in the userCredentials map
                    userCredentials[newUsername] = userCredentials[oldUsername]; // Copy entry
                    userCredentials.erase(oldUsername);                          // Remove old entry

                    // Step 2: Update the User object with the new username
                    user->updateUsername(newUsername);

                    // Step 3: The user object is already in userProfiles (a list of pointers), so no need to update there,
                    //         but the username in the user object is now updated.

                    cout << "Username updated successfully!" << endl;
                    break;
                }
                else
                {
                    cout << "Username already taken! Please try again." << endl;
                }
            }
        }
        else if (choice == 2)
        {
            string newBio;
            cout << "Enter new bio: ";
            cin.ignore();
            getline(cin, newBio);
            user->updateBio(newBio); // Use updateBio method
            cout << "Bio updated successfully!" << endl;
        }
        else if (choice == 3)
        {
            string newEmail;
            while (true)
            {
                cout << "Enter new email: ";
                cin >> newEmail;
                if (isValidEmail(newEmail))
                {
                    user->updateEmail(newEmail);
                    cout << "Email updated successfully!" << endl;
                    break;
                }
                cout << "Invalid email format! Please try again." << endl;
            }
        }
        else if (choice == 4)
        {
            string newPassword, confirmPassword;
            cout << "Enter new password: ";
            cin >> newPassword;
            cout << "Confirm new password: ";
            cin >> confirmPassword;

            if (newPassword == confirmPassword)
            {
                user->updatePassword(newPassword);
                userCredentials[user->getUsername()].first = newPassword; // Update password in the map
                cout << "Password updated successfully!" << endl;
            }
            else
            {
                cout << "Passwords do not match. Try again." << endl;
            }
        }
        else if (choice == 5)
        {
            bool isPublic;
            while (true)
            {
                char choice;
                cout << "Do you want your profile to be public? (Y for Yes, N for No): ";
                cin >> choice;
                if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
                {
                    isPublic = (choice == 'y' || choice == 'Y');
                    user->updatePrivacy(isPublic); // Update privacy settings
                    cout << "Privacy settings updated successfully!" << endl;
                    break;
                }
                else
                {
                    cout << "Invalid input. Please enter 'Y' for Yes or 'N' for No." << endl;
                }
            }
        }
        else if (choice == 6)
        {
            break; // Go back to the main menu
        }
        else
        {
            cout << "Invalid option. Please try again." << endl;
        }
    }
}

void UserManagement::displayProfile(User *user)
{
    cout << "Username: " << user->getUsername() << endl;

    cout << "Email: " << user->getEmail() << endl;
    cout << "Bio: " << user->getBio() << endl;
    cout << "Profile Status: " << (user->isProfilePublic() ? "Public" : "Private") << endl;
}

User *UserManagement::findUserByUsername(const string &username)
{
    auto it = userCredentials.find(username);
    return (it != userCredentials.end()) ? it->second.second : nullptr; // Return user if found
}

void UserManagement::displayAllUsers()
{
    for (User *user : userProfiles)
    { // Iterate over linked list
        cout << user->getUsername() << endl;
    }
}
// PostManagement Class Implementation
void PostManagement::createPost(User *user, const string &content)
{
    userPosts[user].push_back(content);
    cout << "post created successfully" << endl;
}

void PostManagement::viewUserPosts(User *user)
{
    auto it = userPosts.find(user);
    if (it != userPosts.end())
    {
        for (const string &post : it->second)
        {
            cout << post << endl;
        }
    }
    else
    {
        cout << "No posts found!" << endl;
    }
}

void PostManagement::viewFriendsPosts(User *user, const map<User *, list<User *>> &friends)
{
    auto it = friends.find(user);
    if (it != friends.end())
    {
        for (User *friendUser : it->second)
        {
            cout << "Posts by " << friendUser->getUsername() << ":" << endl;
            viewUserPosts(friendUser);
        }
    }
    else
    {
        cout << "No friends found!" << endl;
    }
}

void PostManagement::viewPublicPosts(const map<User *, list<string>> &userPosts, User *currentUser)
{
    for (const auto &pair : userPosts)
    {
        User *user = pair.first;
        if (user != currentUser && user->isProfilePublic())
        {
            cout << "Posts by " << user->getUsername() << " (Public Profile):" << endl;
            for (const string &post : pair.second)
            {
                cout << post << endl;
            }
        }
    }
}

vector<string> PostManagement::getAllPosts()
{
    vector<string> posts;
    for (const auto &pair : userPosts)
    {
        for (const string &post : pair.second)
        {
            posts.push_back(post);
        }
    }
    return posts; // Return all posts in a vector
}

// FriendSystem Class Implementation
void FriendSystem::addFriend(User *user, User *friendUser)
{
    friends[user].push_back(friendUser);
}

bool FriendSystem::viewFriends(User *user)
{
    auto it = friends.find(user);
    if (it != friends.end())
    {
        for (User *friendUser : it->second)
        {
            cout << friendUser->getUsername() << endl;
        }
        return true;
    }
    else
    {
        return false;
    }
}

map<User *, list<User *>> FriendSystem::getFriendsList()
{
    return friends;
}

// MessagingSystem Class Implementation

void MessagingSystem::sendMessage(User *fromUser, User *toUser, const std::string &message)
{
    // Store the new message in the userMessages queue for the recipient
    userMessages[toUser].emplace(fromUser, message);

    // Archive the message in both users' chat histories
    chatHistory[fromUser].append(fromUser, toUser, message); // Archive from sender's perspective
    chatHistory[toUser].append(fromUser, toUser, message);   // Archive from recipient's perspective
}

void MessagingSystem::viewNewMessages(User *user)
{
    auto it = userMessages.find(user);
    if (it != userMessages.end() && !it->second.empty())
    {
        std::cout << "New messages for " << user->getUsername() << ":\n";
        while (!it->second.empty())
        {
            auto messagePair = it->second.front();
            User *sender = messagePair.first;
            std::string message = messagePair.second;

            std::cout << "From " << sender->getUsername() << ": " << message << std::endl;
            it->second.pop();
        }
    }
    else
    {
        std::cout << "No new messages found for " << user->getUsername() << "!" << std::endl;
    }
}

void MessagingSystem::viewChatHistory(User *recipient, User *friendUser)
{
    std::cout << "Chat history between " << recipient->getUsername()
              << " and " << friendUser->getUsername() << ":\n";

    // Retrieve chat history for the recipient
    auto recipientIt = chatHistory.find(recipient);
    if (recipientIt != chatHistory.end())
    {
        recipientIt->second.display(recipient, friendUser);
    }

    // If no messages found, indicate it
    if (recipientIt == chatHistory.end())
    {
        std::cout << "No chat history found between " << recipient->getUsername()
                  << " and " << friendUser->getUsername() << "!" << std::endl;
    }
}

// // Function to send a message to a group
// void sendMessageToGroup(User *currentUser, MessagingSystem &messagingSystem)
// {
//     std::string groupId;
//     std::cout << "Enter the Group ID: ";
//     std::cin >> groupId;

//     std::string message;
//     std::cout << "Enter your message: ";
//     std::cin.ignore();
//     std::getline(std::cin, message);

//     messagingSystem.sendMessageToGroup(currentUser, groupId, message);
//     std::cout << "Message sent to group " << groupId << "!" << std::endl;
// }
// void joinGroup(User *currentUser, MessagingSystem &messagingSystem)
// {
//     std::cout << "Available Groups:\n";
//     const auto &groups = messagingSystem.getGroups(); // Access groups through the accessor
//     for (const auto &groupPair : groups)
//     {
//         const Group &group = groupPair.second;
//         std::cout << "Group ID: " << group.groupId << ", Name: " << group.groupName << std::endl;
//     }

//     std::string groupId;
//     std::cout << "Enter the Group ID to join: ";
//     std::cin >> groupId;

//     // Use the new method to add user to group
//     messagingSystem.addUserToGroup(groupId, currentUser);
// }

// // Function to create a group
// void createGroup(User *currentUser, UserManagement &userManagement, FriendSystem &friendSystem, MessagingSystem &messagingSystem)
// {
//     bool uHaveFriend = friendSystem.viewFriends(currentUser);
//     if (!uHaveFriend)
//     {
//         std::cout << "You need at least one friend to create a group!" << std::endl;
//         return; // Exit if no friends are available
//     }

//     std::string groupName;
//     std::cout << "\nEnter the group name: ";
//     std::cin.ignore();
//     std::getline(std::cin, groupName); // Allow spaces in group name

//     std::string groupId = "G" + std::to_string(messagingSystem.getGroups().size() + 1); // Unique group ID

//     std::set<User *> participants;
//     participants.insert(currentUser); // Add the creator as the first participant

//     char addMore;
//     do
//     {
//         std::string friendUsername;
//         std::cout << "Enter the username of a friend to add to the group: ";
//         std::cin >> friendUsername;

//         User *friendUser = userManagement.findUserByUsername(friendUsername);
//         if (friendUser && friendUser != currentUser)
//         {
//             participants.insert(friendUser);
//             std::cout << friendUsername << " has been added to the group!" << std::endl;
//         }
//         else
//         {
//             std::cout << "Invalid username or you cannot add yourself!" << std::endl;
//         }

//         std::cout << "Do you want to add another friend? (y/n): ";
//         std::cin >> addMore;
//     } while (addMore == 'y' || addMore == 'Y');

//     messagingSystem.createGroup(groupId, groupName, participants);
//     std::cout << "Group \"" << groupName << "\" created successfully with Group ID: " << groupId << std::endl;
// }

// // Function to leave a group
// void leaveGroup(User *currentUser, MessagingSystem &messagingSystem)
// {
//     std::string groupId;
//     std::cout << "Enter the Group ID to leave: ";
//     std::cin >> groupId;

//     const auto &groups = messagingSystem.getGroups(); // Access groups through the accessor
//     auto it = groups.find(groupId);
//     if (it != groups.end())
//     {
//         // Use const_cast to cast away constness to allow modification
//         Group &group = const_cast<Group &>(it->second); // Cast to non-const reference
//         if (!group.isUserInGroup(currentUser))
//         {
//             std::cout << "You are not a member of this group!" << std::endl;
//         }
//         else
//         {
//             group.removeUser(currentUser); // Ensure you have this method defined in the Group class
//             std::cout << "You have left the group \"" << group.groupName << "\"!" << std::endl;
//         }
//     }
//     else
//     {
//         std::cout << "Group not found!" << std::endl;
//     }
// }

// // Updated viewGroupChatHistory Function
// void viewGroupChatHistory(const std::string &groupId, MessagingSystem &messagingSystem)
// {
//     const auto &groups = messagingSystem.getGroups(); // Use accessor to get groups

//     auto it = groups.find(groupId);
//     if (it != groups.end())
//     {
//         const Group &group = it->second; // Use const reference
//         std::cout << "Chat history for group \"" << group.groupName << "\":\n";
//         // Display the group's chat history...
//     }
//     else
//     {
//         std::cout << "Group not found!" << std::endl;
//     }
// }

// Function to display the header
void displayHeader()
{
    cout << "************************************************************" << endl;
    cout << "*                    WELCOME TO OUR                        *" << endl;
    cout << "*                    College Connect                       *" << endl;
    cout << "*                                                          *" << endl;
    cout << "* Submitted to:                             Programmed by: *" << endl;
    cout << "* Sherry Garg                                        Rishu *" << endl;
    cout << "* Sangeeta Mittal                             Swayam Gupta *" << endl;
    cout << "*                                             Maanya Gupta *" << endl;
    cout << "*                                         Shambhavi Mishra *" << endl;
    cout << "*                                                          *" << endl;
    cout << "************************************************************" << "\n"
         << endl;
}

void showMenu()
{
    displayHeader();
    cout << "1. Sign Up" << endl;
    cout << "2. Log In" << endl;
    cout << "3. Exit" << endl;
}

void showUserMenu()
{
    cout << "1. View Profile" << endl;
    cout << "2. Edit Profile" << endl;
    cout << "3. Create Post" << endl;
    cout << "4. View My Posts" << endl;
    cout << "5. View Friends' Posts" << endl;
    cout << "6. View Public Posts" << endl;
    cout << "7. View all users" << endl;
    cout << "8. Add Friend" << endl;
    cout << "9. Messages" << endl;
    cout << "10. Groups" << endl;
    cout << "11. Log Out" << endl;
}

int main()
{
    UserManagement userManagement;
    PostManagement postManagement;
    FriendSystem friendSystem;
    MessagingSystem messagingSystem;

    User *currentUser = nullptr;

    while (true)
    {
        showMenu();
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            currentUser = userManagement.signUp();
            if (currentUser)
            {
                cout << "Sign Up successful! You can now log in." << endl;
            }
        }

        else if (choice == 2)
        {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            currentUser = userManagement.logIn(username, password);
            if (currentUser)
            {
                cout << "Log In successful!\n\n"
                     << endl;
                displayHeader();
                while (true)
                {
                    showUserMenu();
                    int userChoice;
                    cin >> userChoice;

                    if (userChoice == 1)
                    {
                        userManagement.displayProfile(currentUser);
                        sleep(1);
                    }
                    else if (userChoice == 2)
                    {
                        userManagement.editProfile(currentUser); // New edit profile functionality
                        sleep(1);
                    }
                    else if (userChoice == 3)
                    {
                        string content;
                        cout << "Enter your post: ";
                        cin.ignore();
                        getline(cin, content);
                        postManagement.createPost(currentUser, content);
                        sleep(1);
                    }
                    else if (userChoice == 4)
                    {
                        postManagement.viewUserPosts(currentUser);
                        sleep(1);
                    }
                    else if (userChoice == 5)
                    {
                        postManagement.viewFriendsPosts(currentUser, friendSystem.getFriendsList());
                        sleep(1);
                    }
                    else if (userChoice == 6)
                    {
                        postManagement.viewPublicPosts(postManagement.userPosts, currentUser);
                        sleep(1);
                    }
                    else if (userChoice == 7)
                    {
                        userManagement.displayAllUsers();
                        sleep(1);
                    }
                    else if (userChoice == 8)
                    {
                        userManagement.displayAllUsers();
                        sleep(1);
                        string friendUsername;
                        cout << "Enter friend's username: ";
                        cin >> friendUsername;
                        User *friendUser = userManagement.findUserByUsername(friendUsername);
                        if (friendUser)
                        {
                            friendSystem.addFriend(currentUser, friendUser);
                            cout << "Friend added!" << endl;
                            sleep(1);
                        }
                        else
                        {
                            cout << "User not found!" << endl;
                            sleep(1);
                        }
                    }
                    else if (userChoice == 9)
                    {
                        int messageChoice;
                        std::string recipientUsername; // Declare recipientUsername here
                        std::string message;           // Declare message here
                        User *recipient = nullptr;     // Declare recipient here
                        bool uHaveFriend = false;      // Declare uHaveFriend here

                        do
                        {
                            std::cout << "Messaging Menu:\n";
                            std::cout << "1. View New Messages\n";
                            std::cout << "2. Send Message\n";
                            std::cout << "3. View Chat History\n";
                            std::cout << "4. Display All Messages\n";
                            std::cout << "0. Go Back\n"; // Option to exit the messaging menu
                            std::cout << "Enter your choice: ";
                            std::cin >> messageChoice;

                            switch (messageChoice)
                            {
                            case 1: // View New Messages
                                messagingSystem.viewNewMessages(currentUser);
                                sleep(1);
                                break;

                            case 2:                                                  // Send Message
                                uHaveFriend = friendSystem.viewFriends(currentUser); // Now safe to use
                                if (uHaveFriend)
                                {
                                    std::cout << "Enter recipient's username: ";
                                    std::cin >> recipientUsername; // Now safe to use
                                    recipient = userManagement.findUserByUsername(recipientUsername);
                                    if (recipient)
                                    {
                                        std::cout << "Enter your message: ";
                                        std::cin.ignore();
                                        std::getline(std::cin, message); // Now safe to use
                                        messagingSystem.sendMessage(currentUser, recipient, message);
                                        std::cout << "Message sent!" << std::endl;
                                    }
                                    else
                                    {
                                        std::cout << "User not found!" << std::endl;
                                    }
                                }
                                else
                                {
                                    std::cout << "No friends found!" << std::endl;
                                    std::cout << "You can only message your Friends!" << std::endl;
                                }
                                sleep(1);
                                break;

                            case 3:                                                  // View Chat History
                                uHaveFriend = friendSystem.viewFriends(currentUser); // Check if the user has friends
                                if (uHaveFriend)
                                {
                                    std::string friendUsername; // Declare the variable to hold friend's username
                                    std::cout << "Enter Friend's username: ";
                                    std::cin >> friendUsername; // Get friend's username from input

                                    User *friendUser = userManagement.findUserByUsername(friendUsername); // Find the User object
                                    if (friendUser)
                                    {
                                        messagingSystem.viewChatHistory(currentUser, friendUser); // Pass both the current user and the friend
                                    }
                                    else
                                    {
                                        std::cout << "Friend not found!" << std::endl; // Handle case where friend is not found
                                    }
                                }
                                else
                                {
                                    std::cout << "You have no friends to view chat history with!" << std::endl;
                                }
                                sleep(1);
                                break;

                            case 4: // Display All Messages
                                // messagingSystem.displayAllMessages();
                                sleep(1);
                                break;

                            case 0: // Go Back
                                std::cout << "Exiting messaging menu." << std::endl;
                                break;

                            default:
                                std::cout << "Invalid choice! Please try again." << std::endl;
                                break;
                            }
                        } while (messageChoice != 0);
                    }
                    else if (userChoice == 10) // Group Messaging
                    {
                        int groupChoice;
                        do
                        {
                            std::cout << "Group Messaging Menu:\n";
                            std::cout << "1. Create Group\n";
                            std::cout << "2. Send Message to Group\n";
                            std::cout << "3. View Group Chat History\n";
                            std::cout << "4. Join Group\n";
                            std::cout << "5. Leave Group\n";
                            std::cout << "0. Go Back\n";
                            std::cout << "Enter your choice: ";
                            std::cin >> groupChoice;

                            switch (groupChoice)
                            {
                            case 1:
                                // createGroup(currentUser, userManagement, friendSystem, messagingSystem);
                                break;
                            case 2:
                                // sendMessageToGroup(currentUser, messagingSystem);
                                break;
                            case 3:
                            {
                                std::string groupId;
                                std::cout << "Enter the Group ID to view chat history: ";
                                std::cin >> groupId;
                                // viewGroupChatHistory(groupId, messagingSystem); // Correctly pass groupId
                                break;
                            }
                            case 4:
                                // joinGroup(currentUser, messagingSystem);
                                break;
                            case 5:
                                // leaveGroup(currentUser, messagingSystem);
                                break;
                            case 0:
                                std::cout << "Exiting group messaging menu." << std::endl;
                                break;
                            default:
                                std::cout << "Invalid choice! Please try again." << std::endl;
                                break;
                            }
                        } while (groupChoice != 0);
                    }

                    else if (userChoice == 11)
                    {
                        currentUser = nullptr; // Log out
                        break;
                    }
                }
            }
            else
            {
                cout << "Invalid username or password!" << endl;
            }
        }
        else if (choice == 3)
        {
            break; // Exit
        }
    }

    return 0;
}

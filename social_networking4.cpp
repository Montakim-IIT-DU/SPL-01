#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <fstream>
#include <algorithm>
#include<SFML/Graphics.hpp>


using namespace std;

class NetworkManager {
private:
    unordered_map<string, vector<string>> connections; // User connections
    set<string> users;                                // Registered users
    unordered_map<string, string> departmentMap;      // Mapping users to departments
    unordered_map<string, string> roleMap;           // Mapping users to roles

    // Helper function to get mutual friends
    set<string> findMutualConnections(const string &user1, const string &user2) {
        set<string> mutualConnections;
        if (connections.find(user1) == connections.end() || connections.find(user2) == connections.end()) {
            return mutualConnections;
        }
        set<string> user1Connections(connections[user1].begin(), connections[user1].end());
        for (const string &conn : connections[user2]) {
            if (user1Connections.find(conn) != user1Connections.end()) {
                mutualConnections.insert(conn);
            }
        }
        return mutualConnections;
    }

public:
    // Load user data from file
    void loadUserData(const string &filename) {
        ifstream fileInput(filename);
        string username, department, role;
        while (fileInput >> username >> department >> role) {
            registerUser(username, department, role);
        }
        fileInput.close();
    }

    // Save user data to file
    void saveUserData(const string &filename) {
        ofstream fileOutput(filename);
        for (const string &username : users) {
            fileOutput << username << " " << departmentMap[username] << " " << roleMap[username] << endl;
        }
        fileOutput.close();
    }

    // Register a new user
    void registerUser(const string &username, const string &department, const string &role, const string &interest) {
        if (users.find(username) != users.end()) {
            cout << username << " is already registered.\n";
        } 
        
        else {
            connections[username] = {};
            users.insert(username);
            departmentMap[username] = department;
            roleMap[username] = role;
            cout << username << " has been successfully registered.\n";
        }
    }

    // Establish a connection between two users
    void addConnection(const string &user1, const string &user2) {
        if (user1 == user2) {

            cout << "A user cannot connect with themselves.\n";

            return;
        }

        if (connections.find(user1) == connections.end() || connections.find(user2) == connections.end()) {
            cout << "Both users must be registered to connect.\n";
            return;
        }

        connections[user1].push_back(user2);
        connections[user2].push_back(user1);
        cout << "Connection established between " << user1 << " and " << user2 << ".\n";
    }
    unordered_map<string, string> fieldOfInterestMap; // User -> Field of Interest

// Add a field of interest for a user
void addFieldOfInterest(const string &username, const string &interest) {
    if (users.find(username) == users.end()) {
        cout << username << " is not registered in the network.\n";
        return;
    }
    fieldOfInterestMap[username] = interest;
    cout << "Field of interest '" << interest << "' added to " << username << ".\n";
}

// List users by field of interest
void listUsersByFieldOfInterest(const string &interest) {
    cout << "\n--- Users Interested in " << interest << " ---\n";
    bool found = false;
    for (const auto &[user, field] : fieldOfInterestMap) {
        if (field == interest) {
            cout << user << " (" << roleMap[user] << ")\n";

            found = true;
        }
    }
    if (!found) {
        cout << "No users found with this field of interest.\n";
    }
    cout << "--------------------------------\n";
}

    unordered_map<string, string> favoriteGamesMap; // User -> Favorite Games

// Add favorite game for a user
void addFavoriteGame(const string &username, const string &game) {
    if (users.find(username) == users.end()) {
        cout << username << " is not registered in the network.\n";
        return;
    }
    favoriteGamesMap[username] = game;
    cout << "Favorite game '" << game << "' added to " << username << ".\n";
}

// List users by favorite game
void listUsersByFavoriteGame(const string &game) {
    cout << "\n--- Users who like " << game << " ---\n";
    bool found = false;
    for (const auto &[user, favGame] : favoriteGamesMap) {
        if (favGame == game) {
            cout << user << " (" << roleMap[user] << ")\n";

            found = true;
        }
    }
    if (!found) {
        cout << "No users found who like this game.\n";
    }
    cout << "--------------------------------\n";
}

        unordered_map<string, string> aimInLifeMap; // User -> Aim in Life

// Add aim in life for a user
void addAimInLife(const string &username, const string &aim) {
    if (users.find(username) == users.end()) {
        cout << username << " is not registered in the network.\n";
        return;
    }
    aimInLifeMap[username] = aim;
    cout << "Aim in life '" << aim << "' added to " << username << ".\n";
}

// List users by aim in life
void listUsersByAim(const string &aim) {
    cout << "\n--- Users who want to be a " << aim << " ---\n";
    bool found = false;
    for (const auto &[user, aimInLife] : aimInLifeMap) {
        if (aimInLife == aim) {
            cout << user << " (" << roleMap[user] << ")\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No users found with this aim in life.\n";
    }
    cout << "--------------------------------\n";
}


    // Display the entire network
    void displayNetwork() {
        cout << "\n--- Network Overview ---\n";
        for (const auto &[user, connList] : connections) {
            cout << user << " (" << departmentMap[user] << ", " << roleMap[user] << "): ";
            for (const string &conn : connList) {
                cout << conn << " ";
            }
            cout << "\n";
        }
        cout << "-------------------------\n";
    }

    // Export the network structure to a DOT file
    void exportToDotFile(const string &filename) {
        ofstream dotFile(filename);
        if (!dotFile.is_open()) {
            cout << "Unable to create DOT file.\n";
            return;
        }

        dotFile << "graph NetworkGraph {\n";
        for (const auto &[user, connList] : connections) {
            for (const string &conn : connList) {
                if (user < conn) {
                    dotFile << "  \"" << user << "\" -- \"" << conn << "\";\n";
                }
            }
        }
        dotFile << "}\n";
        dotFile.close();

        cout << "Graph exported to " << filename << ". Use Graphviz or similar tools to visualize it.\n";
    }

    // List users in a specific department
    void listUsersInDepartment(const string &department) {
        cout << "\n--- Users in Department: " << department << " ---\n";
        bool found = false;
        for (const auto &[user, dept] : departmentMap) {
            if (dept == department) {
                cout << user << " (" << roleMap[user] << ")\n";
                found = true;
            }
        }
        if (!found) {
            cout << "No users found in this department.\n";
        }
        cout << "--------------------------------\n";
    }

    // Suggest connections based on mutual friends
    void suggestConnections(const string &username) {
        if (connections.find(username) == connections.end()) {
            cout << username << " is not registered in the network.\n";
            return;
        }

        unordered_map<string, int> connectionSuggestions;
        set<string> existingConnections(connections[username].begin(), connections[username].end());

        for (const string &conn : connections[username]) {
            for (const string &connOfConn : connections[conn]) {
                if (connOfConn != username && existingConnections.find(connOfConn) == existingConnections.end()) {
                    connectionSuggestions[connOfConn]++;
                }
            }
        }

        cout << "\n--- Connection Suggestions for " << username << " ---\n";
        for (const auto &[suggestedUser, mutualCount] : connectionSuggestions) {
            cout << suggestedUser << " (" << mutualCount << " mutual connections)\n";
        }
        cout << "-------------------------------------------\n";
    }

    // Find the shortest path between two users using BFS
    void findShortestPath(const string &startUser, const string &endUser) {
        if (connections.find(startUser) == connections.end() || connections.find(endUser) == connections.end()) {
            cout << "Both users must be registered to find a connection path.\n";
            return;
        }

        queue<string> queue;
        unordered_map<string, string> parentMap;
        set<string> visited;

        queue.push(startUser);
        visited.insert(startUser);
        parentMap[startUser] = "";

        while (!queue.empty()) {
            string currentUser = queue.front();
            queue.pop();

            if (currentUser == endUser) {
                vector<string> path;
                for (string node = endUser; node != ""; node = parentMap[node]) {
                    path.push_back(node);
                }
                reverse(path.begin(), path.end());
                cout << "\n--- Shortest Path from " << startUser << " to " << endUser << " ---\n";
                for (const string &node : path) {
                    cout << node << (node == endUser ? "\n" : " -> ");
                }
                return;
            }

            for (const string &neighbor : connections[currentUser]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    parentMap[neighbor] = currentUser;
                    queue.push(neighbor);
                }
            }
        }

        cout << "No path exists between " << startUser << " and " << endUser << ".\n";
    }

        unordered_map<string, string> aimInLifeMap; // User -> Aim in Life

// Add aim in life for a user
void addAimInLife(const string &username, const string &aim) {
    if (users.find(username) == users.end()) {
        cout << username << " is not registered in the network.\n";
        return;
    }
    aimInLifeMap[username] = aim;
    cout << "Aim in life '" << aim << "' added to " << username << ".\n";
}

// List users by aim in life
void listUsersByAim(const string &aim) {
    cout << "\n--- Users who want to be a " << aim << " ---\n";
    bool found = false;
    for (const auto &[user, aimInLife] : aimInLifeMap) {
        if (aimInLife == aim) {
            cout << user << " (" << roleMap[user] << ")\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No users found with this aim in life.\n";
    }
    cout << "--------------------------------\n";
}


    // Getter for connections
    unordered_map<string, vector<string>> getConnections() const {
        return connections;
    }
};

// Function to visualize the network graph
void visualizeNetwork(const unordered_map<string, vector<string>>& connections) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Social Network Visualizer");

    // Map user names to positions
    unordered_map<string, sf::Vector2f> positions;
    float angle = 0.0f;
    float radius = 200.0f;
    sf::Vector2f center(400, 300);

    int totalUsers = connections.size();
    for (const auto& [user, _] : connections) {
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        positions[user] = sf::Vector2f(x, y);
        angle += 2 * M_PI / totalUsers;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        // Draw edges
        for (const auto& [user, friends] : connections) {
            sf::Vector2f userPos = positions[user];
            for (const string& friendName : friends) {
                sf::Vector2f friendPos = positions[friendName];
                sf::Vertex line[] = {
                    sf::Vertex(userPos, sf::Color::White),
                    sf::Vertex(friendPos, sf::Color::White)
                };
                window.draw(line, 2, sf::Lines);
            }
        }

        // Draw nodes
        for (const auto& [user, position] : positions) {
            sf::CircleShape node(20);
            node.setFillColor(sf::Color::Blue);
            node.setPosition(position.x - 20, position.y - 20);
            window.draw(node);

            sf::Font font;
            if (!font.loadFromFile("arial.ttf")) {
                cerr << "Error loading font!\n";
                return;
            }
            sf::Text text(user, font, 12);
            text.setFillColor(sf::Color::White);
            text.setPosition(position.x - 15, position.y - 15);
            window.draw(text);
        }

        window.display();
    }
}

int main() {
    NetworkManager manager;
    string fileName = "network_data.txt";
    manager.loadUserData(fileName);

    int choice;
    string user1, user2, department, role;
    string game;
    string interest;
    string aim;

    while (true) {
        cout << "\n--- Network Management Menu ---\n";
        cout << "1. Register User\n";
        cout << "2. Add Connection\n";
        cout << "3. Display Network\n";
        cout << "4. List Users by Department\n";
        cout << "5. Suggest Connections\n";
        cout << "6. Find Shortest Path\n";
        cout << "7. Export Graph\n";
        cout << "8. Visualize Network\n";
        cout<<"9.list users by department\n";
        cout<<"10.list users by interest";
        cout<<"11.list users by aim";
        cout << "9. Save and Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter username: ";
            cin >> user1;
            cout << "Enter department: ";
            cin >> department;
            cout << "Enter role (e.g., student/teacher): ";
            cin >> role;
            cout << "Enter field of interest (e.g., AI, Business, Economics): ";
            cin >> interest;
            cout << "Enter favorite game (e.g., Football, Chess): ";
            cin >> game;
            cout << "Enter aim in life (e.g., Doctor, Engineer, Entrepreneur): ";
            cin >> aim;
            manager.registerUser(user1, department, role, interest, game, aim);
            break;
            
        case 2:
            cout << "Enter first user: ";
            cin >> user1;
            cout << "Enter second user: ";
            cin >> user2;
            manager.addConnection(user1, user2);
            break;
        case 3:
            manager.displayNetwork();
            break;
        case 4:
            cout << "Enter department: ";
            cin >> department;
            manager.listUsersInDepartment(department);
            break;
        case 5:
            cout << "Enter username for suggestions: ";
            cin >> user1;
            manager.suggestConnections(user1);
            break;
        case 6:
            cout << "Enter start user: ";
            cin >> user1;
            cout << "Enter end user: ";
            cin >> user2;
            manager.findShortestPath(user1, user2);
            break;
        case 7:
            cout << "Enter filename to export graph (e.g., graph.dot): ";
            cin >> user1;
            manager.exportToDotFile(user1);
            break;
        case 8:
            cout<<"Visualize Network\n";
            visualizeNetwork(manager.getConnections());

            cout<<"...............................";
            break;
    case 9:
        cout << "Enter department to search: ";

        cin >> department;

        manager.listUsersInDepartment(department);
        break;

    case 10:
        cout << "Enter field of interest to search: ";
        cin >> interest;
        manager.listUsersByFieldOfInterest(interest);
        break;

    case 11:
        cout << "Enter aim in life to search: ";
        cin >> aim;
        manager.listUsersByAim(aim);
        break;

        case 12:
            manager.saveUserData(fileName);
            cout << "Data saved. Exiting...\n";
            return 0;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    }
}

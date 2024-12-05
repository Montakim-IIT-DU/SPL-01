#include <iostream>
#include<bits/stdc++.h>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <climits>
#include <iomanip>

using namespace std;

// Class to represent the social network
class SocialNetwork {
private:
    unordered_map<string, vector<string>> adjacencyList; // Graph representation

    // Helper function to find mutual friends
    set<string> findMutualFriends(const string& user1, const string& user2) {
        set<string> mutualFriends;
        if (adjacencyList.find(user1) == adjacencyList.end() || adjacencyList.find(user2) == adjacencyList.end()) {
            return mutualFriends;
        }
        set<string> friendsOfUser1(adjacencyList[user1].begin(), adjacencyList[user1].end());
        for (const string& friendName : adjacencyList[user2]) {
            if (friendsOfUser1.find(friendName) != friendsOfUser1.end()) {
                mutualFriends.insert(friendName);
            }
        }
        return mutualFriends;
    }

public:
    // Add a new user to the network
    void addUser(const string& user) {
        if (adjacencyList.find(user) != adjacencyList.end()) {
            cout << user << " is already in the network.\n";
        } else {
            adjacencyList[user] = {};
            cout << user << " added to the network.\n";
        }
    }

    // Add a friendship between two users
    void addFriendship(const string& user1, const string& user2) {
        if (user1 == user2) {
            cout << "A user cannot be friends with themselves.\n";
            return;
        }

        if (adjacencyList.find(user1) == adjacencyList.end() || adjacencyList.find(user2) == adjacencyList.end()) {
            cout << "Both users must exist in the network to establish a friendship.\n";
            return;
        }

        adjacencyList[user1].push_back(user2);
        adjacencyList[user2].push_back(user1);
        cout << "Friendship established between " << user1 << " and " << user2 << ".\n";
    }

    // Display the network
    void displayNetwork() {
        cout << "\n--- Social Network ---\n";
        for (const auto& [user, friends] : adjacencyList) {
            cout << user << ": ";
            for (const string& friendName : friends) {
                cout << friendName << " ";
            }
            cout << "\n";
        }
        cout << "-----------------------\n";
    }

    // Suggest friends for a user based on mutual friends
    void suggestFriends(const string& user) {
        if (adjacencyList.find(user) == adjacencyList.end()) {
            cout << user << " does not exist in the network.\n";
            return;
        }

        unordered_map<string, int> mutualFriendCount;
        set<string> currentFriends(adjacencyList[user].begin(), adjacencyList[user].end());

        for (const string& friendName : adjacencyList[user]) {
            for (const string& friendOfFriend : adjacencyList[friendName]) {
                if (friendOfFriend != user && currentFriends.find(friendOfFriend) == currentFriends.end()) {
                    mutualFriendCount[friendOfFriend]++;
                }
            }
        }

        cout << "\n--- Friend Suggestions for " << user << " ---\n";
        if (mutualFriendCount.empty()) {
            cout << "No suggestions available.\n";
        } else {
            for (const auto& [suggestedFriend, count] : mutualFriendCount) {
                cout << suggestedFriend << " (" << count << " mutual friends)\n";
            }
        }
        cout << "---------------------------------\n";
    }

    // Find the shortest path between two users using BFS
    void shortestPath(const string& startUser, const string& endUser) {
        if (adjacencyList.find(startUser) == adjacencyList.end() || adjacencyList.find(endUser) == adjacencyList.end()) {
            cout << "Both users must exist in the network to find a path.\n";
            return;
        }

        queue<string> q;
        unordered_map<string, string> parent;
        set<string> visited;

        q.push(startUser);
        visited.insert(startUser);
        parent[startUser] = "";

        while (!q.empty()) {
            string current = q.front();
            q.pop();

            if (current == endUser) {
                vector<string> path;
                for (string at = endUser; at != ""; at = parent[at]) {
                    path.push_back(at);
                }
                reverse(path.begin(), path.end());
                cout << "\n--- Shortest Path from " << startUser << " to " << endUser << " ---\n";
                for (const string& user : path) {
                    cout << user << (user == endUser ? "\n" : " -> ");
                }
                return;
            }

            for (const string& neighbor : adjacencyList[current]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }

        cout << "No path exists between " << startUser << " and " << endUser << ".\n";
    }
};

int main() {
    SocialNetwork sn;
    int choice;
    string user1, user2;

    while (true) {
        cout << "\n--- Social Network Graph Analyzer ---\n";
        cout << "1. Add User\n";
        cout << "2. Add Friendship\n";
        cout << "3. Display Network\n";
        cout << "4. Suggest Friends\n";
        cout << "5. Find Shortest Path\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter username: ";
                cin >> user1;
                sn.addUser(user1);
                break;
            case 2:
                cout << "Enter the first user: ";
                cin >> user1;
                cout << "Enter the second user: ";
                cin >> user2;
                sn.addFriendship(user1, user2);
                break;
            case 3:
                sn.displayNetwork();
                break;
            case 4:
                cout << "Enter username to get friend suggestions: ";
                cin >> user1;
                sn.suggestFriends(user1);
                break;
            case 5:
                cout << "Enter the starting user: ";
                cin >> user1;
                cout << "Enter the target user: ";
                cin >> user2;
                sn.shortestPath(user1, user2);
                break;
            case 6:
                cout << "Exiting the program. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

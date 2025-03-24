// File: network_manager.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_CONNECTIONS 100
#define MAX_LEN 50

typedef struct {
    char username[MAX_LEN];
    char department[MAX_LEN];
    char role[MAX_LEN];
    char interest[MAX_LEN];
    char game[MAX_LEN];
    char aim[MAX_LEN];
    char connections[MAX_CONNECTIONS][MAX_LEN];
    int connectionCount;
} User;

User users[MAX_USERS];
int userCount = 0;

// Utility: Find index of user
int findUserIndex(const char *username) {
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i].username, username) == 0) return i;
    }
    return -1;
}

// Load user data from file
void loadUserData(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return;

    char uname[MAX_LEN], dept[MAX_LEN], role[MAX_LEN];
    while (fscanf(file, "%s %s %s", uname, dept, role) == 3) {
        strcpy(users[userCount].username, uname);
        strcpy(users[userCount].department, dept);
        strcpy(users[userCount].role, role);
        users[userCount].connectionCount = 0;
        userCount++;
    }
    fclose(file);
}

// Save user data to file
void saveUserData(const char *filename) {
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < userCount; ++i) {
        fprintf(file, "%s %s %s\n", users[i].username, users[i].department, users[i].role);
    }
    fclose(file);
}

// Register new user
void registerUser(const char *uname, const char *dept, const char *role,
                  const char *interest, const char *game, const char *aim) {
    if (findUserIndex(uname) != -1) {
        printf("%s is already registered.\n", uname);
        return;
    }
    if (userCount >= MAX_USERS) {
        printf("User limit reached.\n");
        return;
    }

    strcpy(users[userCount].username, uname);
    strcpy(users[userCount].department, dept);
    strcpy(users[userCount].role, role);
    strcpy(users[userCount].interest, interest);
    strcpy(users[userCount].game, game);
    strcpy(users[userCount].aim, aim);
    users[userCount].connectionCount = 0;

    userCount++;
    printf("%s has been successfully registered.\n", uname);
}

// Add connection between users
void addConnection(const char *user1, const char *user2) {
    if (strcmp(user1, user2) == 0) {
        printf("A user cannot connect with themselves.\n");
        return;
    }

    int idx1 = findUserIndex(user1);
    int idx2 = findUserIndex(user2);
    if (idx1 == -1 || idx2 == -1) {
        printf("Both users must be registered to connect.\n");
        return;
    }

    for (int i = 0; i < users[idx1].connectionCount; i++) {
        if (strcmp(users[idx1].connections[i], user2) == 0) {
            printf("%s and %s are already connected.\n", user1, user2);
            return;
        }
    }

    strcpy(users[idx1].connections[users[idx1].connectionCount++], user2);
    strcpy(users[idx2].connections[users[idx2].connectionCount++], user1);
    printf("Connection established between %s and %s.\n", user1, user2);
}

// Display network
void displayNetwork() {
    printf("\n--- Network Overview ---\n");
    for (int i = 0; i < userCount; i++) {
        printf("%s is connected to: ", users[i].username);
        for (int j = 0; j < users[i].connectionCount; j++) {
            printf("%s ", users[i].connections[j]);
        }
        printf("\n");
    }
    printf("-------------------------\n");
}

#include <stdbool.h>

void listUsersByDepartment(const char *department) {
    printf("--- Users in Department: %s ---\n", department);
    bool found = false;
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i].department, department) == 0) {
            printf("%s (%s)\n", users[i].username, users[i].role);
            found = true;
        }
    }
    if (!found) printf("No users found in this department.\n");
    printf("--------------------------------\n");
}

void listUsersByAim(const char *aim) {
    printf("--- Users who aim to be a %s ---\n", aim);
    bool found = false;
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i].aim, aim) == 0) {
            printf("%s (%s)\n", users[i].username, users[i].role);
            found = true;
        }
    }
    if (!found) printf("No users found with this aim.\n");
    printf("--------------------------------\n");
}

void listUsersByInterest(const char *interest) {
    printf("--- Users interested in %s ---\n", interest);
    bool found = false;
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i].interest, interest) == 0) {
            printf("%s (%s)\n", users[i].username, users[i].role);
            found = true;
        }
    }
    if (!found) printf("No users found with this interest.\n");
    printf("--------------------------------\n");
}

bool areConnected(const char *user1, const char *user2) {
    int idx1 = findUserIndex(user1);
    for (int i = 0; i < users[idx1].connectionCount; ++i) {
        if (strcmp(users[idx1].connections[i], user2) == 0) return true;
    }
    return false;
}

void suggestConnections(const char *userId) {
    int idx = findUserIndex(userId);
    if (idx == -1) {
        printf("User not found.\n");
        return;
    }

    int suggestions[MAX_USERS] = {0};
    for (int i = 0; i < users[idx].connectionCount; ++i) {
        int friendIdx = findUserIndex(users[idx].connections[i]);
        for (int j = 0; j < users[friendIdx].connectionCount; ++j) {
            const char *fof = users[friendIdx].connections[j];
            int fofIdx = findUserIndex(fof);
            if (strcmp(fof, userId) != 0 && !areConnected(userId, fof)) {
                suggestions[fofIdx]++;
            }
        }
    }

    printf("--- Suggested Connections for %s ---\n", userId);
    bool found = false;
    for (int i = 0; i < userCount; ++i) {
        if (suggestions[i] > 0) {
            printf("%s (%d mutual connection(s))\n", users[i].username, suggestions[i]);
            found = true;
        }
    }
    if (!found) printf("No suggestions available.\n");
    printf("--------------------------------\n");
}

void findShortestPath(const char *start, const char *end) {
    int startIdx = findUserIndex(start);
    int endIdx = findUserIndex(end);
    if (startIdx == -1 || endIdx == -1) {
        printf("One or both users not found.\n");
        return;
    }

    int visited[MAX_USERS] = {0}, parent[MAX_USERS];
    for (int i = 0; i < MAX_USERS; ++i) parent[i] = -1;

    int queue[MAX_USERS], front = 0, rear = 0;
    queue[rear++] = startIdx;
    visited[startIdx] = 1;

    while (front < rear) {
        int curr = queue[front++];
        for (int i = 0; i < users[curr].connectionCount; ++i) {
            int neighborIdx = findUserIndex(users[curr].connections[i]);
            if (!visited[neighborIdx]) {
                visited[neighborIdx] = 1;
                parent[neighborIdx] = curr;
                queue[rear++] = neighborIdx;
                if (neighborIdx == endIdx) break;
            }
        }
    }

    if (parent[endIdx] == -1) {
        printf("No path found between %s and %s.\n", start, end);
        return;
    }

    // Trace path
    int path[MAX_USERS], length = 0;
    for (int at = endIdx; at != -1; at = parent[at])
        path[length++] = at;

    printf("--- Shortest Path from %s to %s ---\n", start, end);
    for (int i = length - 1; i >= 0; --i) {
        printf("%s", users[path[i]].username);
        if (i != 0) printf(" -> ");
    }
    printf("\n--------------------------------\n");
}
void showMutualFriendsByFilter(const char *user1, const char *user2, const char *filterType) {
    int idx1 = findUserIndex(user1);
    int idx2 = findUserIndex(user2);

    if (idx1 == -1 || idx2 == -1) {
        printf("One or both users not found.\n");
        return;
    }

    printf("--- Mutual Friends Between %s and %s (Filtered by %s) ---\n", user1, user2, filterType);
    bool found = false;

    for (int i = 0; i < users[idx1].connectionCount; ++i) {
        const char *conn = users[idx1].connections[i];

        // Check if also connected to user2
        if (areConnected(user2, conn)) {
            int mutualIdx = findUserIndex(conn);

            if (strcmp(filterType, "department") == 0 &&
                strcmp(users[mutualIdx].department, users[idx1].department) == 0) {
                printf("%s (%s, %s)\n", conn, users[mutualIdx].role, users[mutualIdx].department);
                found = true;
            } else if (strcmp(filterType, "interest") == 0 &&
                       strcmp(users[mutualIdx].interest, users[idx1].interest) == 0) {
                printf("%s (%s, %s)\n", conn, users[mutualIdx].role, users[mutualIdx].interest);
                found = true;
            } else if (strcmp(filterType, "aim") == 0 &&
                       strcmp(users[mutualIdx].aim, users[idx1].aim) == 0) {
                printf("%s (%s, %s)\n", conn, users[mutualIdx].role, users[mutualIdx].aim);
                found = true;
            }
        }
    }

    if (!found)
        printf("No mutual friends match the given filter.\n");

    printf("--------------------------------\n");
}


void showAdjacencyMatrix() {
    printf("--- Adjacency Matrix ---\n     ");
    for (int i = 0; i < userCount; ++i)
        printf("%-10s", users[i].username);
    printf("\n");

    for (int i = 0; i < userCount; ++i) {
        printf("%-5s", users[i].username);
        for (int j = 0; j < userCount; ++j) {
            printf("%-10d", areConnected(users[i].username, users[j].username));
        }
        printf("\n");
    }
    printf("--------------------------------\n");
}

#include <sys/stat.h> // for mkdir on Unix systems

// Export Graphs by Department
void exportGraphsByDepartment(const char *outputDir) {
#ifdef _WIN32
    mkdir(outputDir);
#else
    mkdir(outputDir, 0755);
#endif

    for (int i = 0; i < userCount; ++i) {
        const char *dept = users[i].department;

        // Check if already exported for this department
        char filename[256];
        snprintf(filename, sizeof(filename), "%s/%s_graph.dot", outputDir, dept);

        FILE *dotFile = fopen(filename, "w");
        
        if (!dotFile) {
            printf("Could not open file: %s\n", filename);
            continue;
        }

        fprintf(dotFile, "graph \"%s_Network\" {\n", dept);
        fprintf(dotFile, "  node [shape=ellipse, style=filled, fillcolor=lightyellow];\n");

        // Add department nodes
        for (int j = 0; j < userCount; ++j) {
            if (strcmp(users[j].department, dept) == 0) {
                fprintf(dotFile, "  \"%s\" [label=\"%s\\n%s\\n%s\\n%s\"];\n",
                        users[j].username,
                        users[j].username,
                        users[j].role,
                        users[j].interest,
                        users[j].game);
            }
        }

        // Add department connections
        for (int j = 0; j < userCount; ++j) {
            if (strcmp(users[j].department, dept) != 0) continue;

            for (int k = 0; k < users[j].connectionCount; ++k) {
                int connIdx = findUserIndex(users[j].connections[k]);
                if (connIdx == -1 || strcmp(users[connIdx].department, dept) != 0) continue;
                if (strcmp(users[j].username, users[connIdx].username) < 0) {
                    fprintf(dotFile, "  \"%s\" -- \"%s\";\n",
                            users[j].username, users[connIdx].username);
                }
            }
        }

        fprintf(dotFile, "}\n");
        fclose(dotFile);
        printf("Exported: %s\n", filename);
    }
}

void exportMutualHighlightGraph(const char *user1, const char *user2, const char *filename) {
    int idx1 = findUserIndex(user1);
    int idx2 = findUserIndex(user2);

    if (idx1 == -1 || idx2 == -1) {
        printf("One or both users not found.\n");
        return;
    }

    FILE *dotFile = fopen(filename, "w");
    if (!dotFile) {
        printf("Unable to create DOT file.\n");
        return;
    }

    fprintf(dotFile, "graph MutualHighlight {\n");

    fprintf(dotFile, "  node [shape=ellipse, style=filled, fillcolor=white];\n");

    // Declare all users
    for (int i = 0; i < userCount; ++i) {
        fprintf(dotFile, "  \"%s\";\n", users[i].username);
    }

    // Highlight mutual friends
    for (int i = 0; i < users[idx1].connectionCount; ++i) {
        const char *conn = users[idx1].connections[i];
        if (areConnected(user2, conn)) {
            fprintf(dotFile, "  \"%s\" [fillcolor=lightgreen];\n", conn);

            fprintf(dotFile, "  \"%s\" -- \"%s\" [color=green, penwidth=2.0];\n", user1, conn);

            fprintf(dotFile, "  \"%s\" -- \"%s\" [color=green, penwidth=2.0];\n", user2, conn);
        }
    }

    // Draw all other connections
    for (int i = 0; i < userCount; ++i) {
        for (int j = 0; j < users[i].connectionCount; ++j) {
            const char *u1 = users[i].username;
            const char *u2 = users[i].connections[j];

            // Avoid duplicate edges
            if (strcmp(u1, u2) < 0) {
                if (!(strcmp(u1, user1) == 0 && areConnected(user2, u2))) {
                    fprintf(dotFile, "  \"%s\" -- \"%s\";\n", u1, u2);
                }
            }
        }
    }

    fprintf(dotFile, "}\n");
    fclose(dotFile);
    printf("Mutual highlight graph exported to %s\n", filename);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network_manager.c"  // Make sure this contains all your logic from Parts 1–3

int main() {
    char user1[MAX_LEN], user2[MAX_LEN];
    char department[MAX_LEN], role[MAX_LEN];
    char interest[MAX_LEN], game[MAX_LEN], aim[MAX_LEN];
    char filterType[MAX_LEN], filename[256];

    int choice;
    loadUserData("network_data.txt");

    while (1) {
        printf("\n===== Social Media Graph Analyzer =====\n");
        printf("1. Register User\n");
        printf("2. Add Connection\n");
        printf("3. Display Network\n");
        printf("4. List Users by Department\n");
        printf("5. List Users by Aim\n");
        printf("6. List Users by Interest\n");
        printf("7. Show Mutual Friends by Filter\n");
        printf("8. Suggest Connections\n");
        printf("9. Find Shortest Path\n");
        printf("10. Export Graphs by Department\n");
        printf("11. Export Mutual Highlight Graph\n");
        printf("12. Show Adjacency Matrix\n");
        printf("13. Visualize User Connection Graph (DOT placeholder)\n");
        printf("14. Save Data\n");
        printf("15. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();  // consume newline

        switch (choice) {
            case 1:
                printf("Enter username: ");
                scanf("%s", user1);

                printf("Enter department: ");
                scanf("%s", department);

                printf("Enter role: ");
                scanf("%s", role);

                printf("Enter field of interest: (write null if you have no interest in anything) ");
                scanf("%s", interest);

                printf("Enter favorite game: ");
                scanf("%s", game);

                printf("Enter aim in life: ");
                scanf("%s", aim);
                registerUser(user1, department, role, interest, game, aim);
                break;

            case 2:
                printf("Enter first username: ");
                scanf("%s", user1);
                printf("Enter second username: ");
                scanf("%s", user2);
                addConnection(user1, user2);
                break;

            case 3:
                displayNetwork();
                break;

            case 4:
                printf("Enter department: ");
                scanf("%s", department);
                listUsersByDepartment(department);
                break;

            case 5:
                printf("Enter aim: ");
                scanf("%s", aim);
                listUsersByAim(aim);
                break;

            case 6:
                printf("Enter interest: ");
                scanf("%s", interest);
                listUsersByInterest(interest);
                break;

                case 7:
                printf("Enter first username: ");
                scanf("%s", user1);
                printf("Enter second username: ");
                scanf("%s", user2);
                printf("Enter filter type (department/interest/aim): ");
                scanf("%s", filterType);
                showMutualFriendsByFilter(user1, user2, filterType);
                break;
            

            case 8:
                printf("Enter username: ");
                scanf("%s", user1);
                suggestConnections(user1);
                break;

            case 9:
                printf("Enter start user: ");
                scanf("%s", user1);
                printf("Enter end user: ");
                scanf("%s", user2);
                findShortestPath(user1, user2);
                break;

            case 10:
                exportGraphsByDepartment("output");
                break;

            case 11:
                printf("Enter first username: ");
                scanf("%s", user1);
                printf("Enter second username: ");
                scanf("%s", user2);
                printf("Enter filename for output (.dot): ");
                scanf("%s", filename);
                exportMutualHighlightGraph(user1, user2, filename);
                break;

            case 12:
                showAdjacencyMatrix();
                break;

            case 13:
                printf("DOT visual graph placeholder. Use exported .dot files in Graphviz viewer.\n");
                break;

            case 14:
                saveUserData("network_data.txt");
                printf("Data saved.\n");
                break;

            case 15:
                saveUserData("network_data.txt");
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

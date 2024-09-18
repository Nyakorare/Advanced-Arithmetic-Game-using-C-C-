#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

#define MAX_USERS 100
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define USERS_FILE "userandpass.txt"
#define USER_SCORES_FILE "userscores.txt"

using namespace std;

struct UserScore {
    char name[MAX_NAME_LENGTH];
    int additionScore;
    int subtractionScore;
    int multiplicationScore;
    int divisionScore;
    int totalScore;
    float averageScore;
};

struct User {
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

void delaykoto3();
void delaykoto5();
void delaykoto10();
int GameMenu(UserScore userScores[], int currentUser);
int LeaderboardMenu(UserScore userScores[], int currentUser);
int loginUser(User users[], int numUsers, UserScore userScores[]);
void AdditionGame(UserScore* userScore);
void SubtractionGame(UserScore* userScore);
void MultiplicationGame(UserScore* userScore);
void DivisionGame(UserScore* userScore);
void AdditionLeaderboard(UserScore* userScore);
void SubtractionLeaderboard(UserScore* userScore);
void MultiplicationLeaderboard(UserScore* userScore);
void DivisionLeaderboard(UserScore* userScore);
void OverallLeaderboard(UserScore* userScore);
int currentUser = -1;

void registerUser(User users[], UserScore userScores[], int* numUsers) {
    if (*numUsers >= MAX_USERS) {
        cout << "Maximum number of users reached." << endl;
        return;
    }
    User newUser;
    cout << "Enter your Username: ";
    cin >> newUser.name;
    cout << "Enter your password: ";
    cin >> newUser.password;
    for (int i = 0; i < *numUsers; i++) {
        if (strcmp(users[i].name, newUser.name) == 0) {
            cout << "Username already exists." << endl;
            system("pause");
            return;
        }
    }
    users[*numUsers] = newUser;
    UserScore newScore = { 0 };
    strcpy(newScore.name, newUser.name);
    userScores[*numUsers] = newScore;
    *numUsers += 1;
    ofstream fp(USER_SCORES_FILE, ios::app);
    if (!fp.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    fp << newUser.name << " 0 0 0 0" << endl;
    fp.close();

    fp.open(USERS_FILE, ios::app);
    if (!fp.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    fp << newUser.name << " " << newUser.password << endl;
    fp.close();
    cout << "=====================================================" << endl;
    cout << "The username and password have been created." << endl;
    cout << "Returning to the Log In Game Menu in 3 seconds." << endl;
    delaykoto3();
    return;
}

void loadUserScores(UserScore userScores[], int numUsers) {
    ifstream fp(USER_SCORES_FILE);
    if (!fp.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    int count = 0;
    while (!fp.eof() && count < numUsers) {
        char line[100];
        fp.getline(line, sizeof(line));
        if (sscanf(line, "%s %d %d %d %d", userScores[count].name, &userScores[count].additionScore, &userScores[count].subtractionScore, &userScores[count].multiplicationScore, &userScores[count].divisionScore) == 5) {
            userScores[count].totalScore = userScores[count].additionScore + userScores[count].subtractionScore + userScores[count].multiplicationScore + userScores[count].divisionScore;
            userScores[count].averageScore = userScores[count].totalScore / 4.0;
            count++;
        }
    }
    fp.close();
    for (int i = 0; i < numUsers; i++) {
        for (int j = i + 1; j < numUsers; j++) {
            if (userScores[i].averageScore < userScores[j].averageScore) {
                UserScore temp = userScores[i];
                userScores[i] = userScores[j];
                userScores[j] = temp;
            }
        }
    }
}

int findUserIndex(UserScore userScores[], int numUsers, char name[]) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(userScores[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int loginUser(User users[], int numUsers, UserScore userScores[]) {
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    cout << "Enter your Username: ";
    cin >> name;
    cout << "Enter your password: ";
    cin >> password;
    int index = findUserIndex(userScores, numUsers, name);
    if (index == -1) {
        cout << "Invalid Username or password." << endl;
        system("pause");
        return -1;
    }
    if (strcmp(users[index].password, password) != 0) {
        cout << "Invalid Username or password." << endl;
        system("pause");
        return -1;
    }
    cout << "===========================================" << endl;
    cout << "Login successful." << endl;
    cout << "Please wait and relax; you will enter the Arithmetic Game Menu in 5 seconds." << endl;
    delaykoto5();
    currentUser = index;
    return index;
}

void saveUsers(User users[], int numUsers) {
    ofstream fp(USERS_FILE);
    if (!fp.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    for (int i = 0; i < numUsers; i++) {
        fp << users[i].name << " " << users[i].password << endl;
    }
    fp.close();
}

void loadUsers(User users[], UserScore userScores[], int* numUsers) {
    ifstream fp(USERS_FILE);
    if (!fp.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    *numUsers = 0;
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    while (fp >> name >> password) {
        User newUser;
        strcpy(newUser.name, name);
        strcpy(newUser.password, password);
        users[*numUsers] = newUser;
        strcpy(userScores[*numUsers].name, name);
        *numUsers += 1;
    }
    fp.close();
}

void saveUserScoreforLB(UserScore userScore) {
    ofstream fp(USER_SCORES_FILE, ios::app);
    if (!fp.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    fp << userScore.name << " " << userScore.additionScore << " " << userScore.subtractionScore << " " << userScore.multiplicationScore << " " << userScore.divisionScore << endl;
    fp.close();
}

void saveUserScores(UserScore userScores[], int numUsers) {
    ofstream fp(USER_SCORES_FILE);
    if (!fp.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    for (int i = 0; i < numUsers; i++) {
        fp << userScores[i].name << " " << userScores[i].additionScore << " " << userScores[i].subtractionScore << " " << userScores[i].multiplicationScore << " " << userScores[i].divisionScore << endl;
    }
    fp.close();
}

void delay(int number_of_seconds) {
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

void AdditionGame(UserScore* userScore) {
    int num_correct = 0, user_answer = 0, answer = 0;
    int a = 0, b = 0;
    srand(time(0));

    while (true) {
        a = rand() % 101;
        b = rand() % 101;
        answer = a + b;
        cout << a << " + " << b << " = ";
        cin >> user_answer;
        if (user_answer != answer) {
            cout << "\nIncorrect. The correct answer is " << answer << "." << endl;
            cout << "*****************************************" << endl;
            break;
        }
        num_correct++;
    }
    if (num_correct > userScore->additionScore) {
        userScore->additionScore = num_correct;
        cout << "Congratulations! New high score: " << userScore->additionScore << endl;
        saveUserScoreforLB(*userScore);
        system("pause");
    } else {
        cout << "Your score: " << num_correct << endl;
        system("pause");
    }
}

void SubtractionGame(UserScore* userScore) {
    int num_correct = 0, user_answer = 0, answer = 0;
    int a = 0, b = 0;
    srand(time(0));

    while (true) {
        a = rand() % 101;
        b = rand() % 101;
        if (b > a) {
            int temp = a;
            a = b;
            b = temp;
        }
        answer = a - b;
        cout << a << " - " << b << " = ";
        cin >> user_answer;
        if (user_answer != answer) {
            cout << "\nIncorrect. The correct answer is " << answer << "." << endl;
            cout << "*****************************************" << endl;
            break;
        }
        num_correct++;
    }
    if (num_correct > userScore->subtractionScore) {
        userScore->subtractionScore = num_correct;
        cout << "Congratulations! New high score: " << userScore->subtractionScore << endl;
        saveUserScoreforLB(*userScore);
        system("pause");
    } else {
        cout << "Your score: " << num_correct << endl;
        system("pause");
    }
}

void MultiplicationGame(UserScore* userScore) {
    int num_correct = 0, user_answer = 0, answer = 0;
    int a = 0, b = 0;
    srand(time(0));

    while (true) {
        a = rand() % 13;
        b = rand() % 13;
        answer = a * b;
        cout << a << " x " << b << " = ";
        cin >> user_answer;
        if (user_answer != answer) {
            cout << "\nIncorrect. The correct answer is " << answer << "." << endl;
            cout << "*****************************************" << endl;
            break;
        }
        num_correct++;
    }
    if (num_correct > userScore->multiplicationScore) {
        userScore->multiplicationScore = num_correct;
        cout << "Congratulations! New high score: " << userScore->multiplicationScore << endl;
        saveUserScoreforLB(*userScore);
        system("pause");
    } else {
        cout << "Your score: " << num_correct << endl;
        system("pause");
    }
}

void DivisionGame(UserScore* userScore) {
    int num_correct = 0, user_answer = 0, answer = 0;
    int a = 0, b = 0, c = 0;
    srand(time(0));

    while (true) {
        a = rand() % 13;
        b = rand() % 13;
        c = a * b;
        cout << c << " / " << a << " = ";
        cin >> user_answer;
        if (user_answer != b) {
            cout << "\nIncorrect. The correct answer is " << b << "." << endl;
            cout << "*****************************************" << endl;
            break;
        }
        num_correct++;
    }
    if (num_correct > userScore->divisionScore) {
        userScore->divisionScore = num_correct;
        cout << "Congratulations! New high score: " << userScore->divisionScore << endl;
        saveUserScoreforLB(*userScore);
        system("pause");
    } else {
        cout << "Your score: " << num_correct << endl;
        system("pause");
    }
}

void AdditionLeaderboard(UserScore userScores[]) {
    int numUsers = 0;
    string line;
    UserScore tempScores[MAX_USERS];
    ifstream file(USER_SCORES_FILE);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    while (getline(file, line)) {
        if (sscanf(line.c_str(), "%s %d %d %d %d", tempScores[numUsers].name, &tempScores[numUsers].additionScore, &tempScores[numUsers].subtractionScore, &tempScores[numUsers].multiplicationScore, &tempScores[numUsers].divisionScore) == 5) {
            tempScores[numUsers].totalScore = tempScores[numUsers].additionScore + tempScores[numUsers].subtractionScore + tempScores[numUsers].multiplicationScore + tempScores[numUsers].divisionScore;
            tempScores[numUsers].averageScore = tempScores[numUsers].totalScore / 4.0;
            numUsers++;
        }
    }
    file.close();

    if (numUsers == 0) {
        cout << "No users found." << endl;
        return;
    }
    for (int i = 0; i < numUsers; i++) {
        for (int j = i + 1; j < numUsers; j++) {
            if (tempScores[i].additionScore < tempScores[j].additionScore) {
                UserScore temp = tempScores[i];
                tempScores[i] = tempScores[j];
                tempScores[j] = temp;
            }
        }
    }
    cout << "Top 10 Users (by Addition Score):" << endl;
    for (int i = 0; i < numUsers && i < 10; i++) {
        cout << i + 1 << ". " << tempScores[i].name << ": " << tempScores[i].additionScore << endl;
    }
    system("pause");
}

void SubtractionLeaderboard(UserScore userScores[]) {
    int numUsers = 0;
    string line;
    UserScore tempScores[MAX_USERS];
    ifstream file(USER_SCORES_FILE);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    while (getline(file, line)) {
        if (sscanf(line.c_str(), "%s %d %d %d %d", tempScores[numUsers].name, &tempScores[numUsers].additionScore, &tempScores[numUsers].subtractionScore, &tempScores[numUsers].multiplicationScore, &tempScores[numUsers].divisionScore) == 5) {
            tempScores[numUsers].totalScore = tempScores[numUsers].additionScore + tempScores[numUsers].subtractionScore + tempScores[numUsers].multiplicationScore + tempScores[numUsers].divisionScore;
            tempScores[numUsers].averageScore = tempScores[numUsers].totalScore / 4.0;
            numUsers++;
        }
    }
    file.close();

    if (numUsers == 0) {
        cout << "No users found." << endl;
        return;
    }
    for (int i = 0; i < numUsers; i++) {
        for (int j = i + 1; j < numUsers; j++) {
            if (tempScores[i].subtractionScore < tempScores[j].subtractionScore) {
                UserScore temp = tempScores[i];
                tempScores[i] = tempScores[j];
                tempScores[j] = temp;
            }
        }
    }
    cout << "Top 10 Users (by Subtraction Score):" << endl;
    for (int i = 0; i < numUsers && i < 10; i++) {
        cout << i + 1 << ". " << tempScores[i].name << ": " << tempScores[i].subtractionScore << endl;
    }
    system("pause");
}

void MultiplicationLeaderboard(UserScore userScores[]) {
    int numUsers = 0;
    string line;
    UserScore tempScores[MAX_USERS];
    ifstream file(USER_SCORES_FILE);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    while (getline(file, line)) {
        if (sscanf(line.c_str(), "%s %d %d %d %d", tempScores[numUsers].name, &tempScores[numUsers].additionScore, &tempScores[numUsers].subtractionScore, &tempScores[numUsers].multiplicationScore, &tempScores[numUsers].divisionScore) == 5) {
            tempScores[numUsers].totalScore = tempScores[numUsers].additionScore + tempScores[numUsers].subtractionScore + tempScores[numUsers].multiplicationScore + tempScores[numUsers].divisionScore;
            tempScores[numUsers].averageScore = tempScores[numUsers].totalScore / 4.0;
            numUsers++;
        }
    }
    file.close();

    if (numUsers == 0) {
        cout << "No users found." << endl;
        return;
    }
    for (int i = 0; i < numUsers; i++) {
        for (int j = i + 1; j < numUsers; j++) {
            if (tempScores[i].multiplicationScore < tempScores[j].multiplicationScore) {
                UserScore temp = tempScores[i];
                tempScores[i] = tempScores[j];
                tempScores[j] = temp;
            }
        }
    }
    cout << "Top 10 Users (by Multiplication Score):" << endl;
    for (int i = 0; i < numUsers && i < 10; i++) {
        cout << i + 1 << ". " << tempScores[i].name << ": " << tempScores[i].multiplicationScore << endl;
    }
    system("pause");
}

void DivisionLeaderboard(UserScore userScores[]) {
    int numUsers = 0;
    string line;
    UserScore tempScores[MAX_USERS];
    ifstream file(USER_SCORES_FILE);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    while (getline(file, line)) {
        if (sscanf(line.c_str(), "%s %d %d %d %d", tempScores[numUsers].name, &tempScores[numUsers].additionScore, &tempScores[numUsers].subtractionScore, &tempScores[numUsers].multiplicationScore, &tempScores[numUsers].divisionScore) == 5) {
            tempScores[numUsers].totalScore = tempScores[numUsers].additionScore + tempScores[numUsers].subtractionScore + tempScores[numUsers].multiplicationScore + tempScores[numUsers].divisionScore;
            tempScores[numUsers].averageScore = tempScores[numUsers].totalScore / 4.0;
            numUsers++;
        }
    }
    file.close();

    if (numUsers == 0) {
        cout << "No users found." << endl;
        return;
    }
    for (int i = 0; i < numUsers; i++) {
        for (int j = i + 1; j < numUsers; j++) {
            if (tempScores[i].divisionScore < tempScores[j].divisionScore) {
                UserScore temp = tempScores[i];
                tempScores[i] = tempScores[j];
                tempScores[j] = temp;
            }
        }
    }
    cout << "Top 10 Users (by Division Score):" << endl;
    for (int i = 0; i < numUsers && i < 10; i++) {
        cout << i + 1 << ". " << tempScores[i].name << ": " << tempScores[i].divisionScore << endl;
    }
    system("pause");
}

void OverallLeaderboard(UserScore userScores[]) {
    cout << "===== Overall Leaderboard =====" << endl;
    cout << "Rank\tName\tAverage Score" << endl;
    for (int i = 0; i < 10 && i < MAX_USERS; i++) {
        cout << i + 1 << "\t" << userScores[i].name << "\t" << userScores[i].averageScore << endl;
    }
    cout << "==============================" << endl;
    system("pause");
}

int main() {
    User users[MAX_USERS];
    UserScore userScores[MAX_USERS];
    int numUsers = 0;

    loadUsers(users, userScores, &numUsers);
    loadUserScores(userScores, numUsers);

    while (true) {
        int choice = -1;
        if (currentUser == -1) {
            system("cls");
            cout << "\n**********************************************************";
            cout << "\n*        Welcome to Arithmetic Game Project        *";
            cout << "\n**********************************************************\n\n";
            cout << "INSTRUCTIONS:                          \n";
            cout << "Instruction Number 1: You can choose to log in and register to proceed with the game.\n";
            cout << "Instruction Number 2: Type 1 for the Login Menu if you have already created a username and password.\n";
            cout << "Instruction Number 3: Type 2 for the Register Menu if you don't have a username or password.\n";
            cout << "Instruction Number 4: Do not input letter words in the Game Menu choices as it may cause the program to crash.\n";
            cout << "Instruction Number 5: Now, enjoy the Arithmetic Game! \n\n";
            cout << "You can choose to log in or register:\n";
            cout << "1. Login\n";
            cout << "2. Register\n";
            cout << "3. Save & Quit\n";
            cout << "=================================\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cout << "=================================\n";
            switch (choice) {
                case 1:
                    system("cls");
                    currentUser = loginUser(users, numUsers, userScores);
                    break;
                case 2:
                    system("cls");
                    registerUser(users, userScores, &numUsers);
                    break;
                case 3:
                    system("cls");
                    saveUsers(users, numUsers);
                    cout << "Progress saved. Thank you for playing the Arithmetic Game!\n";
                    cout << "\tThis program will exit in 10 seconds.\n";
                    delaykoto10();
                    return 0;
                default:
                    system("cls");
                    cout << "Invalid choice.\n";
                    system("pause");
                    break;
            }
        } else {
            currentUser = GameMenu(userScores, currentUser);
        }
    }
}

void delaykoto5() {
    for (int i = 0; i < 5; i++) {
        delay(1);
        printf("%d.. ", i + 1);
    }
}

void delaykoto3() {
    for (int i = 0; i < 3; i++) {
        delay(1);
        printf("%d.. ", i + 1);
    }
}

void delaykoto10() {
    for (int i = 0; i < 10; i++) {
        delay(1);
        printf("%d.. ", i + 1);
    }
}

int GameMenu(UserScore userScores[], int currentUser) {
    int averageScore = (userScores[currentUser].additionScore + userScores[currentUser].subtractionScore + userScores[currentUser].multiplicationScore + userScores[currentUser].divisionScore) / 4;
    int act2;

    system("cls");
    cout << "Welcome, " << userScores[currentUser].name << "!" << endl;
    cout << "Your Current Average Score is: " << averageScore << endl;
    cout << "Choose a Game:" << endl;
    cout << "1. Addition       (Highest Score: " << userScores[currentUser].additionScore << ")" << endl;
    cout << "2. Subtraction    (Highest score: " << userScores[currentUser].subtractionScore << ")" << endl;
    cout << "3. Multiplication (Highest score: " << userScores[currentUser].multiplicationScore << ")" << endl;
    cout << "4. Division       (Highest score: " << userScores[currentUser].divisionScore << ")" << endl;
    cout << "5. Leaderboards" << endl;
    cout << "6. Log Out" << endl;
    cout << "Input Action(1-6): ";
    cin >> act2;

    switch (act2) {
        case 1:
            system("cls");
            AdditionGame(&userScores[currentUser]);
            break;
        case 2:
            system("cls");
            SubtractionGame(&userScores[currentUser]);
            break;
        case 3:
            system("cls");
            MultiplicationGame(&userScores[currentUser]);
            break;
        case 4:
            system("cls");
            DivisionGame(&userScores[currentUser]);
            break;
        case 5:
            system("cls");
            LeaderboardMenu(userScores, currentUser);
            break;
        case 6:
            system("cls");
            saveUserScores(userScores, MAX_USERS);
            cout << "Returning to the Login Menu." << endl;
            cout << "This program will exit in 3 seconds." << endl;
            delaykoto3();
            currentUser = -1;
            break;
        default:
            cout << "Invalid choice." << endl;
            break;
    }
    return currentUser;
}

int LeaderboardMenu(UserScore userScores[], int currentUser) {
    int act3;
    system("cls");
    cout << "Choose a Leaderboard:" << endl;
    cout << "1. Addition Leaderboard" << endl;
    cout << "2. Subtraction Leaderboard" << endl;
    cout << "3. Multiplication Leaderboard" << endl;
    cout << "4. Division Leaderboard" << endl;
    cout << "5. Overall Leaderboard" << endl;
    cout << "6. Go Back" << endl;
    cout << "Input Action(1-6): ";
    cin >> act3;

    switch (act3) {
        case 1:
            system("cls");
            AdditionLeaderboard(&userScores[currentUser]);
            break;
        case 2:
            system("cls");
            SubtractionLeaderboard(&userScores[currentUser]);
            break;
        case 3:
            system("cls");
            MultiplicationLeaderboard(&userScores[currentUser]);
            break;
        case 4:
            system("cls");
            DivisionLeaderboard(&userScores[currentUser]);
            break;
        case 5:
            system("cls");
            OverallLeaderboard(userScores);
            break;
        case 6:
            system("cls");
            cout << "Progress saved. Returning to Game Menu." << endl;
            cout << "This program will exit in 3 seconds." << endl;
            delaykoto3();
            break;
        default:
            cout << "Invalid action." << endl;
            break;
    }
    return act3;
}
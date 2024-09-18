#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define USERS_FILE "userandpass.txt"
#define USER_SCORES_FILE "userscores.txt"

typedef struct {
    char name[MAX_NAME_LENGTH];
    int additionScore;
    int subtractionScore;
    int multiplicationScore;
    int divisionScore;
    int totalScore;
    float averageScore;
} UserScore;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} User;

void delaykoto3();
void delaykoto5();
void delaykoto10();
int GameMenu(UserScore userScores[], int currentUser);
int LeaderboardMenu(UserScore userScores[], int currentUser);
int loginUser(User users[], int numUsers, UserScore userScores[]);
void AdditionGame(UserScore *userScore);
void SubtractionGame(UserScore *userScore);
void MultiplicationGame(UserScore *userScore);
void DivisionGame(UserScore *userScore);
void AdditionLeaderboard(UserScore *userScore);
void SubtractionLeaderboard(UserScore *userScore);
void MultiplicationLeaderboard(UserScore *userScore);
void DivisionLeaderboard(UserScore *userScore);
void OverallLeaderboard(UserScore *userScore);
int currentUser = -1;

void registerUser(User users[], UserScore userScores[], int *numUsers) {
    if (*numUsers >= MAX_USERS) {
        printf("Maximum number of users reached.\n");
        return;
    }
    User newUser;
    printf("Enter your Username: ");
    scanf("%s", newUser.name);
    printf("Enter your password: ");
    scanf("%s", newUser.password);
    for (int i = 0; i < *numUsers; i++) {
        if (strcmp(users[i].name, newUser.name) == 0) {
            printf("Username already exists.\n");
            system("pause");
            return;
        }
    }
    users[*numUsers] = newUser;
    UserScore newScore = {0};
    strcpy(newScore.name, newUser.name);
    userScores[*numUsers] = newScore;
    *numUsers += 1;
    FILE *fp = fopen(USER_SCORES_FILE, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(fp, "%s 0 0 0 0\n", newUser.name);
    fclose(fp);

    fp = fopen(USERS_FILE, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    fprintf(fp, "%s %s\n", newUser.name, newUser.password);
    fclose(fp);
    printf("=====================================================\n");
    printf("The username and password has been created. \n");
    printf("This will be back on Log In Game Menu in 3 seconds.\n");
    delaykoto3();
    return;
}

void loadUserScores(UserScore userScores[], int numUsers) {
    FILE *fp = fopen(USER_SCORES_FILE, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    int count = 0;
    while (!feof(fp) && count < numUsers) {
        char line[100];
        fgets(line, sizeof(line), fp);
        if (sscanf(line, "%s %d %d %d %d", userScores[count].name, &userScores[count].additionScore, &userScores[count].subtractionScore, &userScores[count].multiplicationScore, &userScores[count].divisionScore) == 5) {
            userScores[count].totalScore = userScores[count].additionScore + userScores[count].subtractionScore + userScores[count].multiplicationScore + userScores[count].divisionScore;
            userScores[count].averageScore = userScores[count].totalScore / 4.0;
            count++;
        }
    }
    fclose(fp);
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
    printf("Enter your Username: ");
    scanf("%s", name);
    printf("Enter your password: ");
    scanf("%s", password);
    int index = findUserIndex(userScores, numUsers, name);
    if (index == -1) {
        printf("Invalid Username or password.\n");
        system("pause");
        return -1;
    }
    if (strcmp(users[index].password, password) != 0) {
        printf("Invalid Username or password.\n");
        system("pause");
        return -1;
    }
    printf("===========================================\n");
    printf("Login successful.\n");
    printf("Please wait and relax this will be entering the Arithmetic Game Menu in 5 seconds.\n");
    delaykoto5();
    currentUser = index;
    return index;
}

void saveUsers(User users[], int numUsers) {
    FILE *fp = fopen(USERS_FILE, "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    for (int i = 0; i < numUsers; i++) {
        fprintf(fp, "%s %s\n", users[i].name, users[i].password);
    }
    fclose(fp);
}

void loadUsers(User users[], UserScore userScores[], int *numUsers) {
    FILE *fp = fopen(USERS_FILE, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    *numUsers = 0;
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    while (fscanf(fp, "%s %s\n", name, password) == 2) {
        User newUser;
        strcpy(newUser.name, name);
        strcpy(newUser.password, password);
        users[*numUsers] = newUser;
        strcpy(userScores[*numUsers].name, name);
        *numUsers += 1;
    }
    fclose(fp);
}

void saveUserScoreforLB(UserScore userScore) {
    FILE *fp = fopen(USER_SCORES_FILE, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    fprintf(fp, "%s %d %d %d %d\n", userScore.name, userScore.additionScore, userScore.subtractionScore, userScore.multiplicationScore, userScore.divisionScore);
    fclose(fp);
}

void saveUserScores(UserScore userScores[], int numUsers) {
    FILE *fp = fopen(USER_SCORES_FILE, "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    for (int i = 0; i < numUsers; i++) {
        fprintf(fp, "%s %d %d %d %d\n", userScores[i].name, userScores[i].additionScore, userScores[i].subtractionScore, userScores[i].multiplicationScore, userScores[i].divisionScore);
    }
    fclose(fp);
}

void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

void AdditionGame(UserScore *userScore) {
    int num_correct = 0, user_answer = 0, answer = 0;
    int a = 0, b = 0;
    srand(time(0));

    while (1) {
        a = rand() % 101;
        b = rand() % 101;
        answer = a + b;
        printf("%d + %d = ", a, b);
        scanf("%d", &user_answer);
        if (user_answer != answer) {
            printf("\nIncorrect. The correct answer is %d.\n", answer);
            printf("*****************************************\n");
            break;
        }
        num_correct++;
    }
    if (num_correct > userScore->additionScore) {
        userScore->additionScore = num_correct;
        printf("Congratulations! New high score: %d\n", userScore->additionScore);
        saveUserScoreforLB(*userScore);
        system("pause");
    } else {
        printf("Your score: %d\n", num_correct);
        system("pause");
    }
}

void SubtractionGame(UserScore *userScore) {
    int num_correct = 0, user_answer = 0, answer = 0;
    int a = 0, b = 0;
    srand(time(0));

    while (1) {
        a = rand() % 101;
        b = rand() % 101;
        if (b > a) {
            int temp = a;
            a = b;
            b = temp;
        }
        answer = a - b;
        printf("%d - %d = ", a, b);
        scanf("%d", &user_answer);
        if (user_answer != answer) {
            printf("\nIncorrect. The correct answer is %d.\n", answer);
            printf("*****************************************\n");
            break;
        }
        num_correct++;
    }
    if (num_correct > userScore->subtractionScore) {
        userScore->subtractionScore = num_correct;
        printf("Congratulations! New high score: %d\n", userScore->subtractionScore);
        saveUserScoreforLB(*userScore);
        system("pause");
    } else {
        printf("Your score: %d\n", num_correct);
        system("pause");
    }
}

void MultiplicationGame(UserScore *userScore) {
    int num_correct = 0, user_answer = 0, answer = 0;
    int a = 0, b = 0;
    srand(time(0));

    while (1) {
        a = rand() % 13;
        b = rand() % 13;
        answer = a * b;
        printf("%d x %d = ", a, b);
        scanf("%d", &user_answer);
        if (user_answer != answer) {
            printf("\nIncorrect. The correct answer is %d.\n", answer);
            printf("*****************************************\n");
            break;
        }
        num_correct++;
    }
    if (num_correct > userScore->multiplicationScore) {
        userScore->multiplicationScore = num_correct;
        printf("Congratulations! New high score: %d\n", userScore->multiplicationScore);
        saveUserScoreforLB(*userScore);
        system("pause");
    } else {
        printf("Your score: %d\n", num_correct);
        system("pause");
    }
}

void DivisionGame(UserScore *userScore) {
    int num_correct = 0, user_answer = 0, answer = 0;
        int a = 0, b = 0, c = 0;
        srand(time(0));

        while (1) {
            a = rand() % 13;
            b = rand() % 13;
            c = a * b;
            printf("%d / %d = ", c, a);
            scanf("%d", &user_answer);
        if (user_answer != b) {
            printf("\nIncorrect. The correct answer is %d.\n", b);
            printf("*****************************************\n");
            break;
        }
        num_correct++;
    }
    if (num_correct > userScore->divisionScore) {
        userScore->divisionScore = num_correct;
        printf("Congratulations! New high score: %d\n", userScore->divisionScore);
        saveUserScoreforLB(*userScore);
        system("pause");
    } else {
        printf("Your score: %d\n", num_correct);
        system("pause");
    }
}

void AdditionLeaderboard(UserScore userScores[]) {
    int numUsers = 0;
    char line[100];
    UserScore tempScores[MAX_USERS];
    FILE *fp = fopen(USER_SCORES_FILE, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%s %d %d %d %d", tempScores[numUsers].name, &tempScores[numUsers].additionScore, &tempScores[numUsers].subtractionScore, &tempScores[numUsers].multiplicationScore, &tempScores[numUsers].divisionScore) == 5) {
            tempScores[numUsers].totalScore = tempScores[numUsers].additionScore + tempScores[numUsers].subtractionScore + tempScores[numUsers].multiplicationScore + tempScores[numUsers].divisionScore;
            tempScores[numUsers].averageScore = tempScores[numUsers].totalScore / 4.0;
            numUsers++;
        }
    }
    fclose(fp);

    if (numUsers == 0) {
        printf("No users found.\n");
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
    printf("Top 10 Users (by Addition Score):\n");
    for (int i = 0; i < numUsers && i < 10; i++) {
        printf("%d. %s: %d\n", i + 1, tempScores[i].name, tempScores[i].additionScore);
    }
    system("pause");
}

void SubtractionLeaderboard(UserScore userScores[]) {
    int numUsers = 0;
    char line[100];
    UserScore tempScores[MAX_USERS];
    FILE *fp = fopen(USER_SCORES_FILE, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%s %d %d %d %d", tempScores[numUsers].name, &tempScores[numUsers].additionScore, &tempScores[numUsers].subtractionScore, &tempScores[numUsers].multiplicationScore, &tempScores[numUsers].divisionScore) == 5) {
            tempScores[numUsers].totalScore = tempScores[numUsers].additionScore + tempScores[numUsers].subtractionScore + tempScores[numUsers].multiplicationScore + tempScores[numUsers].divisionScore;
            tempScores[numUsers].averageScore = tempScores[numUsers].totalScore / 4.0;
            numUsers++;
        }
    }
    fclose(fp);

    if (numUsers == 0) {
        printf("No users found.\n");
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
    printf("Top 10 Users (by Subtraction Score):\n");
    for (int i = 0; i < numUsers && i < 10; i++) {
        printf("%d. %s: %d\n", i + 1, tempScores[i].name, tempScores[i].subtractionScore);
    }
    system("pause");
}

void MultiplicationLeaderboard(UserScore userScores[]) {
    int numUsers = 0;
    char line[100];
    UserScore tempScores[MAX_USERS];
    FILE *fp = fopen(USER_SCORES_FILE, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%s %d %d %d %d", tempScores[numUsers].name, &tempScores[numUsers].additionScore, &tempScores[numUsers].subtractionScore, &tempScores[numUsers].multiplicationScore, &tempScores[numUsers].divisionScore) == 5) {
            tempScores[numUsers].totalScore = tempScores[numUsers].additionScore + tempScores[numUsers].subtractionScore + tempScores[numUsers].multiplicationScore + tempScores[numUsers].divisionScore;
            tempScores[numUsers].averageScore = tempScores[numUsers].totalScore / 4.0;
            numUsers++;
        }
    }
    fclose(fp);

    if (numUsers == 0) {
        printf("No users found.\n");
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
    printf("Top 10 Users (by Multiplication Score):\n");
    for (int i = 0; i < numUsers && i < 10; i++) {
        printf("%d. %s: %d\n", i + 1, tempScores[i].name, tempScores[i].multiplicationScore);
    }
    system("pause");
}

void DivisionLeaderboard(UserScore userScores[]) {
    int numUsers = 0;
    char line[100];
    UserScore tempScores[MAX_USERS];
    FILE *fp = fopen(USER_SCORES_FILE, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%s %d %d %d %d", tempScores[numUsers].name, &tempScores[numUsers].additionScore, &tempScores[numUsers].subtractionScore, &tempScores[numUsers].multiplicationScore, &tempScores[numUsers].divisionScore) == 5) {
            tempScores[numUsers].totalScore = tempScores[numUsers].additionScore + tempScores[numUsers].subtractionScore + tempScores[numUsers].multiplicationScore + tempScores[numUsers].divisionScore;
            tempScores[numUsers].averageScore = tempScores[numUsers].totalScore / 4.0;
            numUsers++;
        }
    }
    fclose(fp);

    if (numUsers == 0) {
        printf("No users found.\n");
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
    printf("Top 10 Users (by Division Score):\n");
    for (int i = 0; i < numUsers && i < 10; i++) {
        printf("%d. %s: %d\n", i + 1, tempScores[i].name, tempScores[i].divisionScore);
    }
    system("pause");
}

void OverallLeaderboard(UserScore userScores[]) {
    printf("===== Overall Leaderboard =====\n");
    printf("Rank\tName\tAverage Score\n");
    for (int i = 0; i < 10 && i < MAX_USERS; i++) {
        printf("%d\t%s\t%.2f\n", i+1, userScores[i].name, userScores[i].averageScore);
    }
    printf("==============================\n");
    system("pause");
}

int main() {
    User users[MAX_USERS];
    UserScore userScores[MAX_USERS];
    int numUsers = 0;

    loadUsers(users, userScores, &numUsers);
    loadUserScores(userScores, numUsers);

    while (1) {
        int choice = -1;
        if (currentUser == -1) {
            system("cls");
            printf("\n**********************************************************");
            printf("\n*        Welcome to Arithmetic Game Project        *");
            printf("\n**********************************************************\n\n");
            printf("INSTRUCTIONS:                          \n");
            printf("Instruction Number 1: You can choose the log in and register to proceed the game.\n");
            printf("Instruction Number 2: Type 1. Login Menu if you have already created a username and password.\n");
            printf("Instruction Number 3: Type 2. Register Menu if you don't have username or password.\n");
            printf("Instruction Number 4: Don't put (letter words) into the Enter your choice Game Menu because these \nwill cause the program to crash.\n");
            printf("Instruction Number 5: Now Enjoy the Game Arithmetic Game! \n\n");
            printf("You can choose to Log in or register:\n");
            printf("1. Login\n");
            printf("2. Register\n");
            printf("3. Save & Quit\n");
            printf("=================================\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            printf("=================================\n");
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
                    saveUsers(users, numUsers);
                    printf("Progress saved. Thank you for playing the Arithmetic Game!\n");
                    printf("\tThis program will be exit in 10 seconds.\n");
				    delaykoto10();
                    return 0;
                default:
                    system("cls");
                    printf("Invalid choice.\n");
                    system("pause");
                    break;
            }
        } else {
            currentUser = GameMenu(userScores, currentUser);
        }
    }
}

void delaykoto5(){
    int i;
for (i = 0; i < 5; i++)
{
    delay(1);
    printf("%d.. ", i + 1);
        		}
}

void delaykoto3(){
    int i;
for (i = 0; i < 3; i++)
{
    delay(1);
    printf("%d.. ", i + 1);
        		}
}

void delaykoto10(){
    int i;
for (i = 0; i < 10; i++)
{
    delay(1);
    printf("%d.. ", i + 1);
        		}
}

int GameMenu(UserScore userScores[], int currentUser) {
    int averageScore = (userScores[currentUser].additionScore + userScores[currentUser].subtractionScore + userScores[currentUser].multiplicationScore + userScores[currentUser].divisionScore) / 4;
    int act2;

    system("cls");
    printf("Welcome, %s!\n", userScores[currentUser].name);
    printf("Your Current Average Score is: %d\n", averageScore);
    printf("Choose a Game:\n");
    printf("1. Addition       (Highest Score: %d)\n", userScores[currentUser].additionScore);
    printf("2. Subtraction    (Highest score: %d)\n", userScores[currentUser].subtractionScore);
    printf("3. Multiplication (Highest score: %d)\n", userScores[currentUser].multiplicationScore);
    printf("4. Division       (Highest score: %d)\n", userScores[currentUser].divisionScore);
    printf("5. Leaderboards\n");
    printf("6. Log Out\n");
    printf("Input Action(1-6): ");
    scanf("%d", &act2);
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
            saveUserScores(userScores, MAX_USERS);
            printf("This will be back to Log In Menu Game.\n");
            printf("\tThis program will be exit in 3 seconds.\n");
            delaykoto3();
            currentUser = -1;
            break;
        default:
            printf("Invalid choice.\n");
            system("pause");
            break;
    }
    return currentUser;
}

int LeaderboardMenu(UserScore userScores[], int currentUser) {
    int act3;
    system("cls");
    printf("Choose a Leaderboard:\n");
    printf("1. Addition Leaderboard\n");
    printf("2. Subtraction Leaderboard\n");
    printf("3. Multiplication Leaderboard\n");
    printf("4. Division Leaderboard\n");
    printf("5. Overall Leaderboard\n");
    printf("6. Go Back\n");
    printf("Input Action(1-6): ");
    scanf("%d", &act3);
    switch(act3) {
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
            printf("Progress saved. This will be back to Game Menu.\n");
            printf("This program will be exit in 3 seconds.\n");
            delaykoto3();
            break;
        default:
            printf("Invalid action.\n");
            break;
    }
    return act3;
}
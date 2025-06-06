#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_QUESTIONS 15
#define INFO_FILE "info.txt"

#define RED     "\033[0;31m"
#define RESET   "\033[0m"

typedef struct {
    char question[256];
    char options[4][128];
    char correct_option;
    int fifty_fifty_remove[2]; 
} Question;

Question questions[15] = {
    {
        "What is the capital of France?",
        {"A. Paris", "B. Madrid", "C. Rome", "D. Berlin"},
        'A',
        {1, 3}
    },
    {
        "How many continents are there?",
        {"A. 5", "B. 6", "C. 7", "D. 8"},
        'C',
        {0, 1}
    },
    {
        "Which gas do plants absorb?",
        {"A. Oxygen", "B. Nitrogen", "C. Carbon Dioxide", "D. Helium"},
        'C',
        {0, 3}
    },
    {
        "Which planet is known as the Red Planet?",
        {"A. Earth", "B. Mars", "C. Venus", "D. Jupiter"},
        'B',
        {0, 2}
    },
    {
        "How many sides does a triangle have?",
        {"A. 3", "B. 4", "C. 5", "D. 6"},
        'A',
        {1, 3}
    },
    {
        "Which animal is known as the king of the jungle?",
        {"A. Elephant", "B. Tiger", "C. Lion", "D. Giraffe"},
        'C',
        {0, 3}
    },
    {
        "What color are bananas when ripe?",
        {"A. Red", "B. Green", "C. Yellow", "D. Purple"},
        'C',
        {0, 3}
    },
    {
        "What is 2 + 2?",
        {"A. 3", "B. 4", "C. 5", "D. 22"},
        'B',
        {0, 3}
    },
    {
        "Which ocean is the largest?",
        {"A. Atlantic", "B. Pacific", "C. Indian", "D. Arctic"},
        'B',
        {0, 3}
    },
    {
        "Which instrument has black and white keys?",
        {"A. Guitar", "B. Flute", "C. Piano", "D. Violin"},
        'C',
        {0, 1}
    },
    {
        "Which of these brands makes processors?",
        {"A. Kent", "B. CocaCola", "C. AMD", "D. Aqua Carpatica"},
        'C',
        {0, 1}
    },
    {
        "Which beer is from Prague?",
        {"A. Heineken", "B. Pilsner Uruquell", "C. Timisoreana", "D. Chimay"},
        'B',
        {0, 2}
    },
    {
        "Which cigarettes are from London?",
        {"A. Gouloises", "B. Camel", "C. Marlboro", "D. Dunhill"},
        'D',
        {0, 1}
    },
    {
        "Whats the author name?",
        {"A. Costi", "B. Mikael Iacson", "C. Orlando", "D. Gobi"},
        'A',
        {1, 2}
    }
};

void saveGameResult(const char* name, float score) {
    FILE *fp = fopen(INFO_FILE, "a");
    if (fp == NULL) {
        printf("Could not open info.txt for writing.\n");
        return;
    }
    fprintf(fp, "%s %.1f\n", name, score);
    fclose(fp);
}

void viewLeaderboard() {
    FILE *fp = fopen(INFO_FILE, "r");
    if (fp == NULL) {
        printf("No game history found.\n");
        return;
    }

    typedef struct {
        char name[100];
        float score;
    } Player;

    Player players[1000];
    int count = 0;

    while (fscanf(fp, "%s %f", players[count].name, &players[count].score) == 2) {
        count++;
    }
    fclose(fp);

    // Sort top 5 by score descending
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (players[j].score > players[i].score) {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    printf("\n--- Top 5 Players ---\n");
    int top = count < 5 ? count : 5;
    for (int i = 0; i < top; i++) {
        printf("%s - " RED "%.1f" RESET " points\n", players[i].name, players[i].score);
    }

    printf("\n--- Full Leaderboard ---\n");
    for (int i = 0; i < count; i++) {
        printf("%s - " RED "%.1f" RESET " points\n", players[i].name, players[i].score);
    }
}

void viewUserHistory(const char* username) {
    FILE *fp = fopen(INFO_FILE, "r");
    if (fp == NULL) {
        printf("No game history found.\n");
        return;
    }

    char name[100];
    float score;
    int found = 0;
    printf("\n--- History for %s ---\n", username);
    while (fscanf(fp, "%s %f", name, &score) == 2) {
        if (strcmp(name, username) == 0) {
            printf("Score: " RED "%.1f" RESET " points\n", score);
            found = 1;
        }
    }
    if (!found) {
        printf("No games found for %s.\n", username);
    }
    fclose(fp);
}

void playGame() {
    float score = 0.0;
    int used_fifty_fifty = 0;
    char input[10];

    srand(time(NULL));

    for (int i = 0; i < MAX_QUESTIONS; i++) {
        printf("\nQuestion %d: %s\n", i + 1, questions[i].question);
        for (int j = 0; j < 4; j++) {
            printf("%c. %s\n", 'A' + j, questions[i].options[j]);
        }

        printf("Enter your answer (A-D) or type 50 to use 50/50: ");
        scanf("%s", input);

        char answer;
        int used_this_question_fifty = 0;

        if (strcmp(input, "50") == 0 && !used_fifty_fifty) {
            used_fifty_fifty = 1;
            used_this_question_fifty = 1;
            printf("Using 50/50...\n");

            int correct_index = questions[i].correct_option - 'A';

            int wrong_indices[3], w = 0;
            for (int j = 0; j < 4; j++) {
                if (j != correct_index)
                    wrong_indices[w++] = j;
            }

            int keep_wrong = wrong_indices[rand() % 3];

            printf("Remaining options:\n");
            printf("%c. %s\n", 'A' + correct_index, questions[i].options[correct_index]);
            printf("%c. %s\n", 'A' + keep_wrong, questions[i].options[keep_wrong]);

            printf("Enter your answer (A-D): ");
            scanf("%s", input);
        }

        answer = toupper(input[0]);

        if (answer == questions[i].correct_option) {
            if (used_this_question_fifty) {
                score += 0.5;
                printf("Correct! (+0.5)\n");
            } else {
                score += 1.0;
                printf("Correct! (+1)\n");
            }
        } else {
            printf("Incorrect. (0 points for this question)\n");
        }
    }

    printf("\nGame over! Your total score: " RED "%.1f" RESET "\n", score);
    printf("Enter your name to save your score: ");
    char name[100];
    scanf("%s", name);
    saveGameResult(name, score);
}

int main() {
    int choice;
    char username[100];

    while (1) {
        printf("\n--- Who Wants to Be a Millionaire ---\n");
        printf("1. Play Game\n");
        printf("2. View Leaderboard\n");
        printf("3. View User History\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                viewLeaderboard();
                break;
            case 3:
                printf("Enter username: ");
                scanf("%s", username);
                viewUserHistory(username);
                break;
            case 4:
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

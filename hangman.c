#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "mongoose.h"

#define MAX_WORD_LENGTH 20
#define MAX_TRIES 6

char secretWord[MAX_WORD_LENGTH];
char guessedWord[MAX_WORD_LENGTH];
int incorrectTries = 0;

void displayHangman() {
    switch (incorrectTries) {
        case 1:
            printf("  O\n");
            break;
        case 2:
            printf("  O\n");
            printf("  |\n");
            break;
        case 3:
            printf("  O\n");
            printf(" /|\n");
            break;
        case 4:
            printf("  O\n");
            printf(" /|\\\n");
            break;
        case 5:
            printf("  O\n");
            printf(" /|\\\n");
            printf(" /\n");
            break;
        case 6:
            printf("  O\n");
            printf(" /|\\\n");
            printf(" / \\\n");
            break;
        default:
            break;
    }
}

void initializeGame() {
    const char* words[] = {"hangman", "programming", "interface", "mongoose", "example"};
    srand(time(NULL));
    strcpy(secretWord, words[rand() % (sizeof(words) / sizeof(words[0]))]);

    for (int i = 0; i < strlen(secretWord); i++) {
        guessedWord[i] = '_';
    }
    guessedWord[strlen(secretWord)] = '\0';

    incorrectTries = 0;
}

int makeGuess(char letter) {
    int correctGuess = 0;

    for (int i = 0; i < strlen(secretWord); i++) {
        if (secretWord[i] == letter) {
            guessedWord[i] = letter;
            correctGuess = 1;
        }
    }

    if (!correctGuess) {
        incorrectTries++;
    }

    return correctGuess;
}

int isGameWon() {
    return strcmp(secretWord, guessedWord) == 0;
}

int isGameOver() {
    return incorrectTries >= MAX_TRIES || isGameWon();
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;

        if (mg_http_match_uri(hm, "/api/hi")) {
            mg_http_reply(c, 200, "", "{%.*s:%.*s}\n",
                          (int)hm->uri.len, hm->uri.ptr,
                          (int)hm->body.len, hm->body.ptr);
        } else {
            struct mg_http_serve_opts opts = {.root_dir = "."};
            mg_http_serve_dir(c, hm, &opts);
        }
    }
}

int main() {
    initializeGame();

    char guess;
    printf("Welcome to Hangman!\n");

    while (!isGameOver()) {
        displayHangman();
        printf("Current word: %s\n", guessedWord);
        printf("Incorrect tries remaining: %d\n", MAX_TRIES - incorrectTries);
        printf("Enter your guess: ");
        scanf(" %c", &guess);

        if (!isalpha(guess)) {
            printf("Please enter a valid alphabetical character.\n");
            continue;
        }

        guess = tolower(guess);

        if (makeGuess(guess)) {
            printf("Correct!\n");
        } else {
            printf("Incorrect guess.\n");
        }
    }

    displayHangman();

    if (isGameWon()) {
        printf("Congratulations! You guessed the word: %s\n", secretWord);
    } else {
        printf("Sorry, you ran out of tries. The word was: %s\n", secretWord);
    }

    // Mongoose setup and event loop
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_log_set("2");  // Set log level to debug
    mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);
    for (;;) mg_mgr_poll(&mgr, 1000);

    return 0;
}

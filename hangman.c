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
    // ... (unchanged)
}

void initializeGame() {
    // ... (unchanged)
}

int makeGuess(char letter) {
    // ... (unchanged)
}

int isGameWon() {
    // ... (unchanged)
}

int isGameOver() {
    // ... (unchanged)
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    // ... (unchanged)
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
    mg_log_set(MG_LL_DEBUG);
    mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);
    for (;;) mg_mgr_poll(&mgr, 1000);

    return 0;
}

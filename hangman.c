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
    // Initialize the secret word (you can expand the word list)
    const char* words[] = {"hangman", "programming", "interface", "mongoose", "example"};
    srand(time(NULL));
    strcpy(secretWord, words[rand() % (sizeof(words) / sizeof(words[0]))]);

    // Initialize guessed word with underscores
    for (int i = 0; i < strlen(secretWord); i++) {
        guessedWord[i] = '_';
    }
    guessedWord[strlen(secretWord)] = '\0';

    incorrectTries = 0;
}

int makeGuess(char letter) {
    int correctGuess = 0;

    // Check if the guessed letter is in the secret word
    for (int i = 0; i < strlen(secretWord); i++) {
        if (secretWord[i] == letter) {
            guessedWord[i] = letter;
            correctGuess = 1;
        }
    }

    // If incorrect, increment the incorrect tries
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

    return 0;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    // The MG_EV_HTTP_MSG event means HTTP request. `hm` holds parsed request,
    // see https://mongoose.ws/documentation/#struct-mg_http_message
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;

    // If the requested URI is "/api/hi", send a simple JSON response back
    if (mg_http_match_uri(hm, "/api/hi")) {
      // Use mg_http_reply() API function to generate JSON response. It adds a
      // Content-Length header automatically. In the response, we show
      // the requested URI and HTTP body:
      mg_http_reply(c, 200, "", "{%m:%m,%m:%m}\n",  // See mg_snprintf doc
                    MG_ESC("uri"), mg_print_esc, hm->uri.len, hm->uri.ptr,
                    MG_ESC("body"), mg_print_esc, hm->body.len, hm->body.ptr);
    } else {
      // For all other URIs, serve static content from the current directory
      struct mg_http_serve_opts opts = {.root_dir = "."};
      mg_http_serve_dir(c, hm, &opts);
    }
  }
}

int main() {
  // Rest of your initialisation code ...

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);        // Init manager
  mg_log_set(MG_LL_DEBUG);  // Set debug log level. Default is MG_LL_INFO
  mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);  // Setup listener
  for (;;) mg_mgr_poll(&mgr, 1000);                       // Infinite event loop

  return 0;
}

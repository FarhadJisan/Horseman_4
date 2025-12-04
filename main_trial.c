#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

// ANSI color and style codes
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_BRIGHT_RED "\033[91m"
#define COLOR_BRIGHT_GREEN "\033[92m"
#define COLOR_BRIGHT_YELLOW "\033[93m"
#define COLOR_BRIGHT_BLUE "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN "\033[96m"
#define COLOR_BRIGHT_WHITE "\033[97m"

#define STYLE_BOLD      "\033[1m"
#define STYLE_UNDERLINE "\033[4m"

// Question structure
typedef struct {
    int number;
    char question[256];
    char option_a[64];
    char option_b[64];
    char hint[128];
    char correct_answer; // 'A' or 'B'
    char correct_response[256];
    char incorrect_response[256];
} Question;

// Game state structure
typedef struct {
    int score;
    bool completed;
    int attempts;
} GameState;

// Questions data
Question questions[5] = {
    {
        1,
        "The forest remembers every soul that enters... but only some that leave. Tell me, mortal---what speaks only when spoken to?",
        "The Echo",
        "The Dead",
        "It borrows your voice but owns no tongue.",
        'A',
        "Ah, Clever Mortal... The forest whispers back to you in approval, or in mockery.",
        "Wrong. The dead remain silent, as do their secrets. You misheard the forest... and now it watches you more closely."
    },
    {
        2,
        "Light burns bright in the hands of fools. But what dies the moment you speak its name?",
        "A Promise",
        "Silence",
        "Mortals break it unknowingly every day.",
        'B',
        "The living can't stand the quiet. It's where they hear themselves decay.",
        "Fool... your words have disturbed the quiet. The forest hums with disapproval."
    },
    {
        3,
        "Listen closely, mortal. In my forest, two souls enter each hour. One departs every third hour. Tell me, after six hours... how many linger?",
        "10",
        "8",
        "The forest takes faster than it gives.",
        'A',
        "You count well... though the forest counts better. It never forgets a single lost soul.",
        "Incorrect. Even numbers bend to me. The souls you miscounted shiver in their graves."
    },
    {
        4,
        "Some graves have no names, yet everyone knows their story. Can you see what marks them? A traveler lies beneath my soil. No stone, no prayer. What marks his grave?",
        "Time",
        "Memory",
        "Names fade. The living forget. Only something invisible remains.",
        'B',
        "Correct. Names fade, stones crumble, but memories linger... even in my realm.",
        "Wrong. Time moves for the living, but it marks nothing for the dead. You've misjudged the forgotten."
    },
    {
        5,
        "Humor can be as sharp as my scythe... if you're clever enough to survive it. Why did the skeleton refuse to fight?",
        "He had no heart",
        "He had no guts",
        "He is literally spineless.",
        'B',
        "Ha... even in death, humor serves the wise. Few mortals can laugh in my forest.",
        "Incorrect. Hearts are fragile, but humor... even the dead must understand it."
    }
};

// Function prototypes
void clear_screen();
void typewriter_effect(const char* text, int delay_ms);
void print_colored(const char* text, const char* color, int style);
void display_intro();
void display_question(int q_num, const char* question, const char* option_a,
                     const char* option_b, const char* hint);
void display_correct_response(const char* response);
void display_incorrect_response(const char* response);
void display_outro(bool success, int score);
void display_title(const char* title);
void display_divider();
char get_valid_input();
int run_trial();
void start_trial_of_death();

// Display functions
void clear_screen() {
    printf("\033[2J\033[1;1H");
}

void typewriter_effect(const char* text, int delay_ms) {
    for (int i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
        fflush(stdout);
        usleep(delay_ms * 1000);

        // Pause for dramatic effect on punctuation
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            usleep(500000); // 500ms pause
        }
    }
    printf("\n");
}

void print_colored(const char* text, const char* color, int style) {
    if (style) {
        printf(STYLE_BOLD);
    }
    printf("%s%s%s", color, text, COLOR_RESET);
}

void display_title(const char* title) {
    printf("\n");
    display_divider();
    print_colored(title, COLOR_BRIGHT_MAGENTA, 1);
    printf("\n");
    display_divider();
    printf("\n");
}

void display_divider() {
    for (int i = 0; i < 60; i++) {
        print_colored("=", COLOR_BRIGHT_BLUE, 0);
    }
    printf("\n");
}

void display_intro() {
    clear_screen();

    display_title("HORSEMAN 4 - THE TRIAL OF DEATH");

    printf(STYLE_BOLD COLOR_BRIGHT_CYAN);
    typewriter_effect("You step into the Shadowy Forest, a realm where time seems to decay with every breath.", 30);
    typewriter_effect("Your torch flickers --- not from wind, but from fear itself.", 40);
    printf(COLOR_RESET);

    usleep(1000000);

    printf("\n" COLOR_BRIGHT_GREEN STYLE_BOLD);
    typewriter_effect("A pale figure emerges from the gloom --- Death, cloaked in blackened fog...", 35);
    typewriter_effect("His eyes glowing faintly with an otherworldly calm.", 40);
    typewriter_effect("His voice is cold and ancient.", 50);
    printf(COLOR_RESET);

    printf("\n" COLOR_YELLOW);
    typewriter_effect("\"Many seek power, but few understand the silence that follows it.", 40);
    typewriter_effect("To achieve your goal, you must prove mastery over mind and mortality.", 40);
    typewriter_effect("Solve these riddles, and perhaps you'll earn the Mark of Death.\"", 40);
    printf(COLOR_RESET);

    printf("\n" COLOR_BRIGHT_WHITE);
    typewriter_effect("You must answer his five mystical questions to claim the Badge of Shadows", 35);
    typewriter_effect("and move one step closer to becoming the Fifth Horseman.", 35);
    printf(COLOR_RESET);

    printf("\n" COLOR_BRIGHT_RED);
    typewriter_effect("The forest falls silent. A crow cries once, and your trial begins...", 50);
    printf(COLOR_RESET "\n\n");

    usleep(2000000); // 2 second pause
}

void display_question(int q_num, const char* question, const char* option_a,
                     const char* option_b, const char* hint) {
    printf("\n");
    display_divider();

    printf(COLOR_BRIGHT_YELLOW STYLE_BOLD);
    printf("\n[QUESTION %d]\n", q_num);
    printf(COLOR_RESET);

    printf(COLOR_CYAN STYLE_BOLD "\n\"");
    typewriter_effect(question, 40);
    printf("\"\n" COLOR_RESET);

    printf(COLOR_WHITE "\nOptions:\n" COLOR_RESET);
    printf(COLOR_GREEN "A) %s\n" COLOR_RESET, option_a);
    printf(COLOR_GREEN "B) %s\n" COLOR_RESET, option_b);

    printf(COLOR_BRIGHT_BLUE "\nHint: \"%s\"\n" COLOR_RESET, hint);

    printf(COLOR_YELLOW "\nYour answer (A/B): " COLOR_RESET);
}

void display_correct_response(const char* response) {
    printf("\n" COLOR_BRIGHT_GREEN STYLE_BOLD "CORRECT!\n" COLOR_RESET);
    printf(COLOR_GREEN "\"");
    typewriter_effect(response, 40);
    printf("\"\n" COLOR_RESET);
}

void display_incorrect_response(const char* response) {
    printf("\n" COLOR_BRIGHT_RED STYLE_BOLD "INCORRECT!\n" COLOR_RESET);
    printf(COLOR_RED "\"");
    typewriter_effect(response, 40);
    printf("\"\n" COLOR_RESET);
}

void display_outro(bool success, int score) {
    clear_screen();
    display_title("THE FOREST'S VERDICT");

    if (success) {
        printf(COLOR_BRIGHT_GREEN STYLE_BOLD);
        typewriter_effect("\"The forest falls silent... a rare occurrence.", 50);
        typewriter_effect("You have walked among shadows, faced my riddles, and counted souls as I do.", 45);
        typewriter_effect("Few mortals earn my mark... but you... you may yet join me in ways you cannot imagine.", 40);
        typewriter_effect("Take this Badge of Shadows. Wear it carefully, mortal ---", 45);
        typewriter_effect("the silence will remember you long after your voice fades.\"", 50);
        printf(COLOR_RESET);

        printf("\n\n" COLOR_BRIGHT_MAGENTA STYLE_BOLD);
        printf("|==========================================================|\n");
        printf("|                    BADGE OF SHADOWS                      |\n");
        printf("|                Score: %d/5 - VICTORY!                    |\n", score);
        printf("|==========================================================|\n");
        printf(COLOR_RESET);
    } else {
        printf(COLOR_BRIGHT_RED STYLE_BOLD);
        typewriter_effect("\"So, the forest decides you are not ready.", 60);
        typewriter_effect("Fear is a faithful companion, mortal...", 60);
        typewriter_effect("and it will follow you long after you leave this place.", 50);
        typewriter_effect("Remember: Death always waits... patiently.\"", 60);
        printf(COLOR_RESET);

        printf("\n\n" COLOR_RED STYLE_BOLD);
        printf("|==========================================================|\n");
        printf("|                    TRIAL FAILED                          |\n");
        printf("|                Score: %d/5 - DEFEAT                      |\n", score);
        printf("|==========================================================|\n");
        printf(COLOR_RESET);
    }

    printf("\n\n" COLOR_BRIGHT_CYAN "Press Enter to continue..." COLOR_RESET);
    getchar();
    getchar();
}

// Game logic functions
char get_valid_input() {
    char input;
    char buffer[10];

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (buffer[0] == 'A' || buffer[0] == 'a' ||
                buffer[0] == 'B' || buffer[0] == 'b') {
                input = toupper(buffer[0]);
                return input;
            }
        }

        printf(COLOR_RED "Invalid input! Please enter A or B: " COLOR_RESET);
    }
}

int run_trial() {
    GameState game = {0, false, 0};

    display_intro();

    for (int i = 0; i < 5; i++) {
        Question q = questions[i];

        display_question(q.number, q.question, q.option_a, q.option_b, q.hint);

        char answer = get_valid_input();

        if (answer == q.correct_answer) {
            game.score++;
            display_correct_response(q.correct_response);
        } else {
            display_incorrect_response(q.incorrect_response);
        }

        printf(COLOR_BRIGHT_BLUE "\nPress Enter to continue..." COLOR_RESET);
        getchar();
        getchar();

        if (i < 4) {
            clear_screen();
        }
    }

    game.completed = (game.score >= 3);

    display_outro(game.completed, game.score);

    return game.score;
}

void start_trial_of_death() {
    printf("\n" COLOR_BRIGHT_YELLOW STYLE_BOLD);
    printf("|========================================================|\n");
    printf("|         WELCOME TO THE TRIAL OF DEATH                  |\n");
    printf("|========================================================|\n");
    printf(COLOR_RESET);

    printf("\nIn this trial, you must answer 5 riddles from Death himself.\n");
    printf("You need at least 3 correct answers to pass the trial.\n");
    printf("Prepare yourself...\n\n");

    printf(COLOR_BRIGHT_CYAN "Press Enter to begin your trial..." COLOR_RESET);
    getchar();
    getchar();

    int score = run_trial();

    printf("\n" COLOR_BRIGHT_WHITE);
    printf("|========================================================|\n");
    printf("|        Trial Completed!                                |\n");
    printf("|        Final Score: %d/5                               |\n", score);
    printf("|========================================================|\n");
    printf(COLOR_RESET);
}

// Main function
int main() {
    start_trial_of_death();
    return 0;
}

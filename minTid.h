#ifndef MINTID_H
#define MINTID_H
// Constants
int MAX_ERROR_STRING_LEN = 100;
int MAX_CHAR_BAR_INPUT = 255;
int CHAR_WIDTH = 14;
int CHAR_HEIGHT = 28;
int INIT_PROFILE_CHAR_LEN = 20;
int PROFILE_OUTPUT_DIM = 30;
int MAX_LINE_LENGTH = 50;
int BAR_OUTPUT_X = 2;
int BAR_OUTPUT_Y = 2;
//char* CONFIG_LOCATION = "./config/config.txt";
char* CONFIG_LOCATION = "./config/config.txt";
char* ART_LOCATION = "./config/asciiart.txt";
int DEFAULT_MTCHOSENCHAR = -1;
time_t DEFAULT_LASTCHECKIN = 0;
// int TRUE = 1;
// int FALSE = 0;

char* show_bar(char* show_string, int expect_input);
void show_char_win();
void print_to_win(WINDOW* w, char* str);
void pick_character();
void handle_char(char c, WINDOW* bar_win);
void destroy_win(WINDOW *win, int clear_window);
WINDOW* show_win(int nlines, int ncols, int begin_y, int begin_x);
void check_allocation(void* allocation);
void exit_prog(int status, char* status_string, char* location_string);

typedef struct {
  int mtChosenChar;
  time_t lastCheckin;
}mtConfig;

typedef struct mtChar {
  char* piece;
  struct mtChar* next;
  int maxWidth;
  int maxHeight;
}mtChar;

#endif

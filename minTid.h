// Constants
int MAX_CHAR_BAR_INPUT = 255;
int MAX_LINE_LENGTH = 50;
int BAR_OUTPUT_X = 2;
int BAR_OUTPUT_Y = 2;
char* CONFIG_LOCATION = "./config/config.txt";
// int TRUE = 1;
// int FALSE = 0;

char* show_bar(char* show_string, int expect_input);
void show_char_win();
void print_to_win(WINDOW* w, char* str);
void pick_character();
void handle_char(char c, WINDOW* bar_win);
void destroy_win(WINDOW *win, int clear_window);
WINDOW* show_win(int nlines, int ncols, int begin_y, int begin_x);
void exit_prog();

typedef struct {
  int mtChosenChar;
}mtConfig;

typedef struct {
  struct mtChar* previous;
  char* piece;
  struct mtChar* next;
}mtChar;



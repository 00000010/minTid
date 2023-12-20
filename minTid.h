void show_char_win();
void print_to_win(WINDOW* w, char* str);
void pick_character();
void handle_char(char c);
void destroy_win(WINDOW *win, int clear_window);
WINDOW* show_win(int nlines, int ncols, int begin_y, int begin_x);
void exit_prog();

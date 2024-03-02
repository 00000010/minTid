// TODO: switch_to_snake_case
#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "minTid.h"
#include "windows.h"

WINDOW* create_main_win() {
  return show_win(LINES - 3, COLS, 0, 0);
}

WINDOW* create_bar_win() {
  return show_win(3, COLS, LINES - 3, 0);
}

WINDOW* create_bar_text_win() {
  return show_win(1, COLS - 2, LINES - 2, 1);
}

int profiles_per_line(int scr_width, int max_profile_width) {
  return scr_width / (max_profile_width + 2);
}

/// Print text in the bar
void print_bar_win(WINDOW* bar_win, char* string, int expect_input) {
  wclear(bar_win);
  mvwprintw(bar_win, 0, 0, string);
  wrefresh(bar_win);
  return; 
}

/// Calculate the amount of space above the profile in a square space of output_dim so it is centered vertically
/// Top affinity when odd
int calculate_profile_top_space(int height, int output_dim) {
  return output_dim / 4 - height / 2;
}

/// Calculate the amount of space to the left of the profile in a square space of output_dim so it is centered horizontally
/// Left affinity when odd
int calculate_profile_left_space(int width, int output_dim) {
  return output_dim / 2 - width / 2;
}

/// Counts the number of characters needed to represent a profile piece containing profile_char_count number of characters (including newlines, but not termininating NULL) centered in a square with output_dim as the dimension. Does not include the null character terminating a string
int calculate_profile_char_count(int profile_char_count, int width, int height, int output_dim) {
  int count = 0;
  count = calculate_profile_top_space(height, output_dim);              // Add space above profile
  count += (calculate_profile_left_space(width, output_dim)) * height; // Add space to left of profile
  count += profile_char_count;                                         // Add profile character count
  return count;
}

/// Concatenates src_string to dest_string if safe
/// If failed, does not change dest_string and src_string
/// dest_string should be stack-allocated
// TODO: check logic (sizeof)
int safe_strcat(char* dest_string, char* src_string) {
  int valid = 0;
  if (strlen(src_string) + 1 <= sizeof(dest_string) - strlen(dest_string)) {
    strncat(dest_string, src_string, sizeof(dest_string) - strlen(dest_string) - 1);
    valid = 1;
  }
  return valid;
}

/// Create string of profile with width and height, in a square space of output_dim dimension
char* create_piece_from_buffer(char* buffer, int width, int height, int output_dim) {
  int buffer_len = strlen(buffer);
  int piece_total_len = calculate_profile_char_count(buffer_len, width, height, output_dim) + 1; // strlen of piece + 1 for \0
  char* piece = malloc(piece_total_len);
  check_allocation(piece);
  int top_spacer_count = calculate_profile_top_space(height, output_dim);
  int left_spacer_count = output_dim / 2 - width / 2;
  int line_count = 0;

  // Create top space above profile
  for (int i = 0; i < top_spacer_count; i++) {
    piece[i] = '\n';
  }
  // Create rest of profile, with space to left of profile
  for (int i = 0; i < buffer_len; i++) {
    // Create left space for current line
    if ((i == 0) || (buffer[i - 1] == '\n')) {
      for (int j = 0; j < left_spacer_count; j++) {
        piece[top_spacer_count + left_spacer_count * line_count + i + j] = ' ';
      }
      line_count++;
    }
    // Create rest of profile for current line
    piece[top_spacer_count + left_spacer_count * line_count + i] = buffer[i];
  }
  piece[piece_total_len - 1] = '\0'; // Add null character at end
  return piece;
}

void check_allocation(void* allocation) {
  if (!allocation) {
    exit_prog(EXIT_FAILURE, "ERROR: unable to allocate memory", NULL);
  }
  return;
}

void initialize_mtChar(mtChar** mtC) {
  *mtC = malloc(sizeof(mtChar));
  check_allocation(*mtC);
  (*mtC)->piece = NULL;
  (*mtC)->maxWidth = 0;
  (*mtC)->maxHeight = 0;
  (*mtC)->next = NULL;
  return;
}

mtChar* get_mtChar_art() {
  FILE* f = fopen(ART_LOCATION, "rb");
  if (!f) {
    exit_prog(EXIT_FAILURE, "ERROR: unable to open ascii art file ", ART_LOCATION);
  }
  int buffer_len = INIT_PROFILE_CHAR_LEN;
  char* buffer = malloc(INIT_PROFILE_CHAR_LEN);
  check_allocation(buffer);
  mtChar* mtC;
  initialize_mtChar(&mtC);

  char c = getc(f);
  int i = 0;
  int line_length = 0;
  mtChar* mtC_p = mtC;
  int getting_profile = 1;
  while (c != EOF) {
    while (c == '\n') {
      c = getc(f);
    }
    getting_profile = 1;
    while (getting_profile) {
      // Resize buffer if needs more space
      if (i >= buffer_len - 1) {
        buffer = realloc(buffer, buffer_len * 2);
        buffer_len = buffer_len * 2;
        check_allocation(buffer);
      }
      buffer[i] = c;
      i++;
      line_length++;
      if (c == '\n') {
        mtC_p->maxHeight++;
        if (line_length > mtC_p->maxWidth) {
          mtC_p->maxWidth = line_length - 1;
        }
        line_length = 0;
        if ((c = getc(f)) == '\n') {
          buffer[i - 1] = '\0';
          mtC_p->piece = create_piece_from_buffer(buffer, mtC_p->maxWidth, mtC_p->maxHeight, PROFILE_OUTPUT_DIM);
          initialize_mtChar(&(mtC_p->next));
          mtC_p = mtC_p->next;
          i = 0;
          getting_profile = 0;
          continue;
        }
      } else {
        c = getc(f);
        if (c == EOF) {
          buffer[i - 1] = '\0';
          mtC_p->piece = create_piece_from_buffer(buffer, mtC_p->maxWidth, mtC_p->maxHeight, PROFILE_OUTPUT_DIM);
          initialize_mtChar(&(mtC_p->next));
          mtC_p = mtC_p->next;
          getting_profile = 0; // TODO: maybe not necessary
          break;
        }
      }
    }
  }
  fclose(f);
  free(buffer);
  return mtC;
}

void show_mtChars() {
  long length;
  mvprintw(1, 1, "Welcome to minTid! It looks like this is your first time here. Select your maître d'hôtel to get started.\n");

  mtChar* mtChars = get_mtChar_art();
  return;
}

void pick_character(mtConfig* config, WINDOW* win) {
  show_mtChars();
  char c = wgetch(win);
  return;
}

/// Handle character input from user
void handle_char(char c, WINDOW* bar_win) {
  switch (c) {
    case 'q':
    {
      char response;
      curs_set(1);         // Show cursor
      nodelay(bar_win, 0); // Make input blocking
      print_bar_win(bar_win, "Quit minTid (y/n)? ", 0);
      response = wgetch(bar_win);
      if (response == 'Y' || response == 'y') {
        destroy_win(bar_win, 0); // TODO: memory leak; destroy other windows too
        exit_prog(EXIT_SUCCESS, NULL, NULL);
      }
      break;
    }
    case ':':
    {
      char response;
      curs_set(1);
      print_bar_win(bar_win, ":", 0);
      response = wgetch(bar_win);
      response = wgetch(bar_win);
      break;
    }
    default:
      break;
  }
  curs_set(0);         // Hide cursor
  nodelay(bar_win, 1); // Make input non-blocking
}

/// Clear the window of text (if indicated) and free the memory associated with it.
void destroy_win(WINDOW *win, int clear_window) {
  if (clear_window) {
    werase(win);
    wrefresh(win);
  }
  delwin(win);
}

WINDOW* show_win(int nlines, int ncols, int begin_y, int begin_x) {
  WINDOW *win = newwin(nlines, ncols, begin_y, begin_x);
  return win;
}

void loadConfig(mtConfig* config) {
  // Load default values
  config->mtChosenChar = DEFAULT_MTCHOSENCHAR;
  config->lastCheckin = DEFAULT_LASTCHECKIN;

  // Read config
  FILE* f = fopen(CONFIG_LOCATION, "rb");
  if (!f) {
    exit_prog(EXIT_FAILURE, "ERROR: unable to open configuration file ", CONFIG_LOCATION);
  }
  char key[MAX_LINE_LENGTH];
  int value;
  fscanf(f, "%s %i\n", &key[0], &value); // TODO: unsafe; fix
  if (strcmp(key, "mtChosenChar") == 0) {
    config->mtChosenChar = value;
  } else if (strcmp(key, "lastCheckin")){
    config->lastCheckin = value;  
  } else {
//    wprintw(bar_win, "ERROR: unknown key \"%s\" in config", key);
//    char c = wgetch(bar_win);
//    ungetch(c);
  }
  fclose(f);
}

// TODO: implement variable arguments?
void exit_prog(int status, char* status_string, char* location_string) {
  delete_windows();
  endwin();
  if (status_string) {
    printf("%s", status_string);
    if (location_string) {
      printf("%s\n", location_string);
    } else {
      printf("\n");
    }
  }
  exit(status);
}

int main() {
  char c = ERR;
  setlocale(LC_ALL, ""); // set the locale for curses to native

  WINDOW* first_win = initscr(); // initialize the curses library
  cbreak();                      // character at a time
  noecho();                      // no printing to screen when user types in

  intrflush(stdscr, FALSE); // do not flush on interrupt key press (so ncurses knows what's on screen)
  keypad(stdscr, TRUE);     // treat function keys specially

  curs_set(0); // Make cursor invisible (for now)

  mtConfig* config = malloc(sizeof(mtConfig));
  check_allocation(config);
  loadConfig(config);

  if (config->mtChosenChar == -1) {
    pick_character(config, first_win);
    erase();
  }

  initialize_windows();

  WINDOW* bar_win = create_bar_win();
  WINDOW* bar_text_win = create_bar_text_win();
  WINDOW* main_win = create_main_win();
  nodelay(bar_win, 1);
  nodelay(main_win, 1);
  box(main_win, 0, 0);
  box(bar_win, 0, 0);

  while(1) {
    c = wgetch(main_win);
    if (c != ERR) {
      handle_char(c, bar_text_win);
    }
    wrefresh(main_win);
    wrefresh(bar_win);
    wrefresh(bar_text_win);
  }

  // Get input
//  c = getch();
//  refresh();

//  while (c) {
//    handle_char(c, bar_win);
//    c = getch();
//  }

  delwin(bar_win);
  delwin(bar_text_win);
  delwin(main_win);
  exit_prog(EXIT_SUCCESS, NULL, NULL);
  return 0;
}

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

/// Print text in the bar
void print_bar_win(WINDOW* bar_win, char* string, int expect_input) {
  wclear(bar_win);
  mvwprintw(bar_win, 0, 0, string);
  wrefresh(bar_win);
  return; 
}

char* get_mtChar_art() {
  FILE* f = fopen(ART_LOCATION, "rb");
  if (!f) {
    // TODO: check logic, cleanup this mess
    char error_string[MAX_ERROR_STRING_LEN];
    char* error_message = "ERROR: unable to open configuration file ";
    if (strlen(error_message) + 1 <= sizeof(error_string)) {
      strncat(error_string, error_message, MAX_ERROR_STRING_LEN);
      if (strlen(error_message) + strlen(ART_LOCATION) + 1 <= MAX_ERROR_STRING_LEN) {
        strncat(error_string, ART_LOCATION, MAX_ERROR_STRING_LEN - strlen(error_message - 1));
      }
      exit_prog(EXIT_FAILURE, error_string);
    } else {
      exit_prog(EXIT_FAILURE, error_message);
    }
  }
  char* buffer = 0;
  mtChar* mtC = malloc(sizeof(mtChar));
  if (!mtC) {
    exit_prog(EXIT_FAILURE, "ERROR: unable to allocate memory");
  }

  char c;
  if (f) {
    while ((c = getc(f)) != EOF) {
      
    }
  }
  fclose(f);
  return buffer;
}

void show_mtChars() {
  long length;
  mvprintw(1, 1, "Welcome to minTid! It looks like this is your first time here. Select your maître d'hôtel to get started.\n");

  char* art_buffer = get_mtChar_art();
//  if (f) {
//    int seek_err = fseek(f, 0, SEEK_END);
//    // TODO: Error validation
////    if (seek_err) {
////
////    }
//    length = ftell(f);
//    // TODO: Error validation
//    fseek(f, 0, SEEK_SET);
//    buffer = malloc(length + 1);
//    if (buffer) {
//      // TODO: Error validation
//      fread(buffer, 1, length, f);
//    }
//    fclose(f);
//  }
//  if (art_buffer) {
//    char c = art_buffer[0];
//     while (c) {
//       char* character_art = 
//     }
//     while (read > 1) {
//       wprintw(w, "First char: %.256s", &(buffer[]));
//       wrefresh(w);
//     }
//  }
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
        exit_prog(EXIT_SUCCESS, NULL);
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

void exit_prog(int status, char* status_string) {
  delete_windows();
  endwin();
  if (status_string) {
    printf("%s", status_string);
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
  exit_prog(EXIT_SUCCESS, NULL);
  return 0;
}

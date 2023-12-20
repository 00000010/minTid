#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "minTid.h"
#include "constants.h"

char* get_char_art() {
  FILE* f = fopen("./config/asciiart.txt", "rb");
  char* buffer = 0;

  if (f) {
    
  }
  fclose(f);
  return buffer;
}

void show_char_win() {
  long length;
  WINDOW* w = show_win(0,0,0,0);

  char* art_buffer = get_char_art();
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
  if (art_buffer) {
    char c = art_buffer[0];
    while (c) {
      char* character_art = 
    }
    while (read > 1) {
      wprintw(w, "First char: %.256s", &(buffer[]));
      wrefresh(w);
    }
  }
  return;
}

void pick_character() {
  show_char_win();
  return;
}

void handle_char(char c) {
  switch (c) {
    case 'q':
    {
      char response;
      WINDOW* prompt_win = show_win(1, COLS, LINES - 1, 0);
      wprintw(prompt_win, "Quit minTid? (Y/N)");
      response = wgetch(prompt_win);
      if (response == 'Y' || response == 'y') {
        exit_prog();
      }
      destroy_win(prompt_win, 1);
      break;
    }
    case ':':
    {
      char response;
      WINDOW* prompt_win = show_win(1, COLS, LINES - 1, 0);
      wprintw(prompt_win, ":");
      response = wgetch(prompt_win);
      destroy_win(prompt_win, 1);
      break;
    }
    default:
      break;
  }
  refresh();
}

void destroy_win(WINDOW *win, int clear_window) {
  if (clear_window) {
    werase(win);
    wrefresh(win);
  }
  delwin(win);
}

WINDOW* show_win(int nlines, int ncols, int begin_y, int begin_x) {
  WINDOW *win = newwin(nlines, ncols, begin_y, begin_x);
  wrefresh(win);
  return win;
}

void exit_prog() {
  endwin();
  exit(EXIT_SUCCESS);
}

int main() {
  char c;
  setlocale(LC_ALL, ""); // set the locale for curses to native

  initscr(); // initialize the curses library
  cbreak();  // character at a time
  noecho();  // no printing to screen when user types in

  intrflush(stdscr, FALSE); // do not flush on interrupt key press (so ncurses knows what's on screen)
  keypad(stdscr, TRUE);     // treat function keys specially

  // Draw main program
  printw("Hello world!");
  refresh();

  pick_character();

  // Read input
  c = getch();
  refresh();

  while (c) {
    handle_char(c);
    c = getch();
  }

  return 0;
}

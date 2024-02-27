#ifndef WINDOWS_H
#define WINDOWS_H

#include <curses.h>

WINDOW* current_task_win;
WINDOW* character_output_win;
WINDOW* character_profile_win;
WINDOW* basic_statistics_win;
WINDOW* tasks_win;
WINDOW* rewards_win;
WINDOW* habits_win;
WINDOW* goals_win;
WINDOW* distractions_win;
WINDOW* command_win;

void initialize_windows();
void delete_windows();

#endif

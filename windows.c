#include "windows.h"

void initialize_windows() {
  current_task_win = NULL;
  character_output_win = NULL;
  character_profile_win = NULL;
  basic_statistics_win = NULL;
  tasks_win = NULL;
  rewards_win = NULL;
  habits_win = NULL;
  goals_win = NULL;
  distractions_win = NULL;
  command_win = NULL;
  return;
}

void delete_windows() {
  if (current_task_win != NULL) {
    delwin(current_task_win);
  }
  if (character_output_win != NULL) {
    delwin(character_output_win);
  }
  if (character_profile_win != NULL) {
    delwin(character_profile_win);
  }
  if (basic_statistics_win != NULL) {
    delwin(basic_statistics_win);
  }
  if (tasks_win != NULL) {
    delwin(tasks_win);
  }
  if (rewards_win != NULL) {
    delwin(rewards_win);
  }
  if (habits_win != NULL) {
    delwin(habits_win);
  }
  if (goals_win != NULL) {
    delwin(goals_win);
  }
  if (distractions_win != NULL) {
    delwin(distractions_win);
  }
  if (command_win != NULL) {
    delwin(command_win);
  }
}

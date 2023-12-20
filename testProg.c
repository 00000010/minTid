#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
  char* myString = "h";
  printf("Length of myString: %lu\n", strlen(myString));
  printf("First char: %c\n", myString[0]);
  printf("Second char: %c\n", myString[1]);
  printf("Third char: %s\n", &(myString[2]));
  return 0;
}

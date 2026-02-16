#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  while (true) {
	  printf("$ ");

	  /* Get the user's command */
	  int cap = 1024;
	  char command[cap];
	  fgets(command, cap, stdin);

	  /* Trim the Newline */
	  command[strcspn(command, "\n")] = '\0';

	  /* Building the exit command */
	  if (strcmp(command, "exit") == 0) {
		  break;
	  }

	  /* Print the command result */
	  printf("%s: command not found\n", command);
  }
  return 0;
}

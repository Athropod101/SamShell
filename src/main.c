#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"

const int cap = 1024;

const char* builtins[] = {"exit", "echo", "type", NULL};

int main(int argc, char* argv[]) {
	struct ShellInput *input = malloc(sizeof(struct ShellInput) + cap);

	// Flush after every printf
	setbuf(stdout, NULL);

	do {
		printf("Sam 🌊 Shell 󱢴  ");
		//printf("$ ");

		/* Get the user's input */
		fgets(input->full, cap, stdin);
		Parse_input(input);

		/* Determine if command is builtin */
		input->isBuiltin = Check_Builtin(input->cmd);
		if (input->isBuiltin) {
			HandleBuiltin(input);
			continue;
		}

		/* Returning Null input Error */
		printf("%s: command not found\n", input->cmd);
	} while (strcmp(input->cmd, "exit") != 0);

	free(input);
	input = NULL;
	return 0;
}


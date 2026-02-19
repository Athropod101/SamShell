#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"

const int cap = 1024;

const char* builtins[] = {"exit", "echo", "type", NULL};

ShellInput shellstruct;
ShellInput* input = &shellstruct;

int main(int argc, char* argv[]) {

	// Flush after every printf
	setbuf(stdout, NULL);
	input->full = malloc(cap);
	input->args = calloc(cap / 2, sizeof(char*));
	input->cmd = NULL;
	
	do {
		printf("Sam 🌊 Shell 󱢴  ");
		//printf("$ ");

		// Get the user's input
		fgets(input->full, cap, stdin);
		if (strcmp(input->full, "\n") == 0) {continue;}
		Parse_input(input);

		// Determine if command is builtin
		input->isBuiltin = Check_Builtin(input->cmd);
		if (input->isBuiltin) {
			HandleBuiltin(input);
			continue;
		}

		// Returning Null input Error
		printf("%s: command not found\n", input->cmd);
	} while (input->cmd == NULL || strcmp(input->cmd, "exit") != 0);
	free(input->full); input->full = NULL;
	free(input->args); input->args= NULL;
	return 0;
}


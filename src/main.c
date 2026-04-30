#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "functions.h"

const int cap = 1024;

const char* builtins[] = {"exit", "echo", "type", "pwd", "cd", NULL};
char* PATH;

ShellInput shellstruct;
ShellInput* input = &shellstruct;

int main(int argc, char* argv[]) {
	// ShellInput setup
	input->full = malloc(cap);
	input->args = calloc(cap / 2, sizeof(char*));
	input->cmd = NULL;

	// Environment Setup
	PATH = getenv("PATH");
	

	// Flush after every printf
	setbuf(stdout, NULL);

	do {
		errno = 0;
		printf("Sam🌊Shell🐳%s 🐬 ", getenv("PWD"));


		// Get the user's input
		fgets(input->full, cap, stdin);
		if (strcmp(input->full, "\n") == 0) {continue;}
		Parse_input(input);

		// Determine if command is builtin
		input->isBuiltin = Check_Builtin(input->cmd);
		if (input->isBuiltin) {
			HandleBuiltin(input);
			Sanitize(input);
			continue;
		}

		RunProgram(input);
		Sanitize(input);

	} while (input->cmd == NULL || strcmp(input->cmd, "exit") != 0);
	free(input->full); input->full = NULL;
	free(input->args); input->args= NULL;
	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

const int cap = 1024;

const char* builtins[] = {"exit", "echo", "type", NULL};

//char* path = strtok(getenv("PATH"), ":");

void FetchCMD(struct ShellInput *input) {
	input->cmdEnd = strcspn(input->full, " ");
	if (input->cmdEnd == strlen(input->full)) {
		input->full[input->cmdEnd - 1] = '\0';
		input->hasPrompt = false;
	}
	else {
		input->full[input->cmdEnd] = '\0';
		input->hasPrompt = true;
	}
	input->cmd = &input->full[0];
}

void FetchPrompt(struct ShellInput *input) {
	input->prompt = &input->full[input->cmdEnd + 1];
	input->prompt[strcspn(input->prompt, "\n")] = '\0';
}

void Parse_input(struct ShellInput *input) {
	FetchCMD(input);
	if (input->hasPrompt) {
		FetchPrompt(input);
	}
}

int main(int argc, char* argv[]) {
	struct ShellInput *input = malloc(sizeof(struct ShellInput) + cap);
	char const* path = getenv("PATH");

	// Flush after every printf
	setbuf(stdout, NULL);

	do {
		printf("$ ");

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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct ShellInput {
	char* full;
	char* cmd;
	char** args;
	int argsz;
	bool isBuiltin;
} ShellInput;

extern const char* builtins[];

bool Check_Builtin(char* command) {
	for (int i = 0; builtins[i]; i++) {
		if (strcmp(command, builtins[i]) == 0) {
			return true;
		}
	}
	return false;
}

void Parse_input(ShellInput* input) {

	char* token;
	input->argsz = 0;
	input->full = strtok(input->full, "\n");
	token = strtok(input->full, " ");
	while (token) {
		input->args[input->argsz] = token;
		input->argsz ++;
		token = strtok(NULL, " ");
	}
	input->cmd = input->args[0];
	return;
}

void HandleBuiltin(struct ShellInput *input) {
	// echo builtin
	if (strcmp(input->cmd, "echo") == 0) {
		if (input->argsz == 1) {return;}
		for (int i = 1; i < input->argsz; i++) {
			printf("%s ", input->args[i]);
		}
		printf("\n");
		return;
	}

	/*
	// type builtin
	else if (strcmp(input->cmd, "type") == 0) {
		if (!input->hasPrompt) {
			printf("Type prompt not given. Please insert prompt to utilize the type builtin.\n");
			return;
		}

		bool isBuiltin = Check_Builtin(input->prompt);
		
		if (isBuiltin) {
			printf("%s is a shell builtin\n", input->prompt);
			return;
		}

		// Search for command in PATH
		char const* path = getenv("PATH");
		char* pathcopy = strdup(path);
		char* dir = strtok(pathcopy, ":");
		while (dir) {
			// The +1 is for the added / directory delimiter between dir and cmd.
			char* cmdpath = malloc(sizeof(*dir) + sizeof(input->prompt) + 1);
			strcpy(cmdpath, dir);
			strcat(cmdpath, "/");
			strcat(cmdpath, input->prompt);
			if (access(cmdpath, X_OK) == 0) {
				printf("%s is %s\n", input->prompt, cmdpath);
				free(pathcopy); pathcopy = NULL;
				free(cmdpath); cmdpath = NULL;
				return;
			}
			dir = strtok(NULL, ":");
			free(cmdpath); cmdpath = NULL;
		}
		free(pathcopy); pathcopy = NULL;

		printf("%s: command not found in PATH.\n", input->prompt);
	}
	*/
}

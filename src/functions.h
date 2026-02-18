#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct ShellInput {
	char* cmd;
	char* prompt;
	size_t cmdEnd;
	bool hasPrompt;
	bool isBuiltin;
	char full[];
} ShellInput;

extern const char* builtins[];

bool Check_Builtin(char* string) {
	for (int i = 0; builtins[i]; i++) {
		if (strcmp(string, builtins[i]) == 0) {
			return true;
		}
	}
	return false;
}

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


void HandleBuiltin(struct ShellInput *input) {

	/* echo builtin */
	if (strcmp(input->cmd, "echo") == 0) {
		if (input->hasPrompt) {
			printf("%s\n", input->prompt);
		}
	}

	/* type builtin */
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

		/* Search for command in PATH */
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
}

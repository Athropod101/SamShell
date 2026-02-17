#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ShellInput {
	char* cmd;
	char* prompt;
	size_t cmdEnd;
	bool hasPrompt;
	bool isBuiltin;
	char full[];
} VoidWarning; // Voids the warning that the struct is useless (it's not).

extern const char* builtins[];

void HandleBuiltin(struct ShellInput *input) {

	/* echo builtin */
	if (strcmp(input->cmd, "echo") == 0) {
		if (input->hasPrompt) {
			printf("%s\n", input->prompt);
		}
	}

	/* type builtin */
	else if (strcmp(input->cmd, "type") == 0) {
		bool isBuiltin = false;
		for (int i = 0; builtins[i]; i++) {
			if (strcmp(input->prompt, builtins[i]) == 0) {
				printf("%s is a shell builtin\n", input->prompt);
				isBuiltin = true;
				break;
			}
		}
		if (!isBuiltin) {
			printf("%s: not found\n", input->prompt);
		}
	}
}

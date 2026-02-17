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

bool Check_Builtin(char* string) {
	for (int i = 0; builtins[i]; i++) {
		if (strcmp(string, builtins[i]) == 0) {
			return true;
		}
	}
	return false;
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
		bool isBuiltin = Check_Builtin(input->prompt);
		
		if (isBuiltin) {
			printf("%s is a shell builtin\n", input->prompt);
		}
		else {
			printf("%s: not found\n", input->prompt);
		}
	}
}

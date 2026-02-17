#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cap = 1024;

struct ShellInput {
	char *cmd;
	char *prompt;
	bool hasPrompt;
	size_t cmdEnd;
	char full[];
};

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
}

void Parse_input(struct ShellInput *input) {
	FetchCMD(input);
	if (input->hasPrompt) {
		FetchPrompt(input);
	}
}

int main(int argc, char *argv[]) {

	// Flush after every printf
	setbuf(stdout, NULL);
	struct ShellInput *input = malloc(sizeof(struct ShellInput) + cap);

	while (true) {
		printf("$ ");

		/* Get the user's input */
		fgets(input->full, cap, stdin);
		Parse_input(input);

		/* Building the exit input */
		if (strcmp(input->cmd, "exit") == 0) {
			break;
		}
		/* Building the echo input */
		else if (strcmp(input->cmd, "echo") == 0) {
			if (input->hasPrompt) {
				printf("%s", input->prompt);
			}
		}
		/* Returning Null input Error */
		else {
		printf("%s: command not found\n", input->cmd);
		}
	}

	free(input);
	return 0;
}


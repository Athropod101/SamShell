#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins/cd.h"

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

char* PATHfind(char* program) {
	// IMPORTANT: Uses unfreed malloc !!!

	extern char* PATH;
	char path[strlen(PATH) + 1];
	strcpy(path, PATH);
	char* dir = strtok(path, ":");
	while (dir) {
		// The +1 is for the added / directory delimiter between dir and program.
		char program_path[strlen(dir) + strlen(program) + 1];
		strcpy(program_path, dir);
		strcat(program_path, "/");
		strcat(program_path, program);
		if (access(program_path, X_OK) == 0) {
			return strdup(program_path);
		}
		dir = strtok(NULL, ":");
	}
	return NULL;
}

void HandleBuiltin(ShellInput* input) {
	// echo builtin
	if (strcmp(input->cmd, "echo") == 0) {
		if (input->argsz == 1) {return;}
		for (int i = 1; i < input->argsz; i++) {
			printf("%s ", input->args[i]);
		}
		printf("\n");
		return;
	}

	// type builtin
	else if (strcmp(input->cmd, "type") == 0) {
		if (input->argsz == 1) {
			printf("Type argument not given. Please insert an argument to utilize the type builtin.\n");
			return;
		}
		else if (input->argsz > 2) {
			printf("More than 1 argument was given to the type builtin. Please only give 1 argument to type.\n");
			return;
		}

		bool isBuiltin = Check_Builtin(input->args[1]);
		if (isBuiltin) {
			printf("%s is a shell builtin\n", input->args[1]);
			return;
		}

		char* program_path = PATHfind(input->args[1]);
		if (program_path) {
			printf("%s is %s\n", input->args[1], program_path);
			free(program_path);
			return;
		}
			
		printf("%s: not found\n", input->args[1]);
	}

	// pwd builtin
	else if (strcmp(input->cmd, "pwd") == 0) {
		printf("%s\n", getenv("PWD"));
		return;
	}

	// cd builtin
	else if (strcmp(input->cmd, "cd") == 0) {
		cd(input->args[1]);
		return;
	}

}

void Sanitize(ShellInput* input) {
	for (int i = 0; i < input->argsz; i++) {
		input->args[i] = NULL;
	}
}

void RunProgram(ShellInput* input) {
	pid_t PID = fork();
	// if (PID < 0) {
	// 	perror("fork");
	// 	continue;
	// }
	// Don't need this error handling for now but will leave commented if needed later.
	if (PID == 0) {
		execvp(input->cmd, (input->args));
		if (errno == ENOENT) {
			printf("%s: command not found\n", input->cmd);
			exit(ENOENT);
		}
		exit(1);
	}
	else {
		wait(NULL);
	}
}

#include <unistd.h>
#include <errno.h>
#include <stdio.h>

extern char* PWD;

void cd(char* path) {
	chdir(path);
	if (errno == ENOENT)
		printf("cd: %s: No such file or directory\n", path);
	setenv("PWD", path, 1);
	return;	
}

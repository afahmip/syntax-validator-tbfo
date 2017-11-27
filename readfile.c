#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define ROW_SIZE 500
#define COL_SIZE 500
#define MAX_SIZE 500
#define STR_SIZE 500

char Input[ROW_SIZE][MAX_SIZE];
int size;

void readFileInput() {
	FILE *fileUser;
	fileUser = fopen("inputUser.txt", "r");
	size = 0;
	while (!feof(fileUser)) {
		printf("a\n");
		fgets(Input[size], MAX_SIZE, fileUser);
		strtok(Input[size], "\n");
		size++;
	}
	fclose(fileUser);
}

int main() {
	int i;
	readFileInput();
	for (i = 0; i < size; i++) {
		printf("%s\n", Input[i]);
	}

	return 0;
}
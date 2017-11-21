#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define ROW_SIZE 200
#define COL_SIZE 50
#define MAX_SIZE 500

/* Global variables */
int P, prodNum, NInput, LENGTH;
char Input[ROW_SIZE][MAX_SIZE];
char currInputArray[MAX_SIZE][MAX_SIZE];
char DP[ROW_SIZE][COL_SIZE][MAX_SIZE];
char Gram[ROW_SIZE][COL_SIZE][MAX_SIZE];
char stateNow[MAX_SIZE];


void break_grammar(int row, char a[20]){ //separate LHS and RHS and insert them to the matrix
	int i, idx = 0;
	char *token, LHS[20], prodSplit[MAX_SIZE][MAX_SIZE];

	/* Separate between LHS and RHS */
	/*
	S -> PROGRAM_START PROGRAM_BODY | PROGRAM_START BODY
	S,->,PROGRAM_START,PROGRAM_BODY,|,PROGRAM_START,BODY
	*/

	token = strtok(a, " ");
	while(token != NULL){
		if((strcmp(token, "->")!=0) && (strcmp(token, "|")!=0)){
			strcpy(Gram[row][idx], token);
			idx++;
		}
		token = strtok(NULL, " ");
	}
}

void break_input(char a[20], int *N){ //separate LHS and RHS and insert them to the matrix
	int i, idx = 0;
	char *token, LHS[20], prodSplit[MAX_SIZE][MAX_SIZE];
	bool isSemicolon=false, isPeriod=false;

	token = strtok(a, " ");
	while(token != NULL){
		if(strcmp(token, "0"))
		strcpy(currInputArray[idx], token);
		idx++;
		token = strtok(NULL, " ");
	}
	*N = idx;
}

char* combine(char A[20], char B[20]){
	int i,j;
	char tmp[20];

	for(i=0; i<strlen(A); i++){
		for(j=0; j<strlen(B); j++){
			strcpy(tmp, "");
			tmp[0] = A[i];
			tmp[1] = B[j];
		}
	}
}


int main(){
	int h, i, j, k, l, m, n;
	char start, prod[MAX_SIZE], strInput[MAX_SIZE], tmp[2], copy[10];

	printf("\nEnter start variable : ");
	scanf("%c",&start);
	printf("\nNumber of productions : ");
	scanf("%d ", &prodNum);

	/* Input CFG in CNF form */
	for(i=0; i<prodNum; i++){
		fgets(prod, MAX_SIZE, stdin);
		strtok(prod, "\n");
		break_grammar(i, prod);
	}

	/* Print token of productions */
	for(i=0; i<prodNum; i++){
		printf("<%d> ",i+1);
		for(j=1; j<3; j++){
			printf("%s ", Gram[i][j]);
		}
		printf("\n");
	}

	printf("\nNumber of input lines : ");
	scanf("%d ", &NInput);
	for(i=0; i<NInput; i++){
		fgets(Input[i], MAX_SIZE, stdin);
		strtok(Input[i], "\n");
	}

	/* Fill in the main diagonal of matrix */
	for(h=0; h<NInput; h++){

		/* Initialize */
		char currInput[100];
		int num_of_input;
		strcpy(currInput, Input[h]);
		LENGTH = strlen(Input[h]); //Length of current input line
		memset(currInputArray, 0, sizeof currInputArray); //Set array to empty
		break_input(Input[h], &num_of_input);

		for(i=0; i<num_of_input; i++){
			bool isFound = false;
			for(j=0; j<prodNum; j++){
				if(strcmp(currInputArray[i], Gram[j][1])==0){
					printf("<%d>",j+1);
					isFound = true;
					break;
				}
			}
			if(!isFound){
				int cntNum=0;
				bool isNum=true;
				for(j=0; j<strlen(currInputArray[i]); j++){
					if(isalpha(currInputArray[i][j])) isNum = false;
					if(isdigit(currInputArray[i][j])) cntNum++;
				}
				if(!isNum && cntNum == 0) strcpy(currInputArray[i], "word");
				else if(isNum) strcpy(currInputArray[i], "constant");
				for(j=0; j<prodNum; j++){
					if(strcmp(currInputArray[i], Gram[j][1])==0){
						printf("<%d>",j+1);
						isFound = true;
						break;
					}
				}
			}
		}
		printf("\n");
	}
}

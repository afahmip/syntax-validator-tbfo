#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define ROW_SIZE 200
#define COL_SIZE 50
#define MAX_SIZE 500

/* Global variables */
int prodNum, NInput, LENGTH;
char Input[ROW_SIZE][MAX_SIZE];
char currInputArray[MAX_SIZE][MAX_SIZE];
char Gram[ROW_SIZE][COL_SIZE][MAX_SIZE];
int tokenList[ROW_SIZE][COL_SIZE];


void break_grammar(int row, char a[20]){ //separate LHS and RHS and insert them to the matrix
	int i, idx = 0;
	char *token;

	token = strtok(a, " ");
	while(token != NULL){
		if((strcmp(token, "->")!=0) && (strcmp(token, "|")!=0)){
			strcpy(Gram[row][idx], token);
			idx++;
		}
		token = strtok(NULL, " ");
	}
}

int checkToken(char isi[MAX_SIZE]){ //Check if input available in token
	int i;

	for(i=0; i<prodNum; i++)
		if(strcmp(isi, Gram[i][1])==0) return i;

	return -1;
}

void break_input(char a[20], int row, int *N){ //Parsing input
	int i, j, k, idx, tokenIndex;
	char *token;

	token = strtok(a, " ");
	idx = 1;

	while(token != NULL){
		char temp[MAX_SIZE], currTemp[2], nextTemp[2];

		for(i=0; i<strlen(token); i++){
			sprintf(currTemp, "%c", token[i]);

			if(isdigit(token[i]) || isalpha(token[i])){ //while alphanumeric
				
				strcat(temp, currTemp);

				if(!isdigit(token[i+1]) && !isalpha(token[i+1])){

					/* Check whether it is number or not */
					bool isNum=true;
					for(j=0; j<strlen(temp); j++){
						if(!isdigit(temp[j])) isNum=false;
					}

					/* Parse again */
					if(!isNum){ //if alphanumeric
						tokenIndex = checkToken(temp);
						if(tokenIndex == -1){ //if word
							strcpy(currInputArray[idx], "word");
							tokenList[row][idx] = checkToken("word");
							idx++;
						}else{ //if not word
							strcpy(currInputArray[idx], temp);
							tokenList[row][idx] = tokenIndex;
							idx++;
						}
					}else{ //if number
						strcpy(currInputArray[idx], "constant");
						tokenList[row][idx] = checkToken("constant");
						idx++;
					}

					strcpy(temp, "");

				}

			}else{ //if not alphanumeric

				tokenIndex = checkToken(currTemp);
				if(tokenIndex != -1){ //if found
					strcpy(currInputArray[idx], currTemp);
					tokenList[row][idx] = tokenIndex;
					idx++;
				}else{ //if not found
					strcpy(currInputArray[idx], "NOT_FOUND");
					tokenList[row][idx] = -999;
					idx++;
				}
			}
		}

		token = strtok(NULL, " ");
	}

	tokenList[row][0] = idx;
	*N = idx-1;
	printf("\n");
}


int main(){
	int h, i, j, k, l, m, n;
	char start, prod[MAX_SIZE], strInput[MAX_SIZE], tmp[2], copy[10];
	FILE *fileInput;

	printf("Tubes TBFO II : 2nd Milestone - THE TOKENIZER\n");
	printf("             ---- Created by : ----          \n");
	printf("------  Ahmad Fahmi Pratama - 13516139 ------\n");
	printf("------     Joseph Salimin - 13516037   ------\n");
	printf("------   Adylan Roaffa Ilmy - 13516016 ------\n");
	printf("---------------------------------------------\n\n");

	printf("\nCFG (Terminals only) :\n\n");
	prodNum = 0;
	fileInput = fopen("input.txt", "r");
	while(!feof(fileInput)){
		fgets(prod, MAX_SIZE, fileInput);
		strtok(prod, "\n");
		printf("%s\n", prod);
		break_grammar(prodNum, prod);
		prodNum++;
	}

	/* Print token of productions */
	printf("\nToken :\n\n");
	for(i=0; i<prodNum; i++){
		printf("<%d> ",i+1);
		for(j=1; j<3; j++){
			printf("%s ", Gram[i][j]);
		}
		printf("\n");
	}

	printf("\nEnter start variable : ");
	scanf("%c",&start);

	printf("\nNumber of input lines : ");
	scanf("%d ", &NInput);
	for(i=0; i<NInput; i++){
		fgets(Input[i], MAX_SIZE, stdin);
		strtok(Input[i], "\n");
	}

	/* Tokenizer */
	for(h=0; h<NInput; h++){
		/* Initialize */
		char currInput[100];
		int num_of_input;

		strcpy(currInput, Input[h]);
		LENGTH = strlen(Input[h]); //Length of current input line
		memset(currInputArray, 0, sizeof currInputArray); //Set array to empty
		break_input(Input[h], h, &num_of_input);
		for(i=1; i<tokenList[h][0]; i++){
			printf("<%d>",tokenList[h][i]+1);
		}
		printf("\n");
	}
}

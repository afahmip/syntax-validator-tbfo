#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define ROW_SIZE 200
#define COL_SIZE 50
#define MAX_SIZE 500

typedef struct{
	int tknIdx;
	char term[100];
}TERMINAL, NONTERMINAL, GRAMMAR;

/* Global variables */
int prodNum, NInput, LENGTH, NTSize, TSize;
char Input[ROW_SIZE][MAX_SIZE];
char currInputArray[MAX_SIZE][MAX_SIZE];
GRAMMAR Gram[ROW_SIZE][COL_SIZE];
TERMINAL terminalList[ROW_SIZE];
NONTERMINAL nonterminalList[ROW_SIZE];
int tokenInput[ROW_SIZE][COL_SIZE];

int checkToken(char isi[MAX_SIZE]){ //Check if input available in token
	int i;

	/* Check in nonterminal list */
	for(i=0; i<NTSize; i++){
		if(strcmp(nonterminalList[i].term, isi) == 0) return nonterminalList[i].tknIdx;
	}

	/* Check in terminal list */
	for(i=0; i<TSize; i++){
		if(strcmp(terminalList[i].term, isi) == 0) return terminalList[i].tknIdx;
	}

	return -1;
}

void break_grammar(int row, char *a){ //separate LHS and RHS and insert them to the matrix
	int i, idx = 0, tokenIndex;
	char *token;

	token = strtok(a, " ");
	while(token != NULL){
		if((strcmp(token, "->")!=0) && (strcmp(token, "|")!=0)){
			Gram[row][idx].tknIdx = checkToken(token);
			strcpy(Gram[row][idx].term, token);
			idx++;
		}
		token = strtok(NULL, " ");
	}
}

/* JANGAN DIOTAK ATIK */
void break_input(char a[20], int row){ //Parsing input
	int i, j, k, idx, tokenIndex;
	char *token;

	token = strtok(a, " ");
	idx = 1;

	while(token != NULL){
		char temp[MAX_SIZE], currTemp[2], nextTemp[2];
		strcpy(temp, "");

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

					//printf("- %s ",temp);

					/* Parse again */
					if(!isNum){ //if alphanumeric
						tokenIndex = checkToken(temp);
						if(tokenIndex == -1){ //if word
							strcpy(currInputArray[idx], "word");
							tokenInput[row][idx] = checkToken("word");
							idx++;
						}else{ //if not word
							//printf("- %s ",temp);
							strcpy(currInputArray[idx], temp);
							tokenInput[row][idx] = tokenIndex;
							idx++;
						}
					}else{ //if number
						strcpy(currInputArray[idx], "constant");
						tokenInput[row][idx] = checkToken("constant");
						idx++;
					}

					strcpy(temp, "");

				}

			}else{ //if not alphanumeric

				tokenIndex = checkToken(currTemp);
				if(tokenIndex != -1){ //if found
					strcpy(currInputArray[idx], currTemp);
					tokenInput[row][idx] = tokenIndex;
					idx++;
				}else{ //if not found
					strcpy(currInputArray[idx], "NOT_FOUND");
					tokenInput[row][idx] = -999;
					idx++;
				}
			}
		}

		token = strtok(NULL, " ");
	}

	tokenInput[row][0] = idx;
	printf("\n");
}


int main(){
	int h, i, j, k, l, m, n;
	char start, prod[MAX_SIZE], strInput[MAX_SIZE], tmp[2], copy[10], TERM[MAX_SIZE];
	FILE *fileInput, *fileTerminal, *fileNonTerminal;

	printf("Tubes TBFO II : 2nd Milestone - THE TOKENIZER\n");
	printf("             ---- Created by : ----          \n");
	printf("------  Ahmad Fahmi Pratama - 13516139 ------\n");
	printf("------     Joseph Salimin - 13516037   ------\n");
	printf("------   Adylan Roaffa Ilmy - 13516016 ------\n");
	printf("---------------------------------------------\n\n");

	/* Tokenize the terminal */
	fileTerminal = fopen("terminal.txt", "r");
	i = 0;
	int currToken = 1;
	while(!feof(fileTerminal)){
		fgets(TERM, MAX_SIZE, fileTerminal);
		strtok(TERM, "\n");
		terminalList[i].tknIdx = currToken;
		strcpy(terminalList[i].term, TERM);
		currToken++;
		i++;
	}
	TSize = currToken - 1;

	/* Tokenize the nonterminal */
	fileNonTerminal = fopen("nonterminal.txt", "r");
	i = 0;
	while(!feof(fileNonTerminal)){
		fgets(TERM, MAX_SIZE, fileNonTerminal);
		strtok(TERM, "\n");
		nonterminalList[i].tknIdx = currToken;
		strcpy(nonterminalList[i].term, TERM);
		currToken++;
		i++;
	}
	NTSize = currToken - 1;

	printf("\nCFG :\n\n");
	prodNum = 0;
	fileInput = fopen("grammar.txt", "r");
	while(!feof(fileInput)){
		fgets(prod, MAX_SIZE, fileInput);
		strtok(prod, "\n");
		printf("%s\n", prod);
		break_grammar(prodNum, prod);
		prodNum++;
	}

	for(i=0; i<prodNum; i++){
		for(j=0; j<10; j++){
			if(Gram[i][j].tknIdx != 0)
				printf("<%d> ", Gram[i][j].tknIdx);
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

	/* Convert input strings into tokens */
	for(h=0; h<NInput; h++){
		memset(currInputArray, 0, sizeof currInputArray); //Set array to empty
		break_input(Input[h], h);
		for(i=1; i<tokenInput[h][0]; i++){
			printf("<%d>",tokenInput[h][i]);
		}
		printf("\n");
	}
}

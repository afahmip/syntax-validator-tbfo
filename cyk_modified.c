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
int NInput, LENGTH, NTSize, TSize, startToken;
char Input[ROW_SIZE][MAX_SIZE];
char CFG[ROW_SIZE][COL_SIZE][20];
char Gram[ROW_SIZE][COL_SIZE][MAX_SIZE];
TERMINAL terminalList[ROW_SIZE];
NONTERMINAL nonterminalList[ROW_SIZE];
int tokenInput[ROW_SIZE][COL_SIZE];
char DP[MAX_SIZE][MAX_SIZE][20];
char baris[200][MAX_SIZE];

int checkToken(char isi[MAX_SIZE]){ //Check if input available in token
	int i;

	/* Check in terminal list */
	for(i=0; i<TSize; i++){
		if(strcmp(terminalList[i].term, isi) == 0) return terminalList[i].tknIdx;
	}

	/* Check in nonterminal list */
	for(i=0; i<NTSize; i++){
		if(strcmp(nonterminalList[i].term, isi) == 0) return nonterminalList[i].tknIdx;
	}

	return -1;
}

void break_grammar(int row, char *a){ //separate LHS and RHS and insert them to the matrix
	int i, idx = 1, tokenIndex;
	char *token, temp[MAX_SIZE], curr[MAX_SIZE];

	token = strtok(a, " ");
	tokenIndex = checkToken(token);
	sprintf(curr, "%d", tokenIndex);
	strcpy(Gram[row][0], curr);
	token = strtok(NULL, " ");

	strcpy(temp, "");
	while(token != NULL){
		if(strcmp(token, "->") != 0){
			if(strcmp(token, "|") == 0){
				strcpy(Gram[row][idx], temp);
				strcpy(temp, "");
				idx++;
			}else{
				tokenIndex = checkToken(token);
				sprintf(curr, "%d ", tokenIndex);
				strcat(temp, curr);
			}
		}
		token = strtok(NULL, " ");
	}
	//printf("\n");
}

/* JANGAN DIOTAK ATIK */
void break_input(char a[20], int row){ //Parsing input
	int i, j, k, idx, tokenIndex;
	char *token;
	char currInputArray[MAX_SIZE][MAX_SIZE];

	token = strtok(a, " ");
	idx = 1;
	memset(currInputArray, 0, sizeof currInputArray); //Set array to empty

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

					/* Parse again */
					if(!isNum){ //if alphanumeric
						tokenIndex = checkToken(temp);
						if(tokenIndex == -1){ //if word
							strcpy(currInputArray[idx], "word");
							tokenInput[row][idx] = checkToken("word");
							idx++;
						}else{ //if not word
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
}

void combine(char A[20], char B[20], int *count){ //generate every combination of 2 strings
	int i, j, nA=0, nB=0;
	char tmp[MAX_SIZE], sA[MAX_SIZE], sB[MAX_SIZE];
	char tA[MAX_SIZE][20], tB[MAX_SIZE][20], *token;

	strcpy(sA, A);
	strcpy(sB, B);

	token = strtok(sA, " ");
	while(token != NULL){
		strcpy(tA[nA], token);
		nA++;
		token = strtok(NULL, " ");
	}

	token = strtok(sB, " ");
	while(token != NULL){
		strcpy(tB[nB], token);
		nB++;
		token = strtok(NULL, " ");
	}

	if(nA>0 && nB>0){
		for(i=0; i<nA; i++){
			for(j=0; j<nB; j++){
				strcpy(tmp, "");
				sprintf(tmp, "%s %s ", tA[i], tB[j]);
				strcpy(baris[*count], tmp);
				(*count)++;
			}
		}
	}else if(nB == 0){
		for(i=0; i<nA; i++){
			strcpy(tmp, "");
			sprintf(tmp, "%s ", tA[i]);
			strcpy(baris[*count], tmp);
			(*count)++;
		}
	}else if(nA == 0){
		for(i=0; i<nB; i++){
			strcpy(tmp, "");
			sprintf(tmp, "%s ", tB[i]);
			strcpy(baris[*count], tmp);
			(*count)++;
		}
	}
}

void printCYK(int len){
	int i,j,k;

	for(i=0; i<len; i++){
		k = 0;
		for(j=len-i-1; j<len; j++){
			printf("{%s}\t", DP[k++][j]);
		}
		printf("\n");
	}
}

int main(){
	int h, i, j, k, l, m, n;
	char start[2], prod[MAX_SIZE], TERM[MAX_SIZE];
	FILE *fileInput, *fileTerminal, *fileNonTerminal;

	printf("Tubes TBFO II : SYNTAX VALIDATOR YOOOOOOOOOOO\n");
	printf("             ---- Created by : ----          \n");
	printf("------  Ahmad Fahmi Pratama - 13516139 ------\n");
	printf("------     Joseph Salimin - 13516037   ------\n");
	printf("------   Adylan Roaffa Ilmy - 13516016 ------\n");
	printf("---------------------------------------------\n\n");

	/* Tokenize the nonterminal */
	fileNonTerminal = fopen("nonterminal.txt", "r");
	i = 0;
	int currToken = 1;
	while(!feof(fileNonTerminal)){
		fgets(TERM, MAX_SIZE, fileNonTerminal);
		strtok(TERM, "\n");
		nonterminalList[i].tknIdx = currToken;
		strcpy(nonterminalList[i].term, TERM);
		currToken++;
		i++;
	}
	NTSize = i;

	/* Tokenize the terminal */
	fileTerminal = fopen("terminal.txt", "r");
	i = 0;
	char *token;

	while(!feof(fileTerminal)){
		fgets(TERM, MAX_SIZE, fileTerminal);
		strtok(TERM, "\n");
		token = strtok(TERM, " ");
		while(token != NULL){
			terminalList[i].tknIdx = currToken;
			strcpy(terminalList[i].term, token);
			i++;
			token = strtok(NULL, " ");
		}
		currToken++;
	}
	TSize = currToken - 1;

	i = 0;
	fileInput = fopen("grammar.txt", "r");
	while(!feof(fileInput)){
		fgets(prod, MAX_SIZE, fileInput);
		strtok(prod, "\n");
		strcat(prod, " |"); //for MARK
		break_grammar(i, prod);
		i++;
	}

	printf("\nEnter start variable : ");
	scanf("%s",&start);
	startToken = checkToken(start);

	printf("\nNumber of input lines : ");
	scanf("%d ", &NInput);
	for(i=0; i<NInput; i++){
		fgets(Input[i], MAX_SIZE, stdin);
		strtok(Input[i], "\n");
	}

	/* Convert input strings into tokens */
	for(i=0; i<NInput; i++){
		break_input(Input[i], i);
	}

	char temp[MAX_SIZE], copy[MAX_SIZE];
	/* Let's do the CYK */
	LENGTH = 0;
	for(i=0; i<NInput; i++){
		for(j=1; j<tokenInput[i][0]; j++){
			strcpy(copy, "");
			for(k=0; k<NTSize; k++){
				sprintf(temp, "%d ", tokenInput[i][j]);
				for(l=1; l<15; l++){
					if(strcmp(temp, Gram[k][l]) == 0){
						strcat(copy, Gram[k][0]);
						strcat(copy, " ");
					}
				}
			}
			strcpy(DP[LENGTH][LENGTH], copy);
			LENGTH++;
		}
	}


	for(i=1; i<LENGTH; i++){

		/* Initialize */
		char A[20], B[20], ans[MAX_SIZE];
		int cnt=0;
		strcpy(temp, "");
		memset(baris, 0, sizeof baris);

		for(j=i; j<LENGTH; j++){
			cnt = 0;
			for(k=j-i; k<j; k++){

				/* Compare pre-computed strings */
				strcpy(ans, "");
				strcpy(A, DP[j-i][k]);
				strcpy(B, DP[k+1][j]);
				combine(A, B, &cnt);
			}

			/* Compare with grammar table */
			strcpy(ans, "");
			for(k=0; k<NTSize; k++){
				for(l=1; l<10; l++){
					for(m=0; m<cnt; m++){
						if(strcmp(baris[m], Gram[k][l]) == 0){

							/* Cek duplicate character */
							bool isExist = false;
							char exist[3];
							for(n=0; n<strlen(ans); n++){
								strcpy(exist, "");
								exist[0] = ans[n];
								if(strcmp(exist, Gram[k][0])==0)isExist = true;
							}
							if(!isExist){
								strcat(ans, Gram[k][0]);
								strcat(ans, " ");
							}
						}
					}
				}
			}

			strcpy(DP[j-i][j], ans);

		}
	}

	//printCYK(LENGTH);
	bool isAccepted = false;
	for(i=0; i<strlen(DP[0][LENGTH-1]); i++)
		if(DP[0][LENGTH-1][i] == '1') isAccepted = true;

	if(isAccepted) printf("Verdict : Compile success!\n");
	else printf("Verdict : Compile error!\n");
}

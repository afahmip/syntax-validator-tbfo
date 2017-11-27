#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* They need big size ofc */
#define ROW_SIZE 500
#define COL_SIZE 500
#define MAX_SIZE 500
#define STR_SIZE 500

typedef struct{
	int tknIdx;
	char term[500];
}TERMINAL, NONTERMINAL;

/* Global variables */
int NInput, LENGTH, NTSize, TSize, startToken, size;
char Input[ROW_SIZE][MAX_SIZE];				//for input
char Gram[ROW_SIZE][COL_SIZE][MAX_SIZE];	//saving tokenized grammar
TERMINAL terminalList[ROW_SIZE];			//for terminals (content + token)
NONTERMINAL nonterminalList[ROW_SIZE];		//for nonterminals (content + token)
int tokenInput[ROW_SIZE][COL_SIZE];			//saving tokenized input lines
char DP[MAX_SIZE][MAX_SIZE][STR_SIZE];		//CYK table
char baris[500][MAX_SIZE];					//* saving temporary precomputed pairs of rules */

 /* Check if input available in token */

/* Check the Token */
int checkToken(char isi[MAX_SIZE]) {
	int i;

	/* Check in terminal list */
	for (i = 0; i<TSize; i++) {
		if (strcmp(terminalList[i].term, isi) == 0) return terminalList[i].tknIdx;
	}

	/* Check in nonterminal list */
	for(i = 0; i < NTSize; i++) {
		if (strcmp(nonterminalList[i].term, isi) == 0) return nonterminalList[i].tknIdx;
	}

	return -1;
}

char* check_content(int tokenIndex) {
	int i;

	/* Check in terminal list */
	for (i = 0; i < TSize; i++) {
		if (terminalList[i].tknIdx == tokenIndex) return terminalList[i].term;
	}

	/* Check in nonterminal list */
	for (i = 0; i < NTSize; i++) {
		if (nonterminalList[i].tknIdx == tokenIndex) return nonterminalList[i].term;
	}

	return "NOT_FOUND";
}

/* Separate LHS and RHS and insert them to the matrix */
void break_grammar(int row, char *a) { 
	/* Local variables */
	int i, idx = 1, tokenIndex;
	char *token, temp[MAX_SIZE], curr[MAX_SIZE];

	/* Call the function to split the grammar */
	token = strtok(a, " ");
	/* Change the grammar to token */
	tokenIndex = checkToken(token);
	sprintf(curr, "%d", tokenIndex);
	strcpy(Gram[row][0], curr);
	token = strtok(NULL, " ");

	strcpy(temp, "");
	/* Break the grammer dude */
	while (token != NULL) {
		if(strcmp(token, "->") != 0) {
			if(strcmp(token, "|") == 0) {
				strcpy(Gram[row][idx], temp);
				strcpy(temp, "");
				idx++;
			} else {
				tokenIndex = checkToken(token);
				sprintf(curr, "%d ", tokenIndex);
				strcat(temp, curr);
			}
		}
		token = strtok(NULL, " ");
	}
}

/* Parsing the input per line from the user */
void break_input (char a[STR_SIZE], int row) { 
	int i, j, k, idx, tokenIndex;
	char *token;
	char currInputArray[MAX_SIZE][MAX_SIZE];

	token = strtok(a, " ");
	idx = 1;
	memset(currInputArray, 0, sizeof currInputArray); //Set array to empty

	while (token != NULL) {
		char temp[MAX_SIZE], currTemp[2], nextTemp[2];
		strcpy(temp, "");

		for(i=0; i<strlen(token); i++){
			sprintf(currTemp, "%c", token[i]);

			if(isdigit(token[i]) || isalpha(token[i])){ /* While it is alphanumeric */
				strcat(temp, currTemp);

				if (!isdigit(token[i+1]) && !isalpha(token[i+1])) {
					/* Check whether it is number or not */
					bool isNum = true;
					/* Iteration */
					for(j = 0; j < strlen(temp); j++){
						if (!isdigit(temp[j])) isNum=false;
					}

					/* Parse again */
					if (!isNum) { /* If it is alphanumeric */
						tokenIndex = checkToken(temp);
						if(tokenIndex == -1){ //If word
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
			/* If the input is alphanumeric */
			} else { 
				tokenIndex = checkToken(currTemp);
				if(tokenIndex != -1){ /* If it is found */
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

/* Generate every combination of 2 strings */
void combine(char A[STR_SIZE], char B[STR_SIZE], int *count){ 
	int i, j, nA=0, nB=0;
	char tmp[MAX_SIZE], sA[MAX_SIZE], sB[MAX_SIZE];
	char tA[MAX_SIZE][STR_SIZE], tB[MAX_SIZE][STR_SIZE], *token;

	strcpy(sA, A);
	strcpy(sB, B);

	token = strtok(sA, " ");
	while (token != NULL) {
		strcpy(tA[nA], token);
		nA++;
		token = strtok(NULL, " ");
	}

	token = strtok(sB, " ");
	while (token != NULL) {
		strcpy(tB[nB], token);
		nB++;
		token = strtok(NULL, " ");
	}

	if (nA > 0 && nB > 0) {
		for (i = 0; i < nA; i++) {
			for (j=0; j < nB; j++) {
				strcpy(tmp, "");
				sprintf(tmp, "%s %s ", tA[i], tB[j]);
				strcpy(baris[*count], tmp);
				(*count)++;
			}
		}
	} else if (nB == 0) {
		for(i = 0; i < nA; i++) {
			strcpy(tmp, "");
			sprintf(tmp, "%s ", tA[i]);
			strcpy(baris[*count], tmp);
			(*count)++;
		}
	} else if (nA == 0) {
		for (i = 0; i < nB; i++) {
			strcpy(tmp, "");
			sprintf(tmp, "%s ", tB[i]);
			strcpy(baris[*count], tmp);
			(*count)++;
		}
	}
}

/* For testing the token input output */
void print_tokeninput(){
	int i, j;

	for(i=0; i<NInput; i++){
		for(j=1; j<tokenInput[i][0]; j++){
			printf("%d ", tokenInput[i][j]);
		}
		printf("\n\n");
	}
}

/* For testing the CYK Table */
void print_cyk(int len){
	int i,j,k;

	for(i=0; i<len; i++){
		if(i == 1) printf("|-|\n");
		k = 0;
		for(j=len-i-1; j<len; j++){
			printf("| %s", DP[k][j]);
			k++;
		}
		printf("|\n");
	}
}

/* For thesting the grammar output */
void print_grammar(){
	int i, j, x;

	for(i=0; i<NTSize; i++){
		for(j=0; j<25; j++){
			x = checkToken(Gram[i][j]);
			printf("%s ", Gram[i][j]);
		}
		printf("\n");
	}
}

void check_error(){
	/*
	if line = 0 then 
	   if program then kol++ else break
	if line=1 then 
	   if var or begin then kol ++
	else break
	*/
	int i, j, k;
}

void stdin_flush(){
	char something[2];
	fgets(something, 2, stdin);
}

void readFileInputUser() {
	FILE *fileUser;
	fileUser = fopen("inputUser.txt", "r");
	NInput = 0;
	while (!feof(fileUser)) {
		fgets(Input[NInput], MAX_SIZE, fileUser);
		if (Input[NInput][0] != '\n') {
			strtok(Input[NInput], "\n");
			NInput++;
		}
	}
	fclose(fileUser);
}

void readFileRule() {
	int i;
	int currToken = 1;
	FILE *fileNonTerminal, *fileTerminal, *fileInput;
	char TERM[MAX_SIZE];
	char prod[MAX_SIZE];
	char* token;

	i = 0; 
	/* Tokenize the nonterminal */
	fileNonTerminal = fopen("nonterminal.txt", "r");
	
	while(!feof(fileNonTerminal)){
		fgets(TERM, MAX_SIZE, fileNonTerminal);
		strtok(TERM, "\n");
		nonterminalList[i].tknIdx = currToken;
		strcpy(nonterminalList[i].term, TERM);
		//printf("%s\n", nonterminalList[i].term);
		currToken++;
		i++;
	}
	NTSize = i;
	
	/* Tokenize the terminal */
	i = 0;

	fileTerminal = fopen("terminal.txt", "r");

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
	TSize = currToken - 1 - NTSize;

	i = 0;
	fileInput = fopen("grammar.txt", "r");

	while(!feof(fileInput)){
		fgets(prod, MAX_SIZE, fileInput);
		strtok(prod, "\n");
		strcat(prod, " |"); //for MARK
		break_grammar(i, prod);
		i++;
	}

	fclose(fileNonTerminal);
	fclose(fileTerminal);
	fclose(fileInput);
}

void readFromTerminal() {
	int i;
	printf("Number of input lines : ");
	scanf("%d", &NInput);
	stdin_flush();
	printf("\nInput your codes here :\n");
	for(i = 0; i < NInput; i++){
		fgets(Input[i], MAX_SIZE, stdin);
		strtok(Input[i], "\n");
	}
}

/* The main program */
int main(){
	int h, i, j, k, l, m, n;
	char start[2];
	FILE *fileOutput, *fileToken;

	printf("Tubes TBFO II : SYNTAX VALIDATOR YOOOOOOOOOOO\n");
	printf("            ---- Created by : ----           \n");
	printf("------  Ahmad Fahmi Pratama - 13516139 ------\n");
	printf("------     Joseph Salimin - 13516037   ------\n");
	printf("------   Adylan Roaffa Ilmy - 13516016 ------\n");
	printf("---------------------------------------------\n\n");

	/* Read the grammar */
	readFileRule();

	/* Input code */
	printf("Select input method (Terminal 1, File 2) : ");
	scanf("%d", &i);

	if (i == 1) {
		readFromTerminal();
	} else if (i == 2) {
		readFileInputUser();
	} else {
		goto Exit;
	}
	
	/* Convert input strings into tokens */
	for(i = 0; i < NInput; i++){
		break_input(Input[i], i);
	}

	char temp[MAX_SIZE], copy[MAX_SIZE];
	/* Let's do the CYK */
	LENGTH = 0;
	for (i = 0; i < NInput; i++){
		for(j = 1; j < tokenInput[i][0]; j++){
			strcpy(copy, "");
			for(k = 0; k < NTSize; k++){
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

	for (i = 1; i < LENGTH; i++){

		/* Initialize */
		char A[STR_SIZE], B[STR_SIZE], ans[MAX_SIZE];
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
							char exist[100], ansN[2];
							strcpy(exist, "");
							for(n=0; n<strlen(ans); n++){
								if(ans[n] != ' '){
									sprintf(ansN, "%c", ans[n]);
									strcat(exist, ansN);
								}else{
									if(strcmp(exist, Gram[k][0])==0)isExist = true;
									strcpy(exist, "");
								}
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

	fileToken = fopen("tokenlist.md", "w");
	fprintf(fileToken, "| Token | Non-Terminal | | Token | Terminal |\n");
	fprintf(fileToken, "|-|-|-|-|-|\n");
	for(i=0; i<NTSize; i++){
		fprintf(fileToken, "| %d | %s |",	nonterminalList[i].tknIdx, nonterminalList[i].term);
		if(terminalList[i].tknIdx != 0)
			fprintf(fileToken, "| %d | %s |\n", terminalList[i].tknIdx, terminalList[i].term);
		else fprintf(fileToken, "| | |\n");
	}
	fclose(fileToken);


	fileOutput = fopen("cyktable.md", "w");
	for(i=0; i<LENGTH; i++){
		k = 0;
		if(i == 1) fprintf(fileOutput, "|-|\n"); //for .md table format
		for(j=LENGTH-i-1; j<LENGTH; j++){
			fprintf(fileOutput, "| %s", DP[k][j]);
			k++;
		}
		fprintf(fileOutput, "|\n");
	}
	fclose(fileOutput);

	//print_tokeninput();

	bool isAccepted = false;
	for(i=0; i<strlen(DP[0][LENGTH-1]); i++)
		if(DP[0][LENGTH-1][i] == '1') isAccepted = true;

	if(isAccepted) printf("\nVerdict : Compile success!\n");
	else printf("\nVerdict : Compile error!\n");
	Exit:
	return 0;
}

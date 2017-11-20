#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 100

/* Global variables */
int P, NP, LENGTH;
char DP[MAX_SIZE][MAX_SIZE][20], Gram[MAX_SIZE][MAX_SIZE][20];
char baris[200][MAX_SIZE];

void concat(char *a[], char b[20]){
	int i;
	bool isExist = false;
	char tmp[3];

	for(i=0; i<strlen(*a); i++){
		strcpy(tmp, "");
		tmp[0] = *a[i];
		if(strcmp(b, tmp)==0)isExist = true;
	}
	if(!isExist) strcat(*a, b);
}

void break_grammar(int row, char a[20]){ //separate LHS and RHS and insert them to the matrix
	int i, idx = 0;
	char *token, LHS[20];

	/* Separate between LHS and RHS */
	token = strtok(a, "->");
	while(token != NULL){
		strcpy(Gram[row][idx], token);
		strcpy(LHS, token);
		idx++;
		token = strtok(NULL, "->");
	}
	idx--;

	/* Let's handle the RHS */
	token = strtok(LHS, "|");
	while(token != NULL){
		strcpy(Gram[row][idx], token);
		idx++;
		token = strtok(NULL, "|");
	}
}

void combine(char A[20], char B[20], int *count){ //generate every combination of 2 strings
	int i,j;
	char tmp[3];

	for(i=0; i<strlen(A); i++){
		for(j=0; j<strlen(B); j++){
			strcpy(tmp, "");
			/*
			tmp[0] = A[i];
			tmp[1] = B[j];
			*/
			//printf("%s\n", tmp);
			sprintf(tmp, "%c%c", A[i], B[j]);
			//printf("%s\n", tmp);
			//printf("\ntmp : %c %c %s\n",A[i], B[j], tmp);
			strcpy(baris[*count], tmp);
			(*count)++;
		}
	}
	//printf(" %d ",*count);
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

void printProd(int N){
	int i,j;

	for(i=0; i<N; i++){
		for(j=0; j<10; j++){
			printf("%s\t", Gram[i][j]);
		}
		printf("\n");
	}
}

/* sample
S->AB|BD
A->a|BA
B->DD|b
D->AB|a
*/

int main(){
	int i, j, k, l, m, n, prodNum;
	char start, prod[MAX_SIZE], strInput[MAX_SIZE], tmp, copy[10];

	printf("\nEnter start variable : ");
	scanf("%c",&start);
	printf("\nNumber of productions : ");
	scanf("%d", &prodNum);

	/* Input CFG in CNF form */
	for(i=0; i<prodNum; i++){
		scanf("%s",&prod);
		break_grammar(i, prod);
	}

	printf("\nEnter string to be checked : ");
	scanf("%s", &strInput);
	LENGTH = strlen(strInput);

	/* Fill in the main diagonal of matrix */
	for(i=0; i<LENGTH; i++){

		/* Initialize */
		tmp = strInput[i];
		strcpy(copy, "");

		for(j=0; j<prodNum; j++){
			for(k=1; k<10; k++){
				if(strlen(Gram[j][k]) == 1){
					if(Gram[j][k][0] == tmp){
						strcat(copy, Gram[j][0]);
					}
				}
			}
		}
		strcpy(DP[i][i], copy);
	}

	for(i=1; i<LENGTH; i++){

		/* Initialize */
		char A[5], B[5], ans[5];
		int cnt=0;
		strcpy(copy, "");
		//memset(baris, 0, sizeof baris);

		for(j=i; j<LENGTH; j++){
			cnt = 0;
			for(k=j-i; k<j; k++){

				/* Compare pre-computed strings */
				strcpy(ans, "");
				strcpy(A, DP[j-i][k]);
				strcpy(B, DP[k+1][j]);
				printf("{%s}{%s}",A,B);
				if(k!=j-1)printf(" U "); else printf(" = ");
				combine(A, B, &cnt);
			}

			/* Print comparation result */
			for(k=0; k<cnt; k++)printf("%s ",baris[k]);
			printf("\n");

			/* Compare with grammar table */
			strcpy(ans, "");
			for(k=0; k<prodNum; k++){
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
								//strcat(ans, " ");
							}
						}
					}
				}
			}
			strcpy(DP[j-i][j], ans);
		}
		printf("\n");
	}

	/* Print the CYK table */
	printf("CYK Table :\n");
	printCYK(LENGTH);

	/* Print token of productions */
	printProd(prodNum);
	

	/* Check whether the string can be generated or not */
	bool isAvail=false;
	char final[20];
	strcpy(final, DP[0][LENGTH-1]);
	for(i=0; i<strlen(final); i++){
		if(final[i] == start)isAvail = true;
	}
	if(isAvail)printf("\nResult : String %s can be generated!\n", strInput);
	else printf("\nResult : String %s cannot be generated!\n", strInput);
}
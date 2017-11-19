#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 100

/* Global variables */
int P, NP, LENGTH;
char DP[MAX_SIZE][MAX_SIZE][20], Gram[MAX_SIZE][MAX_SIZE][20];

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

/* sample
S->AB|BC
A->BA|a
B->CC|b
C->AB|a
*/

int main(){
	int i, j, k, l, m, n, prodNum;
	char start, prod[MAX_SIZE], strInput[MAX_SIZE], tmp[2], copy[10];

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
	//fgets(strInput, MAX_SIZE, stdin);
	scanf("%s", &strInput);
	LENGTH = strlen(strInput);

	/* Fill in the main diagonal of matrix */
	for(i=0; i<LENGTH; i++){

		/* Initialize */
		tmp[0] = strInput[i];
		strcpy(copy, "");

		for(j=0; j<prodNum; j++){
			for(k=1; k<10; k++){
				if(strlen(Gram[j][k]) == 1){
					if(strcmp(Gram[j][k], tmp) == 0){
						strcat(copy, Gram[j][0]);
					}
				}
			}
		}
		strcpy(DP[i][i], copy);
	}

	for(i=1; i<LENGTH; i++){

		/* Initialize */
		char A[5], B[5], baris[10][MAX_SIZE], idx=0;
		char ans[5];
		strcpy(copy, "");

		for(j=i; j<LENGTH; j++){
			idx = 0;
			for(k=j-i; k<j; k++){

				/* Compare pre-computed strings */
				strcpy(ans, "");
				strcpy(A, DP[j-i][k]);
				strcpy(B, DP[k+1][j]);
				printf("{%s}{%s}",A,B);
				if(k!=j-1)printf(" U "); else printf(" = ");
				for(l=0; l<strlen(A); l++){
					for(m=0; m<strlen(B); m++){
						strcpy(copy, "");
						copy[0] = A[l];
						copy[1] = B[m];
						strcpy(baris[idx], copy);
						idx++;
					}
				}
			}

			/* Print comparation result */
			for(k=0; k<idx; k++)printf("%s ",baris[k]);
			printf("\n");

			/* Compare with grammar table */
			strcpy(ans, "");
			for(k=0; k<prodNum; k++){
				for(l=1; l<10; l++){
					for(m=0; m<idx; m++){
						if(strcmp(baris[m], Gram[k][l]) == 0){

							/* Cek duplicate character */
							bool isExist = false;
							char exist[3];
							for(n=0; n<strlen(ans); n++){
								strcpy(exist, "");
								exist[0] = ans[n];
								if(strcmp(exist, Gram[k][0])==0)isExist = true;
							}
							if(!isExist)strcat(ans, Gram[k][0]);
							//concat(&ans, Gram[k][0]);
							//printf("%s\n", ans);
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
	for(i=0; i<LENGTH; i++){
		k = 0;
		for(j=LENGTH-i-1; j<LENGTH; j++){
			printf("{%s}\t", DP[k++][j]);
		}
		printf("\n");
	}

	/* Print token of productions
	for(i=0; i<prodNum; i++){
		for(j=0; j<10; j++){
			printf("%s\t", Gram[i][j]);
		}
		printf("\n");
	}*/

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

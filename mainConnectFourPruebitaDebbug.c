#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN_SIZE_ROW 6
#define MIN_SIZE_COLUMN 7
typedef struct connectFour{
	int *height;
	char **board;
	int turn;
	int inExecution;
	char pieces[2];
	int m;
	int n;
}ConnectFour;


ConnectFour init(int n, int m){
	ConnectFour cf;
	cf.pieces[0] = 'X';
	cf.pieces[1] = 'O';
	cf.m = m;
	cf.n = n;
	cf.height = malloc(m*sizeof(int));
	cf.board = (char**)malloc(n*sizeof(char*));
	for(int i = 0; i < cf.m; i++){
		cf.board[i] = (char*)malloc(m*sizeof(char));
		memset(cf.board[i], ' ', m);
	}
	memset(cf.height, 0, m*sizeof(int));
	cf.inExecution = 0;
	return cf;
}

int isValidSizeBoard(int n, int m){
	return (MIN_SIZE_ROW <= n && MIN_SIZE_COLUMN <= m) ? 1 : 0;
}

void printArray(int *a, int r){
	for(int i = 0; i < r; i++){
		printf("%d ", a[i]);
	}
}

void printToBoard(ConnectFour cf){
	printf("______________________\n");
	for(int i = 0; i < cf.n; i++){
		for(int j = 0; j < cf.m; j++){
			printf("|%c|", cf.board[i][j]);
		}
		printf("\n______________________\n");
	}
}


int inRangeColumn(int nColumns, int column){
	return (0<=column && column<nColumns) ? 1 : 0;
}


int inRangeRow(int nRows, int newHeight){
	return (newHeight <= nRows) ? 1 : 0;
}

int isValidMove(ConnectFour cf, int column){
	return (inRangeColumn(cf.m, column) && inRangeRow(cf.n, cf.height[column]+1));
}

void makeMove(ConnectFour *cf, int column){
	cf->board[cf->n - (cf->height[column]+1)][column] = cf->pieces[cf->turn%2];
	cf->height[column] = cf->height[column]+1;
}


int isFullBoard(ConnectFour cf){
	for(int j = 0; j < cf.m; j++){
		if(cf.height[j]<cf.n) return 0;
	}
	return 1;
}


int winInHorizontal(ConnectFour cf, int row, int column){
	int count = 0;
	for(int i = column-1; i >= 0; i--){
        if(cf.board[row][i] == cf.pieces[cf.turn%2]) count++; 
        else break;
    }
    for(int i = column+1; i < cf.m; i++){
        if(cf.board[row][i] == cf.pieces[cf.turn%2]) count++;
        else break;
    }
    printf("\nCount = %d, col = %d, row = %d\n", count, column, row);
	return (count >= 3) ? 1 : 0;
}


int winInVertical(ConnectFour cf, int row, int column){
	int count = 0;
    for(int i = row-1; i >= 0; i--) {
        if(cf.board[i][column] == cf.pieces[cf.turn%2]) count++;
        else break;
    }
    for(int i = row+1; i < cf.n; i++) {
        if(cf.board[i][column] == cf.pieces[cf.turn%2]) count++;
        else break;
    }
    return (count >= 3) ? 1 : 0;
}


int winInDiagonalDownRight(ConnectFour cf, int row, int column){
	int count = 0;
    int i = row-1;
    int j = column-1;
    while(i >= 0 && j >= 0 && cf.board[i][j] == cf.pieces[cf.turn%2]){
        count++;
        i--;
        j--;
    }
    i = row+1;
    j = column+1;
    while(i < cf.n && j < cf.m && cf.board[i][j] == cf.pieces[cf.turn%2]){
        count++;
        i++;
        j++;
    }
    return (count >= 3) ? 1 : 0;
}


int winInDiagonalUpRight(ConnectFour cf, int row, int column){
	int count = 0;
    int i = row+1;
    int j = column-1;
    while(i < cf.n && j >= 0 && cf.board[i][j] == cf.pieces[cf.turn%2]){
        count++;
        i++;
        j--;
    }
    i = row-1;
    j = column+1;
    while(i >= 0 && j < cf.m && cf.board[i][j] == cf.pieces[cf.turn%2]){
        count++;
        i--;
        j++;
    }
    return (count >= 3) ? 1 : 0;
}
//para checkear una victoria no nos hace falta buscar en todo el tablero
//con tener el ultimo movimiento e ir mirando las posiciones clave en torno al ultimo movimiento basta

int check_win(ConnectFour cf, int height, int column){
    int row = cf.n - height;
    printf("%d", row);
    return (winInDiagonalUpRight(cf, row, column) || winInDiagonalDownRight(cf, row, column) || winInVertical(cf, row, column) || winInHorizontal(cf, row, column));
}

int getColumnUsser(){
	int column;
	printf("Introduce la columna= \n");
	scanf("%d", &column);
	return column;
}

void pasaTurno(ConnectFour *cf){
	cf->turn = cf->turn +1;
}

int main(int argc, char *argv[]){
	printf("Init Connect Four i = {0->5}, j = {0->6}\n");
	ConnectFour cf = init(6, 7);
	printToBoard(cf);
	while(1){
		int column = getColumnUsser();
		if(isValidMove(cf, column)){
			makeMove(&cf, column);
			printToBoard(cf);
			if(check_win(cf, cf.height[column], column)) break;
			pasaTurno(&cf);
		}
		else printf("Columna no valida\n");
	}
	return 0;
}

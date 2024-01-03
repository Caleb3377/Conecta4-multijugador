#ifndef CONNECT_FOUR_H
#define CONNECT_FOUR_H
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

ConnectFour init(int n, int m);
void printToBoard(ConnectFour cf);
int inRangeColumn(int nColumns, int column);
int inRangeRow(int nRows, int newHeight);
int isValidMove(ConnectFour cf, int column);
void makeMove(ConnectFour *cf, int column);
int isFullBoard(ConnectFour cf);
int winInHorizontal(ConnectFour cf, int row, int column);
int winInVertical(ConnectFour cf, int row, int column);
int winInDiagonalDownRight(ConnectFour cf, int row, int column);
int winInDiagonalUpRight(ConnectFour cf, int row, int column);
int check_win(ConnectFour cf, int height, int column);
int getColumnUsser();
int isFirstPlayersTurn(ConnectFour cf);
void pasaTurno(ConnectFour *cf);

#endif

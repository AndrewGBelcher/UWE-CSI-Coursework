#include <graphics.h>

using namespace uwe;

class Board{

private:
	int** board = new int*[4];
	bool** mergeBoard = new bool*[4];

public:
	Board();

	// init functions
	void initBoard();
	void clearMerges();

	// movement
	void moveUp();
	void moveRight();
	void moveLeft();
	void moveDown();

	// visuals
	void display();
	void print();

	// checks
	bool isCellMerged();
	bool isFull();
	bool cantMove(void);
	bool chkBounds(int x, int y);

	// game control
	void won();
	bool gameOver(void);
	void addRandCell();
	void addRandCellDebug();
};


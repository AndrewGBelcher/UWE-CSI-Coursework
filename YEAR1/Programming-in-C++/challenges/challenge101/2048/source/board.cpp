#include "board.h"

// comment out debug to turn DEBUG_PRINT off
//#define DEBUG

// only works here, gets reset if used in header, if it aint broke dont fix it..
int score;

// The game board constructor
Board::Board(){

	// create our game board
	for(int i = 0; i < 4; ++i){
	    board[i] = new int[4];
	}

	//create a boolean board for merge detection
	for(int i = 0; i < 4; ++i){
		mergeBoard[i] = new bool[4];
	}
	
};

void Board::initBoard(){

	// init score
	score = 0;

	// init our 2048 board
	for (int y = 0; y < 4; y++){ 
		for (int x = 0; x < 4; x++){
			board[x][y] = 0;
		}
	}
	// init our boolean board for merge detection
	for (int y = 0; y < 4; y++){ 
		for (int x = 0; x < 4; x++){
			mergeBoard[x][y] = 0;
		}
	}

	// add the first cell to start the game
	addRandCell();
}

// reset the merge board so on next turn blocks can be merged again
void Board::clearMerges(){
	for(int e = 0; e < 4; e++){
		for(int x = 0; x < 4; x++){
			mergeBoard[e][x] = false;
		}
	}
}

// Movment functions to shift our board, tried rotation but wasnt reliable for some reason, this is simpler for now
void Board::moveUp(){
	for(int e = 0; e < 3; e++){
		for(int x = 0; x < 4; x++){
			for(int y = 0; y < 3; y++){

				// merge
				if(board[x][y] == board[x][y+1] && !mergeBoard[x][y] && !mergeBoard[x][y+1]){
	
					// merge new block and set old to 0
					board[x][y] = board[x][y] + board[x][y+1];
					board[x][y+1]=0;

					// set the merge applied block to true
					mergeBoard[x][y] = true;

					// lets update our score with the value of both merged blocks
					score = score + board[x][y];
				}
				// move
				else if(board[x][y] == 0 && board[x][y+1] != 0){

					// just move our block
					board[x][y] = board[x][y+1];
					board[x][y+1] = 0;

					// move the boolean and set the previous to false
					mergeBoard[x][y] = mergeBoard[x][y+1];
					mergeBoard[x][y+1] = false;
				}
			}
		}
	}
	// important to clear the merge board so that blocks can be merged on next turn
	clearMerges();


}

void Board::moveRight(){
	for(int e = 0; e < 4; e++){
		for(int x = 1; x < 4; x++){
			for(int y = 0; y < 4; y++){

				// merge
				if(board[x][y] == board[x-1][y] && !mergeBoard[x][y] && !mergeBoard[x-1][y]){
					
					board[x][y] = board[x][y] + board[x-1][y];
					board[x-1][y]=0;

					mergeBoard[x][y] = true;

					score = score + board[x][y];
				}
				// move
				else if(board[x][y] == 0 && board[x-1][y] != 0){

					board[x][y] = board[x-1][y];
					board[x-1][y] = 0;

					mergeBoard[x][y] = mergeBoard[x-1][y];
					mergeBoard[x-1][y] = false;

				}
			}
		}
	}
	clearMerges();
}

void Board::moveLeft(){

	for(int e = 0; e < 4; e++){
		for(int y = 0; y < 4; y++){
			for(int x = 0; x < 3; x++){

				// merge
				if(board[x][y] == board[x+1][y] && !mergeBoard[x][y] && !mergeBoard[x+1][y]){
						
					board[x][y] = board[x][y] + board[x+1][y];
					board[x+1][y]=0;

					mergeBoard[x][y] = true;

					score = score + board[x][y];
				}
				// move
				else if(board[x][y] == 0 && board[x+1][y] != 0){

					board[x][y] = board[x+1][y];
					board[x+1][y] = 0;

					mergeBoard[x][y] = mergeBoard[x+1][y];
					mergeBoard[x+1][y] = false;
				}
			}
		}			
	}
	clearMerges();


}
void Board::moveDown(){

	for(int e = 0; e < 4; e++){
		for(int x = 0; x < 4; x++){
			for(int y = 1; y < 4; y++){

				// merge
				if(board[x][y] == board[x][y-1] && !mergeBoard[x][y] && !mergeBoard[x][y-1]){
					
					board[x][y] = board[x][y] + board[x][y-1];
					board[x][y-1]=0;

					mergeBoard[x][y] = true;

					score = score + board[x][y];

				}
				// move
				else if(board[x][y] == 0 && board[x][y-1] != 0){

					board[x][y] = board[x][y-1];
					board[x][y-1] = 0;
					
					mergeBoard[x][y] = mergeBoard[x][y-1];
					mergeBoard[x][y-1] = false;
				}
			}
		}
	}
	clearMerges();
}


// draw the board, and the cells and the score etc
void Board::display(){

	// style variables
	int numFont = 25;

	// draw title + credits
	setColour(Colours::black);
	setFontSize(40);
	drawTextCenter("2048",5,5,100,50);


	setColour(Colours::black);
	setFontSize(10);
	drawTextCenter("Developed by",300,0,100,50);
	drawTextCenter( "Andrew Belcher",300,10,100,50);


	// draw score
	char scoreBuf[50];
	sprintf(scoreBuf,"%d",score);

	setColour(Colours::black);
	setFontSize(20);
	drawTextCenter("SCORE",170,-10,80,50);

	setColour(Colours::black);
	setFontSize(20);
	drawTextCenter(scoreBuf,165,15,80,50);

	// init drawing positions
	int xx = 10;
	int yy = 60;

	// now draw the grid and change colors based on blocks
	for(int i =0; i < 4; i++){
		for(int d =0; d < 4; d++){

			char buf[50];
			sprintf(buf,"%d",board[d][i]);

			switch(board[d][i]){
				case 0:
					setColour(Colours::lightblue);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					break;

				case 2:

					setColour(Colours::lightgreen);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					setColour(Colours::white);
					drawText(buf,xx+40,yy,100,100);
					break;
				case 4:

					setColour(Colours::springgreen);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					setColour(Colours::white);
					drawText(buf,xx+40,yy,100,100);
					break;
				case 8:

					setColour(Colours::lightsalmon);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					setColour(Colours::white);
					drawText(buf,xx+40,yy,100,100);
					break;
				case 16:

					setColour(Colours::salmon);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					setColour(Colours::white);
					drawText(buf,xx+30,yy,100,100);
					break;
				case 32:

					setColour(Colours::firebrick);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					setColour(Colours::white);
					drawText(buf,xx+30,yy,100,100);
					break;
				case 64:

					setColour(Colours::darkred);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					setColour(Colours::white);
					drawText(buf,xx+30,yy,100,100);
					break;
				case 128:

					setColour(Colours::palegoldenrod);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					setColour(Colours::white);
					drawText(buf,xx+30,yy,100,100);
					break;
				case 256:

					setColour(Colours::goldenrod);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					setColour(Colours::white);
					drawText(buf,xx+25,yy,100,100);
					break;
				case 512:

					setColour(Colours::darkgoldenrod);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					setColour(Colours::white);
					drawText(buf,xx+25,yy,100,100);
					break;
				case 1024:

					setColour(Colours::purple);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					setColour(Colours::white);
					drawText(buf,xx+20,yy,100,100);
					break;
				case 2048:

					setColour(Colours::silver);
					setFontSize(numFont);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					setColour(Colours::white);
					drawText(buf,xx+20,yy,100,100);
					break;
				default:
					setColour(Colours::black);
					drawFilledRect(xx,yy,400/4 - 10,400/4- 10);
					break;
				}

			// allows use to position the next colloum
			xx = xx+100;
		}
		// adjust from screen edge and draw next row
		xx = 10;
		yy = yy+100;
	}
	
}


// draw the board to console just cause
void Board::print(){

	printf("-----------------------------\n");
	
	for( int i = 0; i < 4; ++i ){
		printf("|      |      |      |      |\n");
		printf("|");
		// for each cell in row
		for(int j = 0; j < 4; ++j){
			if(board[j][i] != 0){

				printf(" %-4d |", board[j][i]);
				
				// print the indexs
				//printf("  %d%d  |", j,i);
			}
			else{
				printf("      |");
			}
		}
	printf("\n|      |      |      |      |");
	printf("\n-----------------------------\n");

	}
}	

bool Board::isFull(){   

	bool full = true;

	for(int x = 0; x < 4; ++x){
        	for (int y = 0; y < 4; ++y){
            		if (board[x][y] == 0){
                		full = false;
			}
		}
	}
	return full;
}

bool Board::chkBounds(int x, int y){

	if(x > 3 || x < 0 || y > 3 || y < 0){
		return false;
	}
	else{
		return true;
	}
}

// checked before moving but only when board is full
bool Board::cantMove(){

	bool hasMove = false;

	for(int i = 0; i < 4; i++){
		for(int x = 0; x < 4; x++){
			for(int y = 0; y < 4; y++){
				switch(i){
					case 0:
						if(chkBounds(x+1,y)){
							if(board[x][y] == board[x+1][y]){
								hasMove = true;
							}
								 
						}
					case 1:
						if(chkBounds(x-1,y)){
							if(board[x][y] == board[x-1][y]){
								hasMove = true;
							}
						}
					case 2:
						if(chkBounds(x,y+1)){
							if(board[x][y] == board[x][y+1]){
								hasMove = true;
							}
						}
					case 3:
						if(chkBounds(x,y-1)){
							if(board[x][y] == board[x][y-1]){
								hasMove = true;
							}
						}
					default:; // do nothing

				}
			}
		}
	}
	return hasMove;
	// use is in bounds on those being checked
}

// same function but allows us to turn off new cells on moves but enables on hitting space
void Board::addRandCellDebug() {

	// lets prevent an infinite loop lol
	if(!isFull()){

		srand(clock());

		// select a number to place on the board
		int num = 0;
		int newFill = rand()%100;

		if(newFill < 90){
			num = 2;
		}
		else{
			num = 4;
		}	

		// now randomly select free cells to place new number

		int placeNumW;
		int placeNumH;

		bool numPlaced = false;

		while(!numPlaced){

			placeNumW = rand()%4; 	
			placeNumH = rand()%4; 

			if(board[placeNumW][placeNumH] == 0){
				board[placeNumW][placeNumH] = num;
				numPlaced = true;
			}
		}
	}
}

void Board::addRandCell() {

#ifdef DEBUG

	// do nothing
	return;
#else
	// lets prevent an infinite loop lol
	if(!isFull()){
		// select a number to place on the board

		srand(clock());

		int num = 0;
		int newFill = rand()%100;

		if(newFill < 90){
			num = 2;
		}
		else{
			num = 4;
		}	

		// now randomly select free cells to place new number

		int placeNumW;
		int placeNumH;

		bool numPlaced = false;

		while(!numPlaced){

			placeNumW = rand()%4; 	
			placeNumH = rand()%4; 

			if(board[placeNumW][placeNumH] == 0){
				board[placeNumW][placeNumH] = num;
				numPlaced = true;
			}
		}
	}
#endif
}

// check the board for 2048 - runs every frame
void Board::won(){
	// check board for 2048

	int e = 0;

	for(int h = 0; h < 4; h++){
		for(int w = 0; w < 4; w++){
			if(board[w][h] == 2048){
				printf("you won!\n");
				exit(0);
			}

			// check if board is full && cant move
			else if(isFull() && !cantMove()){

				gameOver();
			}
		}
	}
}

// only when you cant move && board is full
bool Board::gameOver(void){

	if(isFull() && !cantMove()){

		setColour(Colours::white);
		drawFilledRect(50,190,300,100);
		setColour(Colours::black);
		drawRect(50,190,300,100,1);
		setColour(Colours::black);
		setFontSize(20);
		drawTextCenter("Try Again? - Press Enter",0,200,400,50);
		drawTextCenter("Give up? - Press Q",10,225,400,50);

		return true;
	}
	return false;	
}


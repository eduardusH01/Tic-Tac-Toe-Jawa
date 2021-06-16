#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

// Final Project Ai - Gamedev
using namespace std;
using namespace sf;

struct Move {
	int row, col;
};


// mengecek apakah board masih ada yang kosong atau tidak
bool isMovesLeft(char board[3][3]) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (board[i][j] == '_')
				return true;
	return false;
}

// mengecek apakah ada yang 3 in a row
int evaluate(char b[3][3]) {
	//mengecek 3 in a row dalam baris
	for (int row = 0; row < 3; row++) {
		if (b[row][0] == b[row][1] && b[row][1] == b[row][2]) {
			if (b[row][0] == 'x') return +10;
			else if (b[row][0] == 'o') return -10;
		}
	}

	// mengecek 3 in a row dalam kolom
	for (int col = 0; col < 3; col++) {
		if (b[0][col] == b[1][col] && b[1][col] == b[2][col]) {
			if (b[0][col] == 'x') return +10;
			else if (b[0][col] == 'o') return -10;
		}
	}

	// mengecek 3 in a row dalam diagonal
	if (b[0][0] == b[1][1] && b[1][1] == b[2][2]) {
		if (b[0][0] == 'x') return +10;
		else if (b[0][0] == 'o') return -10;
	}

	if (b[0][2] == b[1][1] && b[1][1] == b[2][0]) {
		if (b[0][2] == 'x') return +10;
		else if (b[0][2] == 'o') return -10;
	}

	// jika draw
	return 0;
}

// minimax function, searching utk segala possible move dan mereturn value dari sebuah move
int minimax(char board[3][3], int depth, bool isMax) {
	int score = evaluate(board);

	// Jika maximizer menang
	if (score == 10)
		return score;

	// Jika minimizer menang
	if (score == -10)
		return score;

	// No winner/tie condition
	if (isMovesLeft(board) == false)
		return 0;

	// If this maximizer's move
	if (isMax){
		int best = -1000;

		// traverse all cells
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				// check if cell is empty
				if (board[i][j] == '_') {
					// Make the move
					board[i][j] = 'x';

					// Call minimax recursively and choose the maximum value
					best = max(best, minimax(board, depth + 1, !isMax));

					// Undo the move
					board[i][j] = '_';
				}
			}
		}
		return best;
	}

	// If this minimizer's move
	else {
		int best = 1000;

		// traverse all cells
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				// check if cell is empty
				if (board[i][j] == '_') {
					// Make the move
					board[i][j] = 'o';

					// Call minimax recursively and choose the minimum value
					best = min(best, minimax(board, depth + 1, !isMax));

					// Undo the move
					board[i][j] = '_';
				}
			}
		}
		return best;
	}
}

// Akan mereturn the best possible move utk maximizing player
Move findBestMoveMax(char board[3][3]) {
	int bestVal = -1000;
	Move bestMove;
	bestMove.row = -1;
	bestMove.col = -1;

	// Traverse all cells, evaluate minimax function for all empty cells, then return the cell with optimal value.
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			// Check if cell is empty
			if (board[i][j] == '_'){
				// Make the move
				board[i][j] = 'x';

				// compute evaluation function for this move.
				int moveVal = minimax(board, 0, false);

				// Undo the move
				board[i][j] = '_';

				// If the value of the current move is more than the best value, then update best value
				if (moveVal > bestVal) {
					bestMove.row = i;
					bestMove.col = j;
					bestVal = moveVal;
				}
			}
		}
	}

	/*
	printf("The value of the best Move is : %d\nRow : %d Col: %d\n",
		bestVal, bestMove.row, bestMove.col);
		*/

	return bestMove;
}

Move findBestMoveMin(char board[3][3]) {
	int bestVal = 1000;
	Move bestMove;
	bestMove.row = -1;
	bestMove.col = -1;

	// Traverse all cells, evaluate minimax function for all empty cells, then return the cell with optimal value.
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			// Check if cell is empty
			if (board[i][j] == '_') {
				// Make the move
				board[i][j] = 'o';

				// compute evaluation function for this move.
				int moveVal = minimax(board, 0, true);

				// Undo the move
				board[i][j] = '_';

				// If the value of the current move is less than the best value, then update best value
				if (moveVal < bestVal) {
					bestMove.row = i;
					bestMove.col = j;
					bestVal = moveVal;
				}
			}
		}
	}
	/*
	printf("The value of the best Move is : %d\nRow : %d Col: %d\n",
		bestVal, bestMove.row, bestMove.col);
	*/
	return bestMove;
}

View getLetterboxView(sf::View view, int windowWidth, int windowHeight) {
	float windowRatio = windowWidth / (float)windowHeight;
	float viewRatio = view.getSize().x / (float)view.getSize().y;
	float sizeX = 1;
	float sizeY = 1;
	float posX = 0;
	float posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	if (horizontalSpacing) {
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
	}

	else {
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.f;
	}

	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

	return view;
}


int main() {
	int resX = 800, resY = 800; // ukuran window
	RenderWindow window(VideoMode(resX, resY), "Tic Tac Toe Jawa", (Style::Resize + Style::Close));

	sf::View view;
	view.setSize(resX, resY);
	view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
	view = getLetterboxView(view, resX, resY);

	Music music;
	music.openFromFile("audio/cublakCublakSuweng.ogg");
	music.play();
	music.setLoop(true);

	//gambar pada menu
	Texture MENU;
	MENU.loadFromFile("image/Menu.png");
	Sprite menu(MENU);

	Texture PLAYVSPLAYER;
	PLAYVSPLAYER.loadFromFile("image/playvsplayer.png");
	Sprite playvsplayer(PLAYVSPLAYER);
	playvsplayer.setPosition(50, 325);

	Texture PLAYVSPLAYER1;
	PLAYVSPLAYER1.loadFromFile("image/playvsplayer1.png");
	Sprite playvsplayer1(PLAYVSPLAYER1);
	playvsplayer1.setPosition(50, 325);

	Texture PLAYVSAI;
	PLAYVSAI.loadFromFile("image/playvsai.png");
	Sprite playvsai(PLAYVSAI);
	playvsai.setPosition(50, 475);

	Texture PLAYVSAI1;
	PLAYVSAI1.loadFromFile("image/playvsai1.png");
	Sprite playvsai1(PLAYVSAI1);
	playvsai1.setPosition(50, 475);

	//tombol quit
	Texture QUIT;
	QUIT.loadFromFile("image/quit.png");
	Sprite quit(QUIT);
	quit.setPosition(50, 625);

	Texture QUIT1;
	QUIT1.loadFromFile("image/quit1.png");
	Sprite quit1(QUIT1);
	quit1.setPosition(50, 625);

	Texture FIRSTORSEC;
	FIRSTORSEC.loadFromFile("image/firstorsec.png");
	Sprite firstorsec(FIRSTORSEC);

	Texture FIRST;
	FIRST.loadFromFile("image/first.png");
	Sprite first(FIRST);
	first.setPosition(200, 375);

	Texture FIRST1;
	FIRST1.loadFromFile("image/first1.png");
	Sprite first1(FIRST1);
	first1.setPosition(200, 375);

	Texture SECOND;
	SECOND.loadFromFile("image/second.png");
	Sprite second(SECOND);
	second.setPosition(200, 525);

	Texture SECOND1;
	SECOND1.loadFromFile("image/second1.png");
	Sprite second1(SECOND1);
	second1.setPosition(200, 525);

	// gambar board pada saat game berjalan
	Texture PAPAN;
	PAPAN.loadFromFile("image/board.png");
	Sprite papan(PAPAN);

	//giliran player1
	Texture PLAYER1;
	PLAYER1.loadFromFile("image/player1.png");
	Sprite player1(PLAYER1);

	//giliran player2
	Texture PLAYER2;
	PLAYER2.loadFromFile("image/player2.png");
	Sprite player2(PLAYER2);

	Texture X;
	X.loadFromFile("image/markx.png");
	Sprite markx(X);

	Texture O;
	O.loadFromFile("image/marko.png");
	Sprite marko(O);

	Texture XWIN;
	XWIN.loadFromFile("image/xwin.png");
	Sprite xwin(XWIN);

	Texture OWIN;
	OWIN.loadFromFile("image/owin.png");
	Sprite owin(OWIN);

	// gambar pada saat game selesai
	Texture PLAYER1WIN;
	PLAYER1WIN.loadFromFile("image/player1win.png");
	Sprite player1win(PLAYER1WIN);

	Texture PLAYER2WIN;
	PLAYER2WIN.loadFromFile("image/player2win.png");
	Sprite player2win(PLAYER2WIN);

	Texture AIWIN;
	AIWIN.loadFromFile("image/AIwin.png");
	Sprite aiwin(AIWIN);

	Texture PWIN;
	PWIN.loadFromFile("image/Pwin.png");
	Sprite pwin(PWIN);

	Texture DRAW;
	DRAW.loadFromFile("image/draw.png");
	Sprite draw(DRAW);

	//tawaran untuk rematch
	Texture REMATCH;
	REMATCH.loadFromFile("image/rematch.png");
	Sprite rematch(REMATCH);

	Texture YES;
	YES.loadFromFile("image/yes.png");
	Sprite yes(YES);
	yes.setPosition(200, 415);

	Texture YES1;
	YES1.loadFromFile("image/yes1.png");
	Sprite yes1(YES1);
	yes1.setPosition(200, 415);

	Texture NO;
	NO.loadFromFile("image/no.png");
	Sprite no(NO);
	no.setPosition(200, 565);

	Texture NO1;
	NO1.loadFromFile("image/no1.png");
	Sprite no1(NO1);
	no1.setPosition(200, 565);


	int point = 0; // penunjuk pada menu
	int clk = 0;

	float x[3] = { 170.0, 340.0, 510.0 };
	float y[3] = { 285.0, 450.0, 615.0 };

	char board[3][3] = { '_', '_', '_', '_', '_', '_', '_', '_', '_' };

	int phase = 0; // fase game awal menu = 0, vsAI hendak 1st turn atau 2nd turn = 1, fase main game = 2, rematch or not = 4, selesai atau keluar = 3
	int player = 1; // player 1 atau 2
	int choice = 0; // memilih pilihan
	char mark; // x atau o tergantung urutannya siapa
	bool keyboard = true, isUsingAi, isAiFirst, isAiAllowed = true, isEnd = false;

	while (window.isOpen()) {
		if (phase == 0) { // bagian awal (memilih play/quit)
			window.draw(menu); // menampilkan sprite menu
			(point == 0) ? window.draw(playvsplayer1) : window.draw(playvsplayer); // menampilkan pilihan play vs player
			(point == 1) ? window.draw(playvsai1) : window.draw(playvsai); // menampilkan pilihan play vs ai
			(point == 2) ? window.draw(quit1) : window.draw(quit); // menampilkan pilihan quit

			if (Keyboard::isKeyPressed(Keyboard::Up)) { // bila up arrow ditekan pilihan akan naik
				clk++;
				if (clk > 150) {
					point--;
					if (point < 0)
						point = 2;
					clk = 0;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) { // bila down arrow ditekan pilihan akan turun
				clk++;
				if (clk > 150) {
					point++;
					if (point > 2)
						point = 0;
					clk = 0;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Enter)) { // bila enter ditekan maka akan memilih
				clk++;
				if (clk > 150) {
					if (point == 0) {
						isUsingAi = false;
						phase = 2;
					}
					else if (point == 1) {
						isUsingAi = true;
						phase = 1;
					}
					else if (point == 2) {
						phase = 3;
					}
					point = 0;
					clk = 0;
				}
			}
		}

		if (phase == 1) {
			window.draw(firstorsec);
			(point == 0) ? window.draw(first1) : window.draw(first); // 
			(point == 1) ? window.draw(second1) : window.draw(second); // 
			if (Keyboard::isKeyPressed(Keyboard::Up)) { // bila up arrow ditekan pilihan akan naik
				clk++;
				if (clk > 150) {
					point--;
					if (point < 0)
						point = 1;
					clk = 0;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) { // bila down arrow ditekan pilihan akan turun
				clk++;
				if (clk > 150) {
					point++;
					if (point > 1)
						point = 0;
					clk = 0;
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Enter)) { // bila enter ditekan maka akan memilih
				clk++;
				if (clk > 150) {
					if (point == 0) {
						isAiFirst = false;
					}
					else if (point == 1) {
						isAiFirst = true;
					}
					phase = 2;
					point = 0;
					clk = 0;
				}
			}
		}

		//fase main
		else if (phase == 2) {
			window.draw(papan);
			player = (player % 2) ? 1 : 2; // menentukan giliran player 1 atau 2
			if (isEnd == false) {
				(player == 1) ? window.draw(player1) : window.draw(player2);
			}
			mark = (player == 1) ? 'x' : 'o'; // player 1 x, player 2 o

			if (isUsingAi == true && isAiAllowed == true) {
				if (isAiFirst == true) {
					if (player == 1) {
						keyboard = false;
						Move bestMove = findBestMoveMax(board);
						if (bestMove.row == 0 && bestMove.col == 0) {
							choice = 1;
						}
						else if (bestMove.row == 0 && bestMove.col == 1) {
							choice = 2;
						}
						else if (bestMove.row == 0 && bestMove.col == 2) {
							choice = 3;
						}
						else if (bestMove.row == 1 && bestMove.col == 0) {
							choice = 4;
						}
						else if (bestMove.row == 1 && bestMove.col == 1) {
							choice = 5;
						}
						else if (bestMove.row == 1 && bestMove.col == 2) {
							choice = 6;
						}
						else if (bestMove.row == 2 && bestMove.col == 0) {
							choice = 7;
						}
						else if (bestMove.row == 2 && bestMove.col == 1) {
							choice = 8;
						}
						else if (bestMove.row == 2 && bestMove.col == 2) {
							choice = 9;
						}
					}
					else if (player == 2) {
						keyboard = true;
					}
				}
				else {
					if (player == 2) {
						keyboard = false;
						Move bestMove = findBestMoveMin(board);
						if (bestMove.row == 0 && bestMove.col == 0) {
							choice = 1;
						}
						else if (bestMove.row == 0 && bestMove.col == 1) {
							choice = 2;
						}
						else if (bestMove.row == 0 && bestMove.col == 2) {
							choice = 3;
						}
						else if (bestMove.row == 1 && bestMove.col == 0) {
							choice = 4;
						}
						else if (bestMove.row == 1 && bestMove.col == 1) {
							choice = 5;
						}
						else if (bestMove.row == 1 && bestMove.col == 2) {
							choice = 6;
						}
						else if (bestMove.row == 2 && bestMove.col == 0) {
							choice = 7;
						}
						else if (bestMove.row == 2 && bestMove.col == 1) {
							choice = 8;
						}
						else if (bestMove.row == 2 && bestMove.col == 2) {
							choice = 9;
						}
					}
					else if (player == 1) {
						keyboard = true;
					}
				}
			}

			if (keyboard == true) { // input 1 sampai 9
				if (Keyboard::isKeyPressed(Keyboard::Num1)) choice = 1;
				if (Keyboard::isKeyPressed(Keyboard::Num2)) choice = 2;
				if (Keyboard::isKeyPressed(Keyboard::Num3)) choice = 3;
				if (Keyboard::isKeyPressed(Keyboard::Num4)) choice = 4;
				if (Keyboard::isKeyPressed(Keyboard::Num5)) choice = 5;
				if (Keyboard::isKeyPressed(Keyboard::Num6)) choice = 6;
				if (Keyboard::isKeyPressed(Keyboard::Num7)) choice = 7;
				if (Keyboard::isKeyPressed(Keyboard::Num8)) choice = 8;
				if (Keyboard::isKeyPressed(Keyboard::Num9)) choice = 9;
			}

			// memberi X atau O dan mengganti player
			if (choice == 1 && board[0][0] == '_') {
				board[0][0] = mark;
				player++;
			}
			if (choice == 2 && board[0][1] == '_') {
				board[0][1] = mark;
				player++;
			}
			if (choice == 3 && board[0][2] == '_') {
				board[0][2] = mark;
				player++;
			}
			if (choice == 4 && board[1][0] == '_') {
				board[1][0] = mark;
				player++;
			}
			if (choice == 5 && board[1][1] == '_') {
				board[1][1] = mark;
				player++;
			}
			if (choice == 6 && board[1][2] == '_') {
				board[1][2] = mark;
				player++;
			}
			if (choice == 7 && board[2][0] == '_') {
				board[2][0] = mark;
				player++;
			}
			if (choice == 8 && board[2][1] == '_') {
				board[2][1] = mark;
				player++;
			}
			if (choice == 9 && board[2][2] == '_') {
				board[2][2] = mark;
				player++;
			}

			// menggambar X atau O pada board
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (board[i][j] == 'x') {
						markx.setPosition(x[j], y[i]);
						window.draw(markx);
					}
					else if (board[i][j] == 'o') {
						marko.setPosition(x[j], y[i]);
						window.draw(marko);
					}
				}
			}

			// win condition
			if (board[0][0] != '_' && board[0][0] == board[0][1] && board[0][1] == board[0][2] ||
				board[1][0] != '_' && board[1][0] == board[1][1] && board[1][1] == board[1][2] ||
				board[2][0] != '_' && board[2][0] == board[2][1] && board[2][1] == board[2][2] ||
				board[0][0] != '_' && board[0][0] == board[1][0] && board[1][0] == board[2][0] ||
				board[0][1] != '_' && board[0][1] == board[1][1] && board[1][1] == board[2][1] ||
				board[0][2] != '_' && board[0][2] == board[1][2] && board[1][2] == board[2][2] ||
				board[0][0] != '_' && board[0][0] == board[1][1] && board[1][1] == board[2][2] ||
				board[0][2] != '_' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
				isEnd = true;
				isAiAllowed = false;
				keyboard = false; // agar saat game sudah selesai, tidak bisa input lagi
				if (board[0][0] != '_' && board[0][0] == board[0][1] && board[0][1] == board[0][2]) {
					if (board[0][0] == 'x') {
						xwin.setPosition(x[0], y[0]);
						window.draw(xwin);
						xwin.setPosition(x[1], y[0]);
						window.draw(xwin);
						xwin.setPosition(x[2], y[0]);
						window.draw(xwin);
					}
					else if (board[0][0] == 'o') {
						owin.setPosition(x[0], y[0]);
						window.draw(owin);
						owin.setPosition(x[1], y[0]);
						window.draw(owin);
						owin.setPosition(x[2], y[0]);
						window.draw(owin);
					}
				}
				else if (board[1][0] != '_' && board[1][0] == board[1][1] && board[1][1] == board[1][2]) {
					if (board[1][0] == 'x') {
						xwin.setPosition(x[0], y[1]);
						window.draw(xwin);
						xwin.setPosition(x[1], y[1]);
						window.draw(xwin);
						xwin.setPosition(x[2], y[1]);
						window.draw(xwin);
					}
					else if (board[1][0] == 'o') {
						owin.setPosition(x[0], y[1]);
						window.draw(owin);
						owin.setPosition(x[1], y[1]);
						window.draw(owin);
						owin.setPosition(x[2], y[1]);
						window.draw(owin);
					}
				}
				else if (board[2][0] != '_' && board[2][0] == board[2][1] && board[2][1] == board[2][2]) {
					if (board[2][0] == 'x') {
						xwin.setPosition(x[0], y[2]);
						window.draw(xwin);
						xwin.setPosition(x[1], y[2]);
						window.draw(xwin);
						xwin.setPosition(x[2], y[2]);
						window.draw(xwin);
					}
					else if (board[2][0] == 'o') {
						owin.setPosition(x[0], y[2]);
						window.draw(owin);
						owin.setPosition(x[1], y[2]);
						window.draw(owin);
						owin.setPosition(x[2], y[2]);
						window.draw(owin);
					}
				}
				else if (board[0][0] != '_' && board[0][0] == board[1][0] && board[1][0] == board[2][0]) {
					if (board[0][0] == 'x') {
						xwin.setPosition(x[0], y[0]);
						window.draw(xwin);
						xwin.setPosition(x[0], y[1]);
						window.draw(xwin);
						xwin.setPosition(x[0], y[2]);
						window.draw(xwin);
					}
					else if (board[0][0] == 'o') {
						owin.setPosition(x[0], y[0]);
						window.draw(owin);
						owin.setPosition(x[0], y[1]);
						window.draw(owin);
						owin.setPosition(x[0], y[2]);
						window.draw(owin);
					}
				}
				else if (board[0][1] != '_' && board[0][1] == board[1][1] && board[1][1] == board[2][1]) {
					if (board[0][1] == 'x') {
						xwin.setPosition(x[1], y[0]);
						window.draw(xwin);
						xwin.setPosition(x[1], y[1]);
						window.draw(xwin);
						xwin.setPosition(x[1], y[2]);
						window.draw(xwin);
					}
					else if (board[0][1] == 'o') {
						owin.setPosition(x[1], y[0]);
						window.draw(owin);
						owin.setPosition(x[1], y[1]);
						window.draw(owin);
						owin.setPosition(x[1], y[2]);
						window.draw(owin);
					}
				}
				else if (board[0][2] != '_' && board[0][2] == board[1][2] && board[1][2] == board[2][2]) {
					if (board[0][2] == 'x') {
						xwin.setPosition(x[2], y[0]);
						window.draw(xwin);
						xwin.setPosition(x[2], y[1]);
						window.draw(xwin);
						xwin.setPosition(x[2], y[2]);
						window.draw(xwin);
					}
					else if (board[0][2] == 'o') {
						owin.setPosition(x[2], y[0]);
						window.draw(owin);
						owin.setPosition(x[2], y[1]);
						window.draw(owin);
						owin.setPosition(x[2], y[2]);
						window.draw(owin);
					}
				}
				else if (board[0][0] != '_' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
					if (board[0][0] == 'x') {
						xwin.setPosition(x[0], y[0]);
						window.draw(xwin);
						xwin.setPosition(x[1], y[1]);
						window.draw(xwin);
						xwin.setPosition(x[2], y[2]);
						window.draw(xwin);
					}
					else if (board[0][0] == 'o') {
						owin.setPosition(x[0], y[0]);
						window.draw(owin);
						owin.setPosition(x[1], y[1]);
						window.draw(owin);
						owin.setPosition(x[2], y[2]);
						window.draw(owin);
					}
				}
				else if (board[0][2] != '_' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
					if (board[0][2] == 'x') {
						xwin.setPosition(x[2], y[0]);
						window.draw(xwin);
						xwin.setPosition(x[1], y[1]);
						window.draw(xwin);
						xwin.setPosition(x[0], y[2]);
						window.draw(xwin);
					}
					else if (board[0][2] == 'o') {
						owin.setPosition(x[2], y[0]);
						window.draw(owin);
						owin.setPosition(x[1], y[1]);
						window.draw(owin);
						owin.setPosition(x[0], y[2]);
						window.draw(owin);
					}
				}

				// menggambar siapa yang menang
				if (isUsingAi == true) {
					if (isAiFirst == true) {
						(player % 2) ? window.draw(pwin) : window.draw(aiwin);
					}
					else {
						(player % 2) ? window.draw(aiwin) : window.draw(pwin);
					}
				}
				else {
					(player % 2) ? window.draw(player2win) : window.draw(player1win);
				}
				
				// bila enter ditekan maka akan masuk ke pilihan rematch
				if (Keyboard::isKeyPressed(Keyboard::Enter)) {
					clk++;
					if (clk > 150) {
						phase = 4;
						clk = 0;
					}
				}
			}

			// kondisi draw
			else if (board[0][0] != '_' && board[0][1] != '_' && board[0][2] != '_' && board[1][0] != '_' && board[1][1] != '_' && board[1][2] != '_' && board[2][0] != '_' && board[2][1] != '_' && board[2][2] != '_') {
				isEnd = true;
				isAiAllowed = false;
				window.draw(draw);
				if (Keyboard::isKeyPressed(Keyboard::Enter)) {
					clk++;
					if (clk > 150) {
						phase = 4;
						clk = 0;
					}
				}
			}
		}

		if (phase == 3) // quit dari game
			window.close();

		// rematch ?
		if (phase == 4) {
			window.draw(rematch);
			(point == 0) ? window.draw(yes1) : window.draw(yes); // menampilkan pilihan ya
			(point == 1) ? window.draw(no1) : window.draw(no); // menampilkan pilihan tidak

			// bila up arrow ditekan pilihan akan naik
			if (Keyboard::isKeyPressed(Keyboard::Up)) {
				clk++;
				if (clk > 150) {
					point--;
					if (point < 0)
						point = 1;
					clk = 0;
				}
			}
			// bila down arrow ditekan pilihan akan turun
			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				clk++;
				if (clk > 150) {
					point++;
					if (point > 1)
						point = 0;
					clk = 0;
				}
			}

			// bila enter ditekan maka akan memilih
			if (Keyboard::isKeyPressed(Keyboard::Enter)) {
				clk++;
				if (clk > 150) {
					if (point == 0) {
						phase = 2;
					}
					else if (point == 1) {
						phase = 0;
					}
					point = 0;
					clk = 0;

					//me-reset game dari awal
					isAiAllowed = true;
					keyboard = true;;
					isEnd = false;
					player = 1;
					choice = 0;
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							board[i][j] = '_';
						}
					}
				}
			}
		}

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::Resized)
				view = getLetterboxView(view, event.size.width, event.size.height);
		}

		window.setView(view);
		window.display();
	}

	return 0;
}

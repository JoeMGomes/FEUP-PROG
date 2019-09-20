/*
FILE : cwplayer.cpp
DATE : 2018/05/12
AUTHORS : José Gomes e Liliana Almeida
PROGRAM PURPOSE:
- Solve a previously created crosswords puzzle stored in a text file;
- Ask for positions and words so the user can try to solve the puzzle;
- The verification of the answers is after ther board is full, giving the chance to make it rigth if the answers are not correct
*/

#include <iostream>
#include "Player.h"
#include "Board.h"
#include "Dictionary.h"
#include <string>
#include <ctime>

using namespace std;

/* reads the position and the word the user wants to put on the table and insert the word if possible;
 * also checks if the board if full and if it was correctly solved ending the game if it's true
* @param	puzzle - object of board class
* @param	dic - object of dict class
* @param	player - object of player class
*/
void putWordsInBoard(Board puzzle, Dict dic, Player player);

int main() {
	srand((unsigned int)time(NULL));

	cout << "CROSSWORDS PUZZLE GAME\n=============================================================\n\n";
	cout << "INSTRUCTIONS:\n"
		<< " To begin playing, type in the name of the text file that you want\n"
		<< "to select. Then choose the name you wish your time and statistics\n"
		<< "to be saved as (choose a single word, no spaces).\n"
		<< " You will then be presented with an empty board and clues for the words.\n"
		<< " The dots represent a cell in which a character will be placed. To\n"
		<< "place a word on the board just type in its starting position (LCD -\n"
		<< "- Line Column Direction) and then the word.\n"
		<< " You can ask for additional clues by typing '?' but this will be\n"
		<< "recorded on your stats.";

	string boardFile, dictionaryFile, namePlayer; //board file, dictionary file and player's names

	cout << "\n-------------------------------------------------------------\n"
		<< "PLAY THE GAME\n"
		<< "-------------------------------------------------------------\n\n";

	//OPENING DICTIONARY FILE
	cout << "Board File Name? ";
	cin >> boardFile;

	//////////////////////////////////////////////////////////
	///CHECK INVALID INPUT///

	string buffer; //what is left in the buffer
	getline(cin, buffer); //reads what is left in the buffer after reading the board file's name
	while (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
		cout << "Invalid input, please try again!\n\n";			     //input is invalid

		cout << "Board File Name? ";   //reads again the board file's name
		cin >> boardFile;
		buffer.clear(); //clears buffer string
		getline(cin, buffer);
	}
	//////////////////////////////////////////////////////////

	//LOAD THE BOARD AND OPEN THE SYNONYMS FILE WITH THE FILE'S NAME READ FROM THE TEXT FILE
	Board puzzle;
	dictionaryFile = puzzle.load(boardFile);

	Dict dic(dictionaryFile);
	dic.openFile();


	vector <pair <string, string> > usedPosWords; //vector that stores the starting position of each word and the word itslef in pairs(Pos, word)
	puzzle.getUsedWords(usedPosWords);

	//set the synonyms of each word to be guessed by the user and the clues to be shown to the user
	dic.setSynonsBoard(usedPosWords);
	dic.setClues();

	//READS PLAYERS' NAME
	cout << "Player Name? ";
	cin >> namePlayer;

	//////////////////////////////////////////////////////////
	///CHECK INVALID INPUT///

	getline(cin, buffer); //reads what is left in the buffer after reading the player's name
	while (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
		cout << "Invalid input, please try again!\n\n";

		cout << "Player Name? "; //reads again the player's name
		cin >> namePlayer;
		buffer.clear(); //clears buffer string
		getline(cin, buffer);
	}
	//////////////////////////////////////////////////////////

	Player player(namePlayer);

	cout << "\n-------------------------------------------------------------\n"
		<< "START OF THE GAME\n"
		<< "-------------------------------------------------------------\n\n";

	///BUILDS THE GRID AND SHOWS IT FOLLOWED BY THE CLUES///
	puzzle.prepareVector();
	puzzle.showPlayerBoard();
	dic.showClues();

	player.startTime();//starts counting the time

	putWordsInBoard(puzzle, dic, player);

	return 0;
}

//============================================//
//============================================//
//============================================//
//============================================//

void putWordsInBoard(Board puzzle, Dict dic, Player player) {
	string word;  //word inserted by the user
	char l, c, dir; //line(Y), column(X), direction

	cout << "\nPosition ('LCD')  . .  ? ";
	cin >> l >> c >> dir;


	//////////////////////////////////////////////////////////////
	///INPUT VALIDATION FOR THE POSITION///

	if (cin.fail()) {
		cin.clear();
		cin.ignore(100000000, '\n');
		cout << "Invalid input, please write again!\n";
		putWordsInBoard(puzzle, dic, player);
	}

	string buffer; //reads what is left in the buffer
	getline(cin, buffer);  //reads what is left in the buffer after reading the position
	if (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
		cout << "Invalid input, please write again!\n";
		putWordsInBoard(puzzle, dic, player);
	}
	//////////////////////////////////////////////////////////////

	///VERIFY POSITION INSERTED///
	if (dir == 'V' || dir == 'v')
		dir = 'V';
	else if (dir == 'H' || dir == 'h')
		dir = 'H';
	else {
		cout << "That direction does not exist, please enter 'V' or 'H'.\n";
		putWordsInBoard(puzzle, dic, player);
	}

	//CHECK IF IN THE POSITION GIVEN, STARTS ONE OF THE WORDS THE USER HAS TO GUESS
	if (!puzzle.existsPosition(l, c, dir)) {
		cout << "That is not a position of a word. Try again!\n";
		putWordsInBoard(puzzle, dic, player);

	}

	//CHECK IF THE COORDINATES AND THE DIRECTION WERE NOT VALID
	if (!puzzle.setPos(c, l, dir)) {
		putWordsInBoard(puzzle, dic, player);
	}



	//WORD INPUT
	cout << "Word (- = remove/ ? = help) ? ";
	cin >> word;
	cout << endl;


	//////////////////////////////////////////////////////////////
	///INPUT VALIDATION FOR THE WORD///

	if (cin.fail()) {
		cin.clear();
		cin.ignore(100000000, '\n');
		cout << "\nInvalid input, please choose again!\n\n";
		putWordsInBoard(puzzle, dic, player);
	}

	getline(cin, buffer); //reads what is left in the buffer after reading the word
	if (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
		cout << "Invalid input, please write again!\n\n";
		putWordsInBoard(puzzle, dic, player);

	}
	//////////////////////////////////////////////////////////////


	dic.lowToUpperCase(&word);

	//////// HELP ////////

	if (word == "?") {
		string altClue; //alternative clue which the user asked 
		string pos; //position given by the user
		pos = puzzle.stringPos();

		dic.getAlternativeClue(pos, altClue);

		if (altClue.empty()) { //if altClue is empty there are no more clues to be shown to the user
			cout << "There are no more clues for this word. Try to guess it!\n\n";
			putWordsInBoard(puzzle, dic, player);
		}
		else { //show the alternative clue and increase the numer of clues given to the user
			cout << "Another clue for the coordinates " << pos << ": " << altClue << ". Do you know which word it's now?\n\n";
			player.addNumberClues();
		}
		putWordsInBoard(puzzle, dic, player);
	}

	////////ERASE WORD////////
	else if (word == "-") {
		puzzle.erasePlayerBoard();
		puzzle.updatePlayerBoard();
		puzzle.showPlayerBoard();
		putWordsInBoard(puzzle, dic, player);
	}


	//////////////////////////
	///VERIFY WORD INSERTED///
	//////////////////////////

	////CHECK IF WORD'S CHARACTERS ARE JUST LETTERS/////
	else if (word.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1) { //if the word doesn't contain only uppercase letters('A' - 'Z') it is not a valid Input
		cout << "Invalid Word. Try another one!\n\n";
		putWordsInBoard(puzzle, dic, player);
	}

	///CHECK IF THE WORD FITS IN THE BOARD IN THE POSITION CHOSEN///
	else if (!puzzle.checkLengthWord(word)) {
		cout << "The word doesn't fit in the board. Try another one!\n\n";
		putWordsInBoard(puzzle, dic, player);
	}

	////REPLACE WORD IN BOARD IF THERE IS ALREADY ONE IN THE POSITION GIVEN////
	else if (puzzle.placed() != " ") {	//if there is a word in the position given by the user
		string previousWord = puzzle.placed(); //word that was already in the board
		puzzle.erasePlayerBoard(); //erase that word just to see if the new word can be placed there
		puzzle.updatePlayerBoard();
		puzzle.setPos(c, l, dir);

		if (!dic.fits(word, puzzle.sendLine())) { //if the new word doesn't fit the other words of the board except the one it can replace
			cout << "The word doensn't fit in with the other words of the board. Try another one!\n\n";
			puzzle.insert(previousWord); //places the word that was already in the board
			puzzle.updatePlayerBoard();
			putWordsInBoard(puzzle, dic, player);
		}
		else { //if the new word fits
			puzzle.insert(word); //places the new word in the position given by the user
			puzzle.updatePlayerBoard();
		}
	}

	///CHECK IF THE WORD DOESN'T CHANGE OTHER WORDS THAT ARE ALREADY IN THE BOARD///
	else if (!dic.fits(word, puzzle.sendLine())) {
		cout << "The word doensn't fit in with the other words of the board. Try another one!\n\n";
		putWordsInBoard(puzzle, dic, player);

	}
	////////////

	///IF INPUT IS VALID...///
	else {
		cout << endl << endl;
		puzzle.insert(word);
		puzzle.updatePlayerBoard();
	}


	//CHECK IF THE BOARD IS FULL
	if (!puzzle.fullBoard()) {
		puzzle.showPlayerBoard();
		dic.showClues();
	}
	else
		if (!puzzle.finalValid()) { //does a final  validation to check if the puzzle was correctly solved
			puzzle.showPlayerBoard();
			dic.showClues();
			cout << "The words are not all correct.. Try to find to make it rigth!\n\n";
			putWordsInBoard(puzzle, dic, player);

		}

	//////END OF THE GAME /////
		else {
			cout << "-------------------------------------------------------------\n"
				<< "END OF THE GAME\n"
				<< "-------------------------------------------------------------\n\n";

			//SHOW COMPLETE BOARD AND THE CLUES
			puzzle.showPlayerBoard();
			dic.showClues();

			//FINNISH GAME AND SAVE PLAYER'S INFORMATION
			cout << "\n\nAll the words are correct. Well Done " << player.getNamePlayer() << "!\n";
			player.stopTime();//stop counting time
			cout << "It took you " << player.getTime() << " to finish the puzzle and you got " << player.getNumberClues() << " alternative clues to help you solve it." << endl;
			player.savePlayerInfo(puzzle.getBoardFile()); //save player's information
			exit(0);
		}
		putWordsInBoard(puzzle, dic, player);
}


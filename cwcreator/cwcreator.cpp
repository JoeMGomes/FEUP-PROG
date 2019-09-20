/*
FILE : cwcreator.cpp
DATE : 2018/05/09
AUTHORS : José Gomes e Liliana Almeida
PROGRAM PURPOSE:
- Create a crosswords puzzle
- Read the choices given to the user and calls the respective functions for each one;
- Read the position and the word and checks if it is possible to write it on the board;
- If the user wants to stop, he can save or finish the puzzle;
- Check if the board is full, and if that is true gives the user the a chance to change the board or to save and finnish.
*/


#include <iostream>
#include "Board.h"
#include "Dictionary.h"
#include <string>
#include <ctime>

/* gives and reads if the user wants to create, to reload a puzzle or to exit */
void chooseOption();

/* reads the dictionary's name and open it; reads the size of the board and builds an empty one */
void createPuzzle();

/* reads the board file's name and loads everything showing how the board was left, setting the size of the board */
void resumePuzzle();

/* reads the position and the word the user wants to put on the table (also '?' -> help , Ctrl-Z -> stop and '-' -> erase)
 * @param	puzzle - object of board class
 * @param	dic - object of dict class
*/
void putWordsInBoard(Board puzzle, Dict dic);

/* reads if the user wants to finish or just save the board and executes what the user chooses
 * @param	puzzle - object of board class
 * @param	dic - object of dict class
*/
void save_finish(Board puzzle, Dict dic);

/* reads if the user wants to make changes in the board, save and exit or exit when the board is full
* @param	puzzle - object of board class
* @param	dic - object of dict class
*/
void fullBoardMenu(Board puzzle, Dict dic);

/* makes the final validation and saves the board
* @param	puzzle - object of board class
* @param	dic - object of dict class
*/
void finalValidation(Board puzzle, Dict dic);


//============================================//
//============================================//
//============================================//

using namespace std;

int main() {
	srand((unsigned int)time(NULL)); //initialize random number generator 

	//START MENU
	cout << "CROSSWORDS PUZZLE CREATOR\n=============================================================\n\n";

	cout << "INSTRUCTIONS:\n"
		<< " Enter de board size with a maximum value of 26 for both\n"
		<< "lines and columns.\n"
		<< " When asked for a position, insert the position on which\n"
		<< "you want the word to start  followed by its orientation.\n"
		<< " Your input should be LCD (Line Column Direction).\n Example: AaV\n"
		<< " Then insert the word you want to be used on the board.\n"
		<< " Example: Carrot\n\n"
		<< "-------------------------------------------------------------\n\n";

	cout << "OPTIONS:\n1 - Create puzzle\n2 - Resume puzzle\n0 - Exit\n\n";
	chooseOption();
	return 0;
}

//============================================//
//============================================//

void chooseOption() {
	string option; //option chosen by the user

	cout << "Option? ";
	cin >> option;


	//////////////////////////////////////////////////////////
	///CHECK INVALID INPUT///
	if (cin.fail()) {
		cin.clear();
		cin.ignore(100000000, '\n');
		cout << "Invalid input, please choose again!\n\n";
		chooseOption();
	}

	string buffer; //what is left in the buffer
	getline(cin, buffer); //reads what is left in the buffer after reading the option
	if (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //if buffer is not empty and doens't have only space characters
		cout << "Invalid input, please choose again!\n\n";		  //input is invalid
		chooseOption();
	}
	/////////////////////////////////////////////////////////////


	if (option == "1")
		createPuzzle();
	else
		if (option == "2")
			resumePuzzle();
		else
			if (option == "0") {
				cout << "\nExiting program...\n";
				exit(0);
			}
			else {
				cout << "Invalid input, please choose again!\n\n";
				chooseOption();
			}
}

//============================================//
//============================================//

void createPuzzle() {

	int xBoard, yBoard; //size of the board 
	string dictionaryFile; //name of the dictionary file

	cout << "\n-------------------------------------------------------------\n"
		<< "CREATE PUZZLE\n"
		<< "-------------------------------------------------------------\n\n";

	//OPENING DICTIONARY FILE
	cout << "Dictionary File Name? ";
	cin >> dictionaryFile; //reads dictionary file's name


	//////////////////////////////////////////////////////////
	///CHECK INVALID INPUT///

	string buffer; //what is left in the buffer
	getline(cin, buffer); //reads what is left in the buffer after reading the dictionary file's name
	while (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
		cout << "Invalid input, please write again!\n\n";		     //input is invalid

		cout << "Dictionary File Name? "; //reads again the dictionary file's name
		cin >> dictionaryFile;
		buffer.clear(); //clears buffer string
		getline(cin, buffer);
	}
	//////////////////////////////////////////////////////////


	Dict dic(dictionaryFile);
	dic.openFile();

	//BUILDING BOARD
	cout << "Board size (line columns)? ";
	cin >> yBoard >> xBoard;


	//////////////////////////////////////////////////////////
	///CHECK INVALID INPUT FOR THE BOARD'S SIZE///

	getline(cin, buffer); //reads what is left in the buffer after reading the size of the board
	while (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
		cout << "Invalid input, please write again!\n\n";		     //input is invalid
		cout << "Board size (line columns)? ";	//reads again the size of the board
		cin >> yBoard >> xBoard;
		buffer.clear(); //clears buffer string
		getline(cin, buffer);
	}

	while (cin.fail() || (xBoard < 1 || xBoard > 26 || yBoard < 1 || yBoard > 26)) {
		if (cin.fail()) {
			cin.clear();
			cin.ignore(100000000, '\n');
			cout << "Invalid input, please write again!\n\n" << "Board size (line columns)? ";
			cin >> xBoard >> yBoard;

			getline(cin, buffer);
			while (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
				cout << "Invalid input, please write again!\n\n";
				cout << "Board size (line columns)? ";
				cin >> yBoard >> xBoard;
				buffer.clear(); //clears buffer string
				getline(cin, buffer);
			}
		}

		else {
			cout << "Invalid board size. Write values between 1 and 26 for lines and columns!\n\n" << "Board size (line columns)? ";
			cin >> xBoard >> yBoard;

			getline(cin, buffer); //reads what is left in the buffer after reading the size of the board
			while (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
				cout << "Invalid input, please write again!\n\n";		         //input is invalid
				cout << "Board size (line columns)? "; //reads again the size of the board
				cin >> yBoard >> xBoard;
				buffer.clear(); //clears buffer string
				getline(cin, buffer);
			}
		}
	}
	/////////////////////////////////////////////////////


	cout << endl;

	Board puzzle;
	puzzle.build(xBoard, yBoard);
	puzzle.show();

	//START ASKING FOR POSITIONS AND WORDS
	putWordsInBoard(puzzle, dic);
}

//============================================//
//============================================//

void resumePuzzle() {

	cout << "\n-------------------------------------------------------------\n"
		<< "RESUME PUZZLE\n"
		<< "-------------------------------------------------------------\n\n";

	string boardFile, dictFile; //board file's name; dictionary file's name
	cout << "Board File Name? ";
	cin >> boardFile;

	//////////////////////////////////////////////////////////
	///CHECK INVALID INPUT///

	string buffer; //reads what is left in the buffer
	getline(cin, buffer);  //reads what is left in the buffer after reading the board file's name
	while (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
		cout << "Invalid input, please write again!\n\n";
		cout << "Board File Name? ";
		cin >> boardFile;
		buffer.clear(); //clears buffer string
		getline(cin, buffer);
	}
	/////////////////////////////////////////////////////////

	cout << endl;

	//LOAD THE BOARD AND OPEN THE SYNONYMS FILE WITH THE FILE'S NAME READ FROM THE TEXT FILE
	Board puzzle;
	dictFile = puzzle.load(boardFile);

	Dict dic(dictFile);
	dic.openFile();


	puzzle.show();

	if (puzzle.fullBoard()) {
		cout << "\n-------------------------------------------------------------\n"
			<< "THE BOARD IS FULL\n"
			<< "-------------------------------------------------------------\n\n";

		cout << "Choose what you want to do now:\n"
			<< "1 - Make changes in this board\n2 - Save and Exit\n0 - Exit\n\n";

		fullBoardMenu(puzzle, dic);
	}

	//START ASKING FOR POSITIONS AND WORDS
	putWordsInBoard(puzzle, dic);
}

//============================================//
//============================================//

void putWordsInBoard(Board puzzle, Dict dic) {

	string word; //word inserted by the user
	char l, c, dir; //line(Y), column(X), direction

	cout << "\nPosition('LCD' / Ctrl - Z = stop) ? ";
	cin >> l >> c >> dir;


	//////////////////////////////////////////////////////////////
	///INPUT VALIDATION FOR THE POSITION///

	if (cin.fail()) {
		if (cin.eof()) { //if user writes Ctrl-Z
			cin.clear(); //clear eof flag

			cout << "\n-------------------------------------------------------------\n"
				<< "SAVE OR FINNISH PUZZLE\n"
				<< "-------------------------------------------------------------\n\n";

			cout << "Choose what you want to do now:\n"
				<< "1 - Save the board and resume later\n2 - Finish the board\n\n";
			save_finish(puzzle, dic); //the user chooses whether he wants to save or finish the board
		}
		else {
			cin.clear();
			cin.ignore(100000000, '\n');
			cout << "Invalid input, please write again!\n";
			putWordsInBoard(puzzle, dic);
		}
	}

	string buffer; //reads what is left in the buffer
	getline(cin, buffer); //reads what is left in the buffer after reading the position
	if (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
		cout << "Invalid input, please write again!\n";
		putWordsInBoard(puzzle, dic);
	}
	//////////////////////////////////////////////////////////////

	///VERIFY POSITION INSERTED///
	if (dir == 'V' || dir == 'v')
		dir = 'V';
	else if (dir == 'H' || dir == 'h')
		dir = 'H';
	else {
		cout << "That direction does not exist, please enter 'V' or 'H'.\n";
		putWordsInBoard(puzzle, dic);
	}
	if (!puzzle.setPos(c, l, dir)) //if the coordinates and the direction were not valid
		putWordsInBoard(puzzle, dic);


	//WORD INPUT
	cout << "Word ( - = remove/ ? = help ) . .  ? ";
	cin >> word;
	cout << endl;


	//////////////////////////////////////////////////////////////
	///INPUT VALIDATION FOR THE WORD///

	if (cin.fail()) {
		cin.clear();
		cin.ignore(100000000, '\n');
		cout << "\nInvalid input, please choose again!\n\n";
		putWordsInBoard(puzzle, dic);
	}

	getline(cin, buffer); //reads what is left in the buffer after reading the word
	if (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
		cout << "Invalid input, please write again!\n\n";
		putWordsInBoard(puzzle, dic);
	}
	//////////////////////////////////////////////////////////////


	dic.lowToUpperCase(&word);

	//////// HELP ////////
	string wildWord; //line/column starting in the position given by the user, where the dots were replaced by the wildcard '*' and '?'
	vector <string> helpList; //vector with the possible words for that position and direction

	if (word == "?") {
		wildWord = puzzle.help();

		if (wildWord.empty()) { //if the wildWord is empty it's not possible to put any word of the dictionary in the position and direction given
			cout << "It is not possible to put any words in that position and direction. Try another position/direction!\n\n";
			putWordsInBoard(puzzle, dic);
		}

		dic.incWords(wildWord, helpList); //returns the possible words in helpList vector

		for (size_t i = 0; i < helpList.size(); i++) { //checks if there is any word in the helpList that is already in the board
			if (puzzle.wordExists(helpList.at(i)))	   //if it exists
				helpList.erase(helpList.begin() + i);  //erase it from the list
		}

		if (helpList.empty()) {
			cout << "There is no word in the dictionary that can be put in that position/direction. Try again!\n\n";
			putWordsInBoard(puzzle, dic);
		}

		//SHOW THE LIST OF POSSIBLE WORDS TO THE USER
		cout << "List of possible words: \n";
		for (size_t i = 0; i < helpList.size() && i < 10; i++)
			cout << helpList[i] << endl;
		cout << endl;
	}

	////////ERASE WORD////////
	else if (word == "-") {
		puzzle.erase();
		puzzle.update();
		puzzle.show();
		putWordsInBoard(puzzle, dic);
	}


	//////////////////////////
	///VERIFY WORD INSERTED///
	//////////////////////////

	////CHECK IF WORD'S CHARACTERS ARE JUST LETTERS/////
	else if (word.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1) { //if the word doesn't contain only uppercase letters('A' - 'Z') it is not a valid Input
		cout << "Invalid Word. Try another one!\n\n";
		putWordsInBoard(puzzle, dic);
	}

	///CHECK IF THE WORD FITS IN THE BOARD///
	else if (!puzzle.valid(word)) {
		cout << "The word doesn't fit in the board. Try another one!\n\n";
		putWordsInBoard(puzzle, dic);
	}

	///CHECK IF THE WORD IS IN THE DICTIONARY///
	else if (!dic.valid(word)) {
		cout << "The word doesn't belong to the dictionary. Try another one!\n\n";
		putWordsInBoard(puzzle, dic);
	}

	///CHECK IF THE WORD DOESN'T CHANGE OTHER WORDS THAT ARE ALREADY IN THE BOARD///
	else if (!dic.fits(word, puzzle.sendLine())) {
		cout << "The word doensn't fit in with the other words of the board. Try another one!\n\n";
		putWordsInBoard(puzzle, dic);
	}

	///CHECKS IF THE WORD WAS ALREADY PUT ON THE BOARD///
	else if (puzzle.wordExists(word)) {
		cout << "The word has already been used. Try another one!\n\n";
		putWordsInBoard(puzzle, dic);
	}
	////////////

	///IF INPUT IS VALID...///
	else {
		puzzle.storeWord(word);
		puzzle.update();
		puzzle.show();
	}

	///CHECK IF BOARD IS FULL
	if (puzzle.fullBoard()) {
		cout << "\n-------------------------------------------------------------\n"
			<< "THE BOARD IS FULL\n"
			<< "-------------------------------------------------------------\n\n";

		cout << "Choose what you want to do now:\n"
			<< "1 - Make changes in this board\n2 - Save and Exit\n0 - Exit\n\n";

		fullBoardMenu(puzzle, dic);
	}
	else
		putWordsInBoard(puzzle, dic);

}

//============================================//
//============================================//

void save_finish(Board puzzle, Dict dic) {
	string option; //option chosen by the user
	cout << "Option? ";
	cin >> option;


	//////////////////////////////////////////////////////////
	///CHECK INVALID INPUT///

	if (cin.fail()) {
		cin.clear();
		cin.ignore(100000000, '\n');
		cout << "Invalid input, please choose again!\n\n";
		save_finish(puzzle, dic);
	}

	string buffer; //reads what is left in the buffer
	getline(cin, buffer); //reads what is left in the buffer after reading the option
	if (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
		cout << "Invalid input, please write again!\n\n";
		save_finish(puzzle, dic);
	}
	////////////////////////////////////////////////////////////


	//SAVE BOARD
	if (option == "1") {
		puzzle.save(dic.getSynonFile());
		exit(0);
	}

	//FINISH BOARD
	else if (option == "2") {
		puzzle.finishBoard(); //complete board with '#'
		puzzle.show();
		cout << endl;
		finalValidation(puzzle, dic);
		exit(0);
	}

	//Invalid input
	else {
		cout << "Invalid input, please choose again!\n\n";
		save_finish(puzzle, dic);
	}
}

//============================================//
//============================================//

void fullBoardMenu(Board puzzle, Dict dic) {

	string option; //option chosen by the user
	cout << "Option? ";
	cin >> option;


	//////////////////////////////////////////////////////////
	///CHECK INVALID INPUT///

	if (cin.fail()) {
		cin.clear();
		cin.ignore(100000000, '\n');
		cout << "Invalid input, please choose again!\n\n";
		fullBoardMenu(puzzle, dic);
	}

	string buffer; //reads what is left in the buffer
	getline(cin, buffer); //reads what is left in the buffer after reading the option
	if (!buffer.empty() && buffer.find_first_not_of(" ") != -1) { //while buffer is not empty and doens't have only space characters
		cout << "Invalid input, please write again!\n\n";
		fullBoardMenu(puzzle, dic);
	}
	//////////////////////////////////////////////////////////


	//CHANGE THE BOARD
	if (option == "1") {
		cout << "\n\n-------------------------------------------------------------\n"
			<< "CONTINUE BOARD\n"
			<< "-------------------------------------------------------------\n\n";
		puzzle.update(); //removes the hashtags that are not needed if those exist, so that the user can see where he can put more words if he desires to do it
		puzzle.show();
		return;
	}

	//SAVE AND EXIT
	else if (option == "2") {
		finalValidation(puzzle, dic);
		exit(0);
	}

	//EXIT
	else if (option == "0") {
		cout << "\nExiting program...\n";
		exit(0);
	}

	//Invalid input
	else {
		cout << "Invalid input, please choose again!\n\n";
		fullBoardMenu(puzzle, dic);
	}
	finalValidation(puzzle, dic);
}

//============================================//
//============================================//

void finalValidation(Board puzzle, Dict dic) {
	if (puzzle.finalValid()) {
		cout << "The final validation was successful!\n";
		puzzle.sortUsedWords(); //sort the vector with the used Words
		puzzle.save(dic.getSynonFile()); //saves the board and the synonyms' file
	}
	else {
		cout << "The board is not valid. There is at least one word that changed. Please try to fix that mistake or start a new game!\n\n";
		putWordsInBoard(puzzle, dic);
	}
}

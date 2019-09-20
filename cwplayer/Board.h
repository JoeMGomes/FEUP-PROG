#ifndef board_H
#define board_H
#include<iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

class Board {

public:
	/* Sets the size of the board and resizes the vector contaning the coordinates accordingly. Changes the sizeX, sizeY and coords variables.
	 * @param x - horizontal size of the board
	 * @param y - vertical size of the board
	*/
	void build(int x, int y);

	/* Displays the board on screen with the cooresponding colors. Doesn't change any variables. */
	void show();

	/* Changes posX, posY and direction. These are meant to be the startin cell of a word.
	 * @param	x - The X position of the starting cell
	 * @param	y - The Y position of the starting cell
	 * @param	direction - The direction that will be taken in account
	 * @return	true if the positions and coordinates are valid or false if they are invalid
	*/
	bool setPos(char x, char y, char dir);

	/* Checks if a word fits on the board taking in account the starting position and direction,
	the word lenght and the size of the board. Ignores if there are words on the board already
	 * @param	word - The word to testfit on the board
	 * @return	true if the positions and coordinates are valid or false if they are invalid
	*/
	bool valid(string word);

	/* Accesses usedWords vector and searches for a word
	 * @param	word - word to be searched for
	 * @return	true if the word is already on the board and false if it is not
	*/
	bool wordExists(string word);

	/* For every word on usedWords vector replaces the respective coordinates on the coords vector. Prior to that it checks if
	the words boundaries are safe. This means the # placed before and after the word will not interfere with already placed words
	*/
	void update();

	/* Stores a word on the usedWords vector along with the position set with setPos()
	* @param	word - word to be stored
	*/
	void storeWord(string word);

	/* Forms a word with the letters that are in that line/column and
	wildcards characters instead of dots starting from the given position
	 * @return word with wildcards characters
	*/
	string help();

	/* gets the line starting on the position given by the user from the vector with the coordinates of the board
	* @return	line starting on the setPos
	*/
	string sendLine();

	/* Searches the usedWords vector for the position set with setPos() and
	removes that index on the vector if it exists
	*/
	void erase();

	/* Combines posY,posX and direction(by this order) into a string
	* @return	string with the combines positions and direction
	*/
	string stringPos();

	/* Prints into a .txt a savefile containing the name of the dictionary file used, the board itself and the used words and respective starting positions
	 * @param	dictFile - The name of the dicitonary file used
	*/
	void save(string dictFile);

	/* Reads from a .txt file a complete os incomplete board and changes the variables acordingly
	 * @param	bFile - Save file where the dictionary file name, the board and words are stored
	 * @return	the name of the dictionary file read from the first line of board file
	*/
	string load(string bFile);

	/* Checks if the board is full by searching for '.' in the coords vector
	 * @return	true if there are no '.' and false if there are
	*/
	bool fullBoard();

	/* Compares the words on the board and the words saved in the vector
	which should be the same (in the respective position and direction)
	*/
	bool finalValid();

	/* Finishes the board locking all available cells with # */
	void finishBoard();

	/* Sorts the usedWords vector so that the horizontals words come first and
	then the words are placed by the order in which they appear on the board
	*/
	void sortUsedWords();


	//////////////////////////////////////////////////////////////////////////////////
	////////////////////////METHODS USED TO PLAY THE GAME ONLY////////////////////////
	//////////////////////////////////////////////////////////////////////////////////

	/* Updates the coords vector in order to be played. The alphabetic characters are changed to a '.' */
	void prepareVector();

	/* Displays the playable board on screen with the cooresponding colors. Doesn't change any variables */
	void showPlayerBoard();

	/* Checks if that position is the begining of a word by comparing said position with guessedWords's first element of each pair
	 * @param	line - First coordinate
	 * @param	column - Second Coordinate
	 * @param	dir - Direction
	 * @return	true if the position was already used or false if not
	*/
	bool existsPosition(char line, char column, char dir);

	/* Verefies if the lenght of the word the user wants to use is the same size as the word on usedWords vector that match the position
	 * @param	word - word to check the lenght
	 * @return	true if the word lenght matches or false if it does not
	*/
	bool checkLengthWord(string word);

	/* Places a word on the vector along with its starting position and direction set with setPos()
	* @param	word - Word to be placed on the vector
	*/
	void insert(string word);

	/* For every word on guessedWords will update the coords vector accordingly */
	void updatePlayerBoard();

	/* Searches the guessedWords vector for the position set with setPos() and
	removes that index on the vector if it exists
	*/
	void erasePlayerBoard();

	/*Allows access to the private vector "usedWords";
	 * @param	usedPosWords - Vector to which usedWords will be copied
	*/
	void getUsedWords(vector <pair <string, string> > &usedPosWords);

	/* Checks if the user already placed a word on said position
	 * @return	the placed word if found or returns " " if not found
	*/
	string placed();

	/* Use to get the Board .txt name
	* @return	Board .txt name
	*/
	string getBoardFile();


private:
	/* Converts the letters used on setPos() to numbers in order to be used on the coords vector
	These numbers are stores on the variables X and Y. For example:
	*/
	void posToCoord();

	/* set the variables with the size of the board when the board is loaded from a text file */
	void setSize();


	//size measured horizontally, size measured vertically of the board
	unsigned int sizeX, sizeY;

	//column(posX) and line(posY) (given in letters) and the direction (horizontal or vertical) given by the user
	char posX, posY, direction;

	//index of the position given (column, line) in the board
	int X, Y;

	//Name of the .txt on which the board will be saved
	string boardFile;

	//stores every char of every word in the correct [y][x] position
	vector <vector <char> > coords;

	//stores the starting position of each word and the word itslef in pairs (Pos, word)
	vector<pair<string, string>> usedWords;


	//////////////////////////////////////////////////////////////////////////
	///////////////////////USED TO PLAY THE GAME ONLY/////////////////////////
	//////////////////////////////////////////////////////////////////////////

	//stores the starting position of each word that the user has put on the board and the word itslef in pairs (Pos, word)
	vector<pair<string, string>> guessedWords;
};

#endif
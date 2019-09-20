/*
FILE : Board.cpp
DATE : 2018/05/04
AUTHORS : José Gomes e Liliana Almeida
PROGRAM PURPOSE:
- build, show and update the board;
- check if the word fits in the board and if it already exists;
- set position each time the user enters a valid one;
- insert new words in the board and its coordinates;
- erase a previously placed word;
- create a word with wildcard characters when the user asks for help;
- set size of the board when it's loaded
- save/load the board in/from a text file;
- complete the board with hashtags;
- check if the board is full.
*/

#include "Board.h"
#include <windows.h>
#include <fstream>
#include <sstream>
#include <algorithm>

/* Sets the color with which the console will print
 * @param	k - background and foreground color depend only on k and it goes from 1-254
*/
void setcolor(unsigned int k);

/* Automatically uses setcolor() based on the character given(E.g. '#' is painted white in a black background)
 * @param	a - char to set the color based on
*/
void paint(char a);

/* Knowing the number for the file's name, adds zeros to the name so it will have 3 digits in the end
 * @param	boardFile - file's name where the board will be saved
 * @param	num - number of the board (without zeros before the number)
*/
void zerosNameFile(string *boardFile, int num);



//============================================//
//============================================//
//============================================//

void Board::build(int x, int y) {
	sizeX = x;
	sizeY = y;

	coords.resize(sizeY, vector <char>(sizeX, '.'));
}

//============================================//
//============================================//

void Board::show() {

	setcolor(12);
	cout << "   ";
	for (unsigned int i = 0; i < coords[0].size(); i++) //prints the first line of red lower case letters to indicate the x coordinate
		cout << " " << char(97 + i);
	cout << endl;

	for (unsigned int x = 0; x < coords.size(); x++) { //prints each line 
		setcolor(12);
		cout << " " << char(65 + x) << " "; //prints the red capital letter that indicates the Y coordinate
		for (unsigned int y = 0; y < coords[x].size(); y++) { //Prints the characters of coords vector with a " " in between while painting every character
			setcolor(112);									  //with the right color
			cout << " ";
			paint(coords[x][y]);
			cout << coords[x][y];
		}
		setcolor(112);
		cout << " ";
		cout << endl;
		setcolor(15);
	}
	setcolor(7);
}

//============================================//
//============================================//

void paint(char a) {

	if (isalpha(a) || a == ' ' || a == '.') { //all alphabetic characters, spaces and '.' are painted black in a white background
		setcolor(112);
	}
	else if (a == '#') //All '#' are painted white in a black background
		setcolor(7);
}

//============================================//

void setcolor(unsigned int k) {
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, k);
}

//============================================//
//============================================//

bool Board::setPos(char x, char y, char dir) {
	posX = x;
	posY = y;
	direction = dir;

	posToCoord();
	if (!(posY >= 65 && posY <= 90 && posX >= 97 && posX <= 122)) { //forces posY to be in range of A-Z and posXto be in range of a-z
		cout << "The coordinates given are not valid, please enter the line in uppercase and the column in lowercase in this order.\n";
		return false;
	}

	if ((unsigned int)X > sizeX || (unsigned int)Y > sizeY) { //if the player tries to access cells that do not exist returns false
		cout << "The board is not that big. Please try again!\n";
		return false;
	}

	if (direction == 'V' || direction == 'v') //converts lowercase direction input to uppercase or returns false if the input is invalid
		direction = 'V';
	else if (direction == 'H' || direction == 'h')
		direction = 'H';
	else {
		cout << "That direction does not exist, please enter 'V' or 'H'.\n";
		return false;
	}
	posToCoord();
	return true;
}

//============================================//
//============================================//

void Board::posToCoord() {
	//converts the letters input to number to be used on the coords vector indexes
	Y = posY - 65;
	X = posX - 97;
}

//============================================//
//============================================//

bool Board::valid(string word) {

	if (direction == 'V') {
		if (Y + word.size() <= sizeY) {
			return true;
		}
		else return false;
	}

	else if (direction == 'H') {
		if (X + word.size() <= sizeX) {
			return true;
		}
		else return false;
	}

	return false;
}

//============================================//
//============================================//

bool Board::wordExists(string word) {
	for (size_t i = 0; i < usedWords.size(); i++) {
		if (word == usedWords.at(i).second)
			return true;
	}
	return false;
}

//============================================//
//============================================//

void Board::update() {

	coords.resize(0, vector <char>(0, '.')); //clears the vector
	coords.resize(sizeY, vector <char>(sizeX, '.'));

	for (size_t j = 0; j < usedWords.size(); j++) { //for every word on usedWords updates coords vector accordingly
		posY = usedWords[j].first.at(0);
		posX = usedWords[j].first.at(1);
		direction = usedWords[j].first.at(2);
		posToCoord();
		string word = usedWords[j].second;  //word of usedWords

		if (direction == 'V') {
			if (Y > 0) {
				if (!isalpha(coords[Y - 1][X])) //prevents word from beeing placed so close that the '#' overlaps the previous word
					coords[Y - 1][X] = '#';
				else {
					setPos(posX, posY, direction);
					erase();
					cout << "You can't place words adjacent that way\n\n";
					update();
					break;
				}
			}

			for (size_t i = 0; i < word.size(); i++)
				coords[Y + i][X] = word.at(i);

			if (Y + word.size() < sizeY) {
				if (!isalpha(coords[Y + word.size()][X])) //prevents word from beeing placed so close that the '#' overlaps the previous word
					coords[Y + word.size()][X] = '#';
				else {
					setPos(posX, posY, direction);
					erase();
					cout << "You can't place words that way\n\n";
					update();
					break;
				}
			}
		}
		if (direction == 'H') {
			if (X > 0) {
				if (!isalpha(coords[Y][X - 1])) //prevents word from beeing placed so close that the '#' overlaps the previous word
					coords[Y][X - 1] = '#';
				else {
					setPos(posX, posY, direction);
					erase();
					cout << "You can't place words that way\n\n";
					update();
					break;
				}
			}

			for (size_t i = 0; i < word.size(); i++)
				coords[Y][X + i] = word.at(i);

			if (X + word.size() < sizeX) {
				if (!isalpha(coords[Y][X + word.size()])) //prevents word from beeing placed so close that the '#' overlaps the previous word
					coords[Y][X + word.size()] = '#';
				else {
					setPos(posX, posY, direction);
					erase();
					cout << "You can't place words that way\n\n";
					update();
					break;
				}
			}

		}
	}
}

//============================================//
//============================================//

void Board::storeWord(string word) {
	string Pos = stringPos(); //position and direction given by the user
	usedWords.push_back(make_pair(Pos, word));
}

//============================================//
//============================================//

string Board::help() {
	string wildWord; //word with the letters that are in that line/column and wildcards characters instead of dots starting from the given position
	string line = sendLine(); //line/column since the position given to the end of it

	if (direction == 'H') {
		if (X != 0) { //if the position chosen is not the first of the line of the board
			if (isalpha(coords.at(Y).at(X - 1))) //if the position before in the line is a letter
				return wildWord; //returns empty word (it needs space for the hashtag)
		}
		for (size_t i = 0; i < line.length(); i++) {
			if (line.at(i) == '#') break; //if the character is an '#' the possible words have to end at most before it
			if (isalpha(line.at(i))) { //if it is a letter adds it to the wildWord
				wildWord.push_back(line.at(i));
			}
			else //if it is a dot adds a '?' to the end of the wildWord
				wildWord.push_back('?');
		}
	}

	else if (direction == 'V') {
		if (Y != 0) { //if the position chosen is not the first of the column of the board
			if (coords.at(Y - 1).at(X) != '.' && coords.at(Y - 1).at(X) != '#') //if the position before in the column is a letter
				return wildWord; //returns empty word (it needs space for the hashtag)
		}

		for (size_t i = 0; i < line.length(); i++) {
			if (line.at(i) == '#') break; //if the character is an '#' the possible words have to end at most before it
			if (line.at(i) != '.') { //if it is a letter adds it to the wildWord
				wildWord.push_back(line.at(i));
			}
			else //if it is a dot adds a '?' to the end of the wildWord
				wildWord.push_back('?');
		}
	}

	for (int i = (int)wildWord.length() - 1; i >= 0; i--) //replaces the last '?' for '*' until the last letter of the word
		if (wildWord.at(i) == '?')
			wildWord.at(i) = '*';
		else
			break;

	return wildWord;
}

//============================================//
//============================================//

string  Board::sendLine() {

	string line; //line of the board starting on the setPos

	if (direction == 'V') {
		for (size_t i = 0; i < sizeY - Y; i++)
			line += coords[Y + i][X];
	}

	if (direction == 'H') {

		for (size_t i = 0; i < sizeX - X; i++)
			line = line + coords[Y][X + i];
	}

	return line;
}

//============================================//
//============================================//

void Board::erase() {

	bool posFound = false; //used to verified if a word was erased
	string Pos = stringPos();  //position and direction given by the user

	for (size_t i = 0; i < usedWords.size(); i++) {
		if (usedWords[i].first == Pos) {
			usedWords.erase(usedWords.begin() + i);
			posFound = true;
			break;
		}
	}

	if (!posFound)
		cout << "Error, word not found.1n\n\n";
}

//============================================//
//============================================//

string Board::stringPos() {
	string Pos;
	Pos.push_back(posY);
	Pos.push_back(posX);
	Pos.push_back(direction);
	return Pos;

}

//============================================//
//============================================//

void Board::save(string dictFile) {
	string bFile; //string to store the name of the board .txt

				  //NAME OF THE FILE
	if (!boardFile.empty()) //if the board was loaded
		bFile = boardFile;
	else {
		ifstream fileD; //File open for reading (file.peek())
		string nameFile = "b";
		int num = 1;

		bFile = nameFile;  //adds 'b'
		zerosNameFile(&bFile, num); //adds zeros
		bFile += to_string(num) + ".txt"; //adds the number and the extension
		fileD.open(bFile);

		while (fileD.peek() != EOF) { //A new file is created if it doesn´t exist and the words are saved in it
			fileD.close();
			num += 1;
			bFile = nameFile; //adds 'b'
			zerosNameFile(&bFile, num); //adds zeros
			bFile += to_string(num) + ".txt"; //adds the number and the extension
			fileD.open(bFile); //open the file with the new name
		}
		fileD.close();
	}

	ofstream fileW; //File open for writing
	fileW.open(bFile);

	cout << "\nSaving board into file " << bFile << " ...\n";

	fileW << dictFile << endl << endl; //writes the name of the file

	for (size_t i = 0; i < coords.size(); i++) {  //writes the board
		for (size_t j = 0; j < coords.at(i).size(); j++)
			fileW << coords.at(i).at(j) << " ";

		fileW << endl;
	}
	fileW << endl;

	for (size_t i = 0; i < usedWords.size(); i++) { //writes the position and the respective words used
		fileW << usedWords.at(i).first << " " << usedWords.at(i).second;

		fileW << endl;
	}
	fileW.close();
	cout << "The board was successfully saved in " << bFile << ".\n\n";
}

//============================================//
//============================================//

void zerosNameFile(string *boardFile, int num) {
	if (num > 0 && num < 10) //if number has only 1 digit adds 2 zeros to the file's name
		*boardFile += "00";
	else if (num > 9 && num < 99) //if number has only 2 digit adds a zero to the file's name
		*boardFile += "0";
	else if (num > 999) { //if number has more than 3 digits
		cout << "It is not possible to save the board into a file. Delete some previous file and try again!\n\n";
		exit(1);
	}
}

//============================================//
//============================================//

string Board::load(string bFile) {
	ifstream fileB; //File open for reading
	string synomFile, line; //dictionary file's name with the synonyms; stores each line of the text file
	boardFile = bFile;

	//OPENING FILE WITH SAVED BOARD
	fileB.open(boardFile);

	if (fileB.fail())//if opening file fails exit program
	{
		cerr << "Impossible to open the file " << boardFile << endl << endl;
		exit(1);
	}
	else //if opening file doesn't fail
	{
		int i = -1;//index of the vector of vectors with the letters (or dots or hashtags) placed according to their coordinates

		getline(fileB, line); //reads synonyms file name
		synomFile = line;

		getline(fileB, line); //reads blank line

							  //GET LINES OF THE BOARD
		getline(fileB, line); //reads first line of the board

		while (line.length() != 0) { //reads lines until an empty line
			i++;
			coords.resize(i + 1); //adds to the end of the vector a empty vector that will have the respective line of the board
			for (int j = 0; j < (int)line.length(); j += 2)//reads the letters (or dots or hashtags) that are separated by spaces and put them in the vector 
				coords.at(i).push_back(line.at(j));

			getline(fileB, line); //reads next line of the board
		}

		//GET POSITION AND RESPECTIVE WORD
		stringstream ssPosWord; //keeps the position and word each line of the text file
		int k = -1; //index of the vectors, one with the used and the other with the respective position

		getline(fileB, line);
		while (!fileB.eof()) {
			if (line.length() != 0) //doesn't read blank lines
			{
				k++;
				usedWords.resize(k + 1); //increases the size of the vector in 1 position
				ssPosWord.str(line); //puts in the stringstream the line with the position and the word in this order
				ssPosWord >> usedWords.at(k).first >> usedWords.at(k).second; //puts strings in the respective vector
				ssPosWord.clear(); //clear eof flag
			}

			getline(fileB, line); //reads next position and word
		}
		fileB.close();
	}
	setSize();
	return synomFile;
}

//============================================//
//============================================//

void Board::setSize() {
	sizeY = coords.size(); //number of vectors that are in the main vector
	sizeX = coords.at(0).size(); //size of the first vector inside the main vector that is the same as the others
}

//============================================//
//============================================//

bool Board::fullBoard() {
	for (size_t i = 0; i < coords.size(); i++)
		for (size_t j = 0; j < coords[i].size(); j++) {
			if (coords[i][j] == '.') //if there is at least one dot, the board is not full
				return false;
		}
	return true;
}

//============================================//
//============================================//

bool Board::finalValid() {
	string line, //line from the board that is going to be shrinked to the size of the word saved in the usedWords vector
		word; //word that must be on the board on the respective position and direction
	stringstream ssPosDir; //keeps the positions and the direction of each word

	for (size_t i = 0; i < usedWords.size(); i++) {
		ssPosDir.str(usedWords.at(i).first);
		ssPosDir >> posY >> posX >> direction; //
		posToCoord(); //converts each letter of the coordinates to the respective index of the board

		word = usedWords.at(i).second;
		line = sendLine();
		line = line.substr(0, word.length()); //word that is on the board from the position saved to the length of the word saved
		if (line != word) //if the word is not the same that is on the board
			return false;

		ssPosDir.clear(); //clear eof flag
	}
	return true;
}

//============================================//
//============================================//

void Board::finishBoard() {
	for (size_t i = 0; i < coords.size(); i++)
		for (size_t j = 0; j < coords[i].size(); j++) {
			if (coords[i][j] == '.') //replaces the remaining dots for hashtags to complete the board
				coords[i][j] = '#';
		}
	cout << endl;
}

//============================================//
//============================================//

struct mycompare {
	/* @ left - first pair (Pos,Word) with the position of the word and the word
	@ rigth - second pair (Pos,Word) to compare
	*/

	bool  operator() (const pair<string, string>& left, const pair<string, string>& right)
	{
		if (right.first == left.first) return false; //if the two elements to compare are the same, does nothing

		if (right.first[2] != left.first[2])		//if one of the positions is horzontal and the other ir vertical
			return left.first[2] < right.first[2];	//the horizontal pair comes first than the vertical one

		if ((left.first[2] == right.first[2]) && (left.first[2] == 'H')) {	//if both positions are horizontal
			return left < right;											//comes first the position alphabetically smaller
		}

		if ((left.first[2] == right.first[2]) && (left.first[2] == 'V')) {  //if both positions are vertical
			if (left.first[1] == right.first[1])							//if the second letter of the position is equal in both pairs
				return left.first[0] < right.first[0];						//comes first the pair with the first letter alphabetically smaller
			return left.first[1] < right.first[1];							//if the second letter is different, comes first the pair with the second letter alphabetically smaller
		}
		return false;
	}
}comparation;

//============================================//

void Board::sortUsedWords() {
	sort(usedWords.begin(), usedWords.end(), comparation);  //the elements are compared using comparation object
}


//////////////////////////////////////////////////////////////////////////////////
////////////////////////METHODS USED TO PLAY THE GAME ONLY////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void Board::prepareVector() {

	//replaces the vector coords with the apropriate characters to form the playing board
	for (unsigned int x = 0; x < coords.size(); x++) {
		for (unsigned int y = 0; y < coords[x].size(); y++) {
			if (isalpha(coords[x][y])) {
				coords[x][y] = '.';
			}
		}
	}
}

//============================================//
//============================================//

void Board::showPlayerBoard() {

	setcolor(12);
	cout << "   ";
	for (unsigned int i = 0; i < coords[0].size(); i++) //prints the first line of red lower case letters to indicate the x coordinate
		cout << " " << char(97 + i);
	cout << endl;

	for (unsigned int y = 0; y < coords.size(); y++) {
		setcolor(12);
		cout << " " << char(65 + y) << " "; //prints the red capital letter that indicates the Y coordinate
		setcolor(112); cout << " ";
		for (unsigned int x = 0; x < coords[y].size(); x++) { //prints the characters of coords vector with a " " in between 
			setcolor(112);

			if (isalpha(coords[y][x]) || coords[y][x] == '.') { //if the character is alpha prints the character itself
				cout << coords[y][x];
				cout << ' ';
			}
			else { //if the character is not alpha prints a small black square( char(254))
				cout << char(254);
				cout << ' ';
			}
		}
		cout << endl;
	}
	setcolor(7); cout << endl;
}

//============================================//
//============================================//

bool Board::existsPosition(char line, char column, char dir)
{
	string pos;
	pos.push_back(line);
	pos.push_back(column);
	pos.push_back(dir);

	//check if there is a word that begins on setPos
	for (size_t i = 0; i < usedWords.size(); i++)
		if (pos == usedWords.at(i).first)
			return true;

	return false;
}

//============================================//
//============================================//

bool Board::checkLengthWord(string word) {
	string Pos = stringPos();

	//check if the word lenghts match
	for (size_t i = 0; i < usedWords.size(); i++) {
		if (Pos == usedWords.at(i).first)
			if (usedWords.at(i).second.length() != word.size()) {
				return false;
			}
			else
				return true;
	}
	return false;
}

//============================================//
//============================================//

void Board::insert(string word) {
	string Pos = stringPos();

	//insert the word on the board and vector
	guessedWords.push_back(make_pair(Pos, word));
}

//============================================//
//============================================//

void Board::updatePlayerBoard() {

	for (size_t j = 0; j < guessedWords.size(); j++) { //for every word, updates coords
		posY = guessedWords[j].first.at(0);
		posX = guessedWords[j].first.at(1);
		direction = guessedWords[j].first.at(2);
		posToCoord();
		string word = guessedWords[j].second;

		if (direction == 'V') {
			for (size_t i = 0; i < word.size(); i++)
				coords[Y + i][X] = word.at(i);

		}
		if (direction == 'H') {
			for (size_t i = 0; i < word.size(); i++)
				coords[Y][X + i] = word.at(i);
		}
	}
}

//============================================//
//============================================//

void Board::erasePlayerBoard() {

	size_t i = 0;
	string Pos = stringPos();
	string word;

	//erases word from guessesWords
	for (i; i < guessedWords.size(); i++) {
		if (guessedWords[i].first == Pos) {
			word = guessedWords[i].second;
			guessedWords.erase(guessedWords.begin() + i);
			break;
		}
	}

	//erases word from the board
	if (direction == 'V') {
		for (size_t i = 0; i < word.size(); i++)
			coords[Y + i][X] = '.';

	}
	if (direction == 'H') {
		for (size_t i = 0; i < word.size(); i++)
			coords[Y][X + i] = '.';
	}

}

//============================================//
//============================================//

void Board::getUsedWords(vector <pair <string, string> > &usedPosWords) {
	usedPosWords = usedWords;
}

//============================================//
//============================================//

string Board::placed() {

	string Pos = stringPos();

	for (size_t i = 0; i < guessedWords.size(); i++) { //checks if the user already placed a word there
		if (Pos == guessedWords.at(i).first) {
			return guessedWords.at(i).second;

		}
	}
	return " ";
}

//============================================//
//============================================//

string Board::getBoardFile() {
	return boardFile;
}
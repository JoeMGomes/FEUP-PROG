/*
FILE : Dictionary.cpp
DATE : 2018/05/03
AUTHORS : José Gomes e Liliana Almeida
PROGRAM PURPOSE:
- open and read the words and their synonyms from a text file (removing extra spaces in each word);
- replace lowercase letters for uppercase letters in a word;
- returns the synonyms file's name;
- check if a word belongs to the dictionary;
- with a word with wildcard characters, make a vector with all possible words that can be put in the position and direction given by the user;
- scramble the words of a vector;
- check if the word fits in the board with the other words that are already in it;
- keep the synonyms and the random clue with the position of its corresponding word of the dictionary;
- show the clues on the screen;
- gets an alternative clue if the user asks for help.
*/

#include "Dictionary.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>

/* Removes multiple spaces and sapces in the beggining and in the end of a word
 * @param	word - word in which spaces will be removed
*/
void removeSpaces(string *word);

/* Checks if a word is made up only by letters and certain characters:  (space)'()-
 * @param	word - word to be checked
 * @return	true if the word contain only letters and certain characters, otherwise flase
*/
bool checkValidWord(string *word);

/* Sorts, removes the repeated words of a vector and scrambles them
* @param	helpList - vector to be scrambled and returned
*/
void scrambleVector(vector<string> &helpList);

/* Checks if a word matches the string with wildcard characters
 * @param	str - word in C-string to be checked
 * @param	strWild - C-string with wildcard characters
 * @return	true if the word matched the string with wildcard characters
*/
bool wildcardMatch(const char *str, const char *strWild);


//============================================//
//============================================//
//============================================//

Dict::Dict(string synonFile) {
	this->synonFile = synonFile;
}

//============================================//
//============================================//

void Dict::openFile() {

	ifstream fileSynon; //ifstream object to read a file

	//OPENING DICTIONARY FILE
	fileSynon.open(synonFile); //open the file with the synonyms

	if (fileSynon.fail())//if opening file fails exit program
	{
		cerr << "Impossible to open the file " << synonFile << endl << endl;
		exit(1);
	}
	else //if opening file doesn't fail
	{
		readFile(&fileSynon); //reads the file and saves all words and synonyms into a map
		fileSynon.close();
	}
}

//============================================//
//============================================//

void Dict::readFile(ifstream *fileS) {

	string line; //line from the dictionary file
	string dicWord; //saves the line and after erasing the other words it will be a dictionary word
	string synonWord; //saves the line with more than one synonym and it will be a synonym
	vector <string> synons; //contains the synonyms of a dictionary word

	getline(*fileS, line); //read the line from the synonyms file

	while (!(*fileS).eof()) //while the program doesn't read the end of the file, it will read a new line
	{
		if (line.length() != 0)//doesn't read blank lines
		{
			// READ AND SAVE THE WORD UNTIL ':' IN THE DICTIONARY VECTOR
			if (line.find_first_of(":") != -1) {

				dicWord = line;
				dicWord.erase(dicWord.begin() + dicWord.find_first_of(":"), dicWord.end());//separates the first word from the others
				line.erase(0, line.find_first_of(":") + 1); //deletes from the original word the first part until ':'

				removeSpaces(&dicWord);
				lowToUpperCase(&dicWord);
			}

			//IF IT HAS ','
			while (line.find(",") != -1) //while the line contains ","
			{
				synonWord = line;

				synonWord.erase(synonWord.begin() + synonWord.find_first_of(","), synonWord.end());//separates the first word from the others
				if (synonWord.find_first_not_of(" ") != -1)//if the string is filled only by spaces
				{
					removeSpaces(&synonWord);
					lowToUpperCase(&synonWord);
					if (checkValidWord(&synonWord))
						synons.push_back(synonWord); //adds the synonym to the end of the vector
				}
				line.erase(0, line.find_first_of(",") + 1); //deletes from the original word the first part until ","
			}
			if (line.find_first_not_of(" ") != -1) //if the string isn't filled only by spaces
			{
				removeSpaces(&line);
				lowToUpperCase(&line);
				if (checkValidWord(&line))
					synons.push_back(line); //adds the synonym to the end of the vector
			}
		}

		//INSERT THE DICTIONARY'S WORD AND ITS SYNONYMS IN THE MAP
		pair< map <string, vector <string> >::iterator, bool> ret; //pair that receives (after map::insert funtion) in the first member an iterator pointing to either the newly 
																   //inserted element or to the element with an equivalent key in the map and the second member is set to true if 
																   //a new element was inserted or false if the key already existed

		ret = dictSynonWords.insert(pair <string, vector <string> >(dicWord, synons)); //insert pair with the dictionary word and the vector of strings with the synonyms of that word 

		if (ret.second == false) //if the second element returns false, the key already exists
			dictSynonWords[dicWord].insert(dictSynonWords[dicWord].end(), synons.begin(), synons.end()); //adds the vector that is already int the map with that key and the vector 
																										 //with new synonyms for the same dictionary word(key)


		synons.clear(); //clear the vector with synonyms
		getline(*fileS, line); //read another linE
	}
}

//============================================//
//============================================//

void removeSpaces(string *word) {

	while ((*word).find("  ") != string::npos) //replaces multiple characters spaces for only one character space
		(*word).erase((*word).find("  "), 1);

	if ((*word)[0] == ' ') //if it exists, removes the character space from the beggining of the word
		(*word).erase(0, 1);

	if ((*word)[(*word).length() - 1] == ' ') //if it exists, removes the character space from the end of the word
		(*word).erase((*word).length() - 1, 1);
}

//============================================//
//============================================//

void Dict::lowToUpperCase(string *word) {
	for (size_t i = 0; i < (*word).length(); i++)	//for all the letters of the word
		if (!isupper((*word)[i]))					//if it's a lowercase letter
			(*word).at(i) = toupper((*word)[i]);	//converts it to uppercase letter
}

//============================================//
//============================================//

bool checkValidWord(string *word) {

	if ((*word).find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ '()-") != -1) //if the word doesn't contain only uppercase letters('A' - 'Z') and the other characters specified
																			//it is not a valid word
		return false;
	if ((*word).find_last_of("-") == (*word).length() - 1) //if the word ends with a '-' characater
		return false;									   //the word is not valid
	return true;
}

//============================================//
//============================================//

string Dict::getSynonFile() {
	return synonFile;
}

//============================================//
//============================================//

bool Dict::valid(string word) {

	if (dictSynonWords.find(word) != dictSynonWords.end()) { //if the word is a map's key then it belongs to the dictionary
		return true;										 //so it's a valid word
	}
	else	//otherwise the word is not valid
		return false;
}

//============================================//
//============================================//

void Dict::incWords(string wildWord, vector <string>&possibleWords) {

	for (map<string, vector <string> >::iterator it = dictSynonWords.begin(); it != dictSynonWords.end(); ++it) { //for each word of the dictionary
		if (it->first.length() > wildWord.length() || it->first.length() < wildWord.find_last_not_of("*?") + 1) continue; //if the word is bigger than the word with the wildcard characters 
																														  //or smaller than the length of wildWord until its last letter 
		if (wildcardMatch(it->first.c_str(), wildWord.c_str())) { //checks if the word matches the string with wildcard characters

			if (fits(it->first, wildWord)) //checks if the word fits the other words that are in the board
				possibleWords.push_back(it->first); //adds the word to the end of the vector
			else
				continue; //if the word doesn't fit check the next word of the dictionary
		}
	}

	if (wildWord.find_first_not_of("*?") != -1) { //if it doesn't find any letter in the word with wildcards
		if (wildWord.find_last_not_of("*?") != 0) { //if the only letter left is not in the first position, if it has any letter
			if (wildWord.find_first_not_of("*?") != -1) //if the wild word has at least one letter

				while (isalpha(wildWord.at(wildWord.find_last_not_of("*?") - 1))) { //while there are two adjacent letters as the last letter to appear on the wildWord
					wildWord.erase(wildWord.begin() + wildWord.find_last_not_of("*?"), wildWord.begin() + wildWord.find_last_not_of("*?") + 1); //erases the last one of the letters 
																																				//(the word needs to end with an hastag)
					if (wildWord.find_last_not_of("*?") == 0) { //if the only letter left is in the first position
						return;
						scrambleVector(possibleWords);
					}
				}
			wildWord.erase(wildWord.begin() + wildWord.find_last_not_of("*?") - 1, wildWord.end()); //erases from the position before the last letter of the wildWord 
																									//(to save a position for the hastag) to the end
		}
		else { //if the only letter left is in the first position, it's not possible to shrink the word so there is no more possible words
			return;
			scrambleVector(possibleWords);
		}

		//SHRINK WILDWORD
		if (wildWord.find_first_not_of("?") != -1) { //if the wildWord has at least one letter
			for (size_t i = wildWord.length() - 1; i >= 0; i--) //replaces the '?' for '*' since the last letter of the wildWord to the end of it
				if (wildWord.at(i) == '?')
					wildWord.at(i) = '*';
				else
					break;
		}
		incWords(wildWord, possibleWords); //repeats the proccess with the smaller word
	}
	scrambleVector(possibleWords);
}

//============================================//
//============================================//

void scrambleVector(vector<string> &helpList) {

	//SORT VECTOR ELEMENTS
	sort(helpList.begin(), helpList.end());

	//REMOVE REPEATED ELEMENTS
	helpList.erase(unique(helpList.begin(), helpList.end()), helpList.end()); //removes the words that are repeated in the vector except the first one

																			  //SCRUMBLE VECTOR ELEMENTS

	if (!(helpList.size() < 3)) { //if the size of the list is bigger than 2 elements
		size_t k; //random index of the vector

		for (size_t i = 0; i < helpList.size(); i++) //every word of the vector is going to be switched with another word
		{
			k = rand() % (helpList.size()); //random index between 0 and the number of elements of helpList minus 1

			while (k == i)
				k = rand() % (helpList.size()); //it doesn't allow to switch the word for itself so t will be more scrambled

			swap(helpList.at(i), helpList.at(k)); //switch the words in the index i and k of the vector
		}
	}
}

//============================================//
//============================================//

bool Dict::fits(string word, string space) {

	string wTM = space.substr(0, word.size()); //line/column of the board starting in the position given and with the length of the word to be checked

	for (size_t i = 0; i < wTM.size(); i++) //replaces all the dots for '*'
		if (wTM[i] == '.')
			wTM[i] = '*';

	return wildcardMatch(word.c_str(), wTM.c_str()); //returns the bool value that the function wildcardMatch will return
}

//============================================//
//============================================//

bool wildcardMatch(const char *str, const char *strWild) {
	// We have a special case where string is empty ("") and the mask is "*".
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild)
	{	// Single wildcard character
		if (*strWild == '?') {
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else if (*strWild == '*') {
			//Need to do some tricks.
			//1. The wildcard * is ignored. 
			//So just an empty string matches. This is done by recursion.
			//Because we eat one character from the match string,
			//the recursion will stop.
			if (wildcardMatch(str, strWild + 1))
				// we have a match and the * replaces no other character
				return true;
			// 2. Chance we eat the next character and try it again, 
			//  with a wildcard * match. This is done by recursion.
			// 	Because we eat one character from the string,
			//	the recursion will stop.
			if (*str && wildcardMatch(str + 1, strWild))
				return true;
			// Nothing worked with this wildcard.
			return false;
		}
		else {
			// Standard compare of 2 chars. Note that *	str might be 0 here,
			// but then we never get a match on * strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return
		!*str && !*strWild;
}



////////////////////////////////////////////////////////////////////////////////////
////////////////////////METHODS USED TO PLAY THE GAME ONLY//////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void Dict::setSynonsBoard(vector <pair <string, string> > &usedWords) {
	for (size_t i = 0; i < usedWords.size(); i++) { //for every element of the usedWords vector
		synonyms.resize(i + 1);	//adds an empty element to the synonyms' vector
		synonyms[i].first = usedWords[i].first; //puts in the first member of the pair the position; usedWords[i].first - position and direction of the words

		if (dictSynonWords.find(usedWords[i].second) != dictSynonWords.end()) //if there is at least one synonym for that word;
																			  //* usedWords[i].second - word of the dictionary to check if has any synonym
			synonyms[i].second = dictSynonWords[usedWords[i].second];		  //they are written in the second member of the pair 
																			  //* dictSynonWords[usedWords[i].second] - synonyms of the word usedWords[i].second
		else		//if there isn't any synonyms, it leaves the second member empty
			continue;
	}
}

//============================================//
//============================================//

void Dict::setClues() {
	int j; //random index of the vector

	for (size_t i = 0; i < synonyms.size(); i++) { //for each word of synonyms
		clues.resize(i + 1);
		clues[i].first = synonyms[i].first; //saves the coordinates of the word in the first member of the pair
		j = rand() % (synonyms[i].second.size()); //random index between 0 and the number of synonyms the word minus 1
		clues[i].second = synonyms[i].second[j]; //saves the random synonym as a clue for that word in the second member of the pair
		synonyms[i].second.erase(synonyms[i].second.begin() + j); //erases the synonym from the synonyms vector so it won't appear again
	}
}

//============================================//
//============================================//

void Dict::showClues() {

	if (!clues.empty()) {

		size_t i = 0; //index of the clues vector

		cout << "CLUES\n"
			<< "======";

		///////HORIZONTAL CLUES//////////////
		if (clues[i].first.at(2) == 'H') {
			float numH = 0; //number of lines to be occupied with horizontal clues which is the index of the first word in the second column

			cout << "\nHORIZONTAL:\n";

			//VERIFY HOW MANY LINES HORIZONTAL CLUES WILL OCUPPY IN TWO COLUMNS
			for (size_t k = 0; k < clues.size(); k++) { //increases the numH variabel until a direction's word is vertical, having in the end the number of horizontal clues
				if (clues[k].first.at(2) == 'V') break;
				else numH++;
			}
			numH = ceil(numH / 2); //number of lines horizontal clues will occupy

			//SHOWING THE HORIZONTAL CLUES IN TWO COLUMNS
			for (; clues[i].first.at(2) == 'H' && i < clues.size(); i++) {
				if (i == numH) break; //if number of horizontal clues is eaven, when i=numH it means that all clues are already written

				cout << left << clues[i].first.at(0) << clues[i].first.at(1) << " -> " << setw(25) << clues[i].second; //output of the firts position and clue  of the first column

				if (clues[i + (int)numH].first.at(2) == 'H' && i + (int)numH < clues.size()) //if number of horizontal clues is odd, in the last iteration it stops here beacause 
																							 //the index of the second column doens't exits or it is an index of a vertical clue
					cout << clues[i + (int)numH].first.at(0) << clues[i + (int)numH].first.at(1)
					<< " -> " << setw(25) << clues[i + (int)numH].second << endl; //output of the firts position and clue of the second column
				else {
					cout << endl; break;
				}
			}
			i += (size_t)numH; //index of the first vertical word if it exists
		}

		///////VERTICAL CLUES/////////////////
		if (i < clues.size() && clues[i].first.at(2) == 'V') { //if the index i exists and it's an index of a vertical clue
			int numV; // //number of lines to be occupied with vertical clues which is the index of the first word in the second column

			if (i == 0) //if there isn't any horizontal clues, there is only vertical clues
				numV = (int)ceil(clues.size() / 2);

			else //if there is at least one horizontal clue
				numV = (int)ceil((clues.size() - (i - 1)) / 2); //(clues.size() - (i - 1)) -> number of vertical clues

			int j = i; //index of the first vertical clue

			//VERIFY HOW MANY LINES VERTICAL CLUES WILL OCUPPY IN TWO COLUMNS
			cout << "\nVERTICAL:\n";
			for (; i < clues.size(); i++) {
				if (i == j + numV) break; //if number of vertical clues is eaven, when i== j+numV it means that all clues are already written

				cout << left << clues[i].first.at(0) << clues[i].first.at(1) << " -> " << setw(25) << clues[i].second; //output of the firts position and clue  of the first column

				if (i + numV < clues.size()) //if number of vertical clues is odd, in the last iteration it stops here beacause the index of the second column doens't exits or it's 
											 //an index of a vertical clue
					cout << clues[i + numV].first.at(0) << clues[i + numV].first.at(1)
					<< " -> " << setw(25) << clues[i + numV].second << endl; //output of the firts position and clue of the second column

				else {
					cout << endl; break;
				}
			}
		}
		cout << endl;
	}
	else {
		cout << "There isn't any clues to show. Choose another board.\n\n";
		exit(1);
	}
}

//============================================//
//============================================//

void Dict::getAlternativeClue(string position, string& helpClue) {
	int j; //random index of synonyms vector

	for (size_t i = 0; i < synonyms.size(); i++)
		if (synonyms[i].first == position) { //searches in the synonyms vector for the position given by the user
			if (!synonyms[i].second.empty()) { //if there are synonyms for that word
				if (synonyms[i].second.size() < 2) //if there is only one synonym
					j = 0;	  //index of the synonym
				else j = rand() % (synonyms[i].second.size()); //random index between 0 and the number of synonyms of the word minus 1

				helpClue = synonyms[i].second[j];	//random clue is returned in helpClue
				synonyms[i].second.erase(synonyms[i].second.begin() + j); //erases the synonym from the synonyms vector so it won't appear again
			}
			break;
		}
}

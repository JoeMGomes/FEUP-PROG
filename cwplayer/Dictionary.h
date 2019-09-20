#ifndef dict_H
#define dict_H
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Dict {

public:
	/* Contructor of the class defining the class data member synonFile
	 * @param	synonFile - name of the file with the synonyms
	*/
	Dict(string synonFile);

	/* Opens the file and sends it to be read */
	void openFile();

	/* Converts the lowercase letters of a word to uppercase letters
	 * @param	word - word to be converted
	*/
	void lowToUpperCase(string *word);

	/* Returns	the name of the file with the synonyms
	 * @return	name of the synonyms' file
	*/
	string getSynonFile();

	/* Checks if a word belongs to the dictionary
	 * @param	word - word to be checked
	 * @return	true if the word belongs to the dictionary, otherwise false
	*/
	bool valid(string word);

	/* Finds which words of the dictionary can be put in the position and direction chosen
	 * @param	wildWord - line/column starting in the position given by the user, where the dots were replaced by the wildcard '*' and '?'
	 * @param	possibleWords - vector with all the words possible in the coordinates given by the user
	*/
	void incWords(string wildWord, vector <string> &possibleWords);

	/* Verifies if a word can be put in that position and direction knowing the letters in that line or column, as the direction is horizontal or vertical
	 * @param	word - word to be checked
	 * @param	space - line/column starting in the position given by the user
	 * @return	true if the word fits with the other words of thw board, otherwise false
	*/
	bool fits(string word, string space);


	//////////////////////////////////////////////////////////////////////////////////
	////////////////////////METHODS USED TO PLAY THE GAME ONLY////////////////////////
	//////////////////////////////////////////////////////////////////////////////////

	/* For each word that the user must guess, stores into a pair the coordinates and the vector with all synonyms of the word in this order
	 * @param  usedWords - vector with the coordinates and words that the user must guess
	*/
	void setSynonsBoard(vector <pair <string, string> > &usedWords);

	/* Randomly chooses for each coordinate one of the synonyms stored, erasing it from the snonyms vector*/
	void setClues();

	/* Show the clues separated in horizontal and vertical words */
	void showClues();

	/* Returns by reference another clue for the coordinates specified by the user, erasing it from the synonyms vector
	 * @param	position - position and direction chosen by the user
	 * @param	helpClue - clue returned to help the user; when there is no other clue in the synonyms vector this parameter is returned empty
	 */
	void getAlternativeClue(string position, string &helpClue);


private:
	/* Reads and saves the synonyms and dictionary's words into a map
	 * @param	fileS - ifstream object already associated with the file to be read
	*/
	void readFile(ifstream *fileS);


	//name of the synonyms file
	string synonFile;

	//stores the words of the dictionary as keys and all of their synonyms as mapped values associated with they're corresponding key
	map <string, vector <string> > dictSynonWords;


	//////////////////////////////////////////////////////////////////////////
	///////////////////////USED TO PLAY THE GAME ONLY/////////////////////////
	//////////////////////////////////////////////////////////////////////////

	//stores the start position of the word and all of its clues in pairs (Pos, vector with clues)
	vector <pair <string, vector <string> > > synonyms;

	//stores the start position of the word and its clue in pairs (Pos, Clue)
	vector <pair <string, string> > clues;
};

#endif
#ifndef player_H
#define player_H
#include <iostream>
#include <ctime>

using namespace std;

class Player {
public:
	/* Contructor of the class defining the class data member playerName
	 * @param	name - player's name
	*/
	Player(string name);

	/* Use to get the player's name
	 * @return	name of the player
	 */
	string getNamePlayer();

	/* Saves the time the user starts solving the game */
	void startTime();

	/* Saves the time the user finished the game */
	void stopTime();

	/* Use to get the time the player took to solve the game
	 * @return	total time in minutes:seconds
	*/
	string getTime();

	/* Use to get number of clues the user got
	 * @return	number of clues
	*/
	int getNumberClues();

	/* Increases the number of clues given to the user */
	void addNumberClues();

	/* Saves player's information into a file appending it to other players' information
	 * @param	boardFile - name of the boards file
	 */
	void savePlayerInfo(string boardFile);

private:
	/* Converts the total time in seconds to minutes and seconds */
	void convertTime();


	//name of the player
	string namePlayer;

	//time the user starts playing the game
	time_t iTime;

	//time the user finished the game
	time_t fTime;

	//total time the player took to solve the game in minutes and seconds
	string  totalTime;

	//number of clues given to the user
	int numberClues;
};

#endif
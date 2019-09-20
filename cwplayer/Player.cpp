/*
FILE : Player.cpp
DATE : 2018/05/15
AUTHORS : José Gomes e Liliana Almeida
PROGRAM PURPOSE:
- Keep player's information;
- Count the time the user needs to solve the puzzle, converting that time in the end;
- Count numbe of alternatives the user gets;
- Save all that information in a text file which name is formed from the board file's name.
*/

#include "Player.h"
#include <fstream>
#include <iomanip>
#include <string>


Player::Player(string namePlayer) {
	this->namePlayer = namePlayer;
}

//============================================//
//============================================//

string Player::getNamePlayer() {
	return namePlayer;
}

//============================================//
//============================================//

void Player::startTime() {
	iTime = time(NULL); //save current time
	numberClues = 0; //intialize number of clues given to the user
}

//============================================//
//============================================//

void Player::stopTime() {
	fTime = time(NULL); //save current time 
	convertTime();
}

//============================================//
//============================================//

void Player::convertTime() {
	int m = 0; //minutes
	time_t s = fTime - iTime; //total time the user spent to solve the game in seconds

	while (s > 60) { //while seconds are bigger than 60, increases the minutes and decreases the seconds
		s -= 60;
		m++;
	}

	//saves minutes and seconds with the respective zeros if necessary, so the time has 2 digits for minutes and 2 digits for seconds
	if (m < 10 && s > 10)
		totalTime = "0" + to_string(m) + ":" + to_string(s);
	else if (m > 10 && s < 10)
		totalTime = to_string(m) + ":" + "0" + to_string(s);
	else if (m < 10 && s < 10)
		totalTime = "0" + to_string(m) + ":" + "0" + to_string(s);
	else
		totalTime = to_string(m) + ":" + to_string(s);
}

//============================================//
//============================================//

string Player::getTime() {
	return totalTime;
}

//============================================//
//============================================//

int Player::getNumberClues() {
	return numberClues;
}

//============================================//
//============================================//

void Player::addNumberClues() {
	numberClues += 1; //increases number of clues
}

//============================================//
//============================================//

void Player::savePlayerInfo(string boardFile) {
	ofstream fileP; //ofstream object to write in a file
	string playerFile; //name of the file where the player's information will be saved

	playerFile = boardFile.substr(0, boardFile.find(".txt")) + "_p.txt"; //construction of the name of the file with players' information from the board file's name
	fileP.open(playerFile, ios::app); //opens the file so that the new content will be appended to the current content of the file

	cout << "\nSaving player's information into file " << playerFile << " ...\n";

	fileP << left << namePlayer << " -> " << "Tempo: " << setw(7) << totalTime << "Number of clues asked: " << numberClues << endl; //saves name, total time needed to solve the game 
																																	//and number of clues used
	fileP.close();
	cout << "The information was successfully saved in " << playerFile << ".\n\n";
}


#include "classes.h"
#include <iostream>
MultiplayerGame::MultiplayerGame(const string& name, const string& instructions,\
	int min_players_, int max_players_, int min_age_ = 0) : Game(name, instructions) { //использую конструктор базового класса чтобы поставвить name и instructions_filename
	//т.к. они приватные и не доступны в методах(и конструкторах) производного класса
	//instructions_filename = instructions <- не сработает
	//но можно this->set_instructions_filename(instructions);
	min_players = min_players_;
	max_players = max_players_;
	min_age = min_age_;
}

MultiplayerGame::MultiplayerGame(const Game& game, int min_players_,\
 int max_players_, int min_age_ = 0) : Game(game){
	min_players = min_players_;
	max_players = max_players_;
	min_age = min_age_;
}

MultiplayerGame::MultiplayerGame(const MultiplayerGame& mg) : Game(mg) {
	//т.к MultiplayerGame производный от Game, его обьект можно передать в конструктор Game(const Game&). он воспримется как обьект Game 
	max_players = mg.max_players;
	min_players = mg.min_players;
	min_age = mg.min_age;
}

bool MultiplayerGame::CanPlay(Player** players, int len) const{
	int minage = 1000;
	for (int i = 0; i<len; i++) minage = (players[i]->get_age() < minage)? \
		players[i]->get_age(): minage;

	if(len >= min_players && len <= max_players && minage >= min_age) return true;

	return false;  
}

int MultiplayerGame::get_min_players() const {
	return min_players;
}

int MultiplayerGame::get_max_players() const {
	return max_players;
}

int MultiplayerGame::get_min_age() const {
	return min_age;
}

void MultiplayerGame::set_min_players(int min_players_) {
	if (min_players_ < 1) cout << "[ERR] invalid min_players" << endl;
	else min_players = min_players_;
}

void MultiplayerGame::set_max_players(int max_players_) {
	if (max_players_ < 1 || max_players_ < min_players) cout << "[ERR] invalid max_players" << endl;
	else max_players = max_players_;
}

void MultiplayerGame::set_min_age(int min_age_) {
	if (min_age_ < 0 || min_age_ > 200) cout << "[ERR] invalid min_age" << endl;
	else min_age = min_age_;
}

string MultiplayerGame::whoami() const {
	return "MultiplayerGame";
}
#include "classes.h"
#include <vector>
#include <memory>
#include <iostream>
using namespace std;

GameRoom::GameRoom(const vector<shared_ptr<Game>>& g, const vector<shared_ptr<Player>>& p){
	games = g;
	players = p;
}

GameRoom::GameRoom(const GameRoom& gr){
	games = gr.get_games();
	players = gr.get_players();
}

void GameRoom::add_game(shared_ptr<Game> g){
	games.push_back(g);
}

void GameRoom::add_player(shared_ptr<Player> p){
	players.push_back(p);
}

const vector<shared_ptr<Game>>& GameRoom::get_games() const { //нельзя изменять вектор но можно изменять игры
	return games;
}

const vector<shared_ptr<Player>>& GameRoom::get_players() const {
	return players;
}

vector<shared_ptr<Player>> GameRoom::get_players_subset(const vector<string>& names){
	vector<shared_ptr<Player>> subset;
	for(auto c = names.begin(); c<names.end(); c++){
		for(auto c2 = players.begin(); c2<players.end(); c2++){ //iterators
			if(*c == (*c2)->get_name()) subset.push_back(*c2);
		}
	}
	return subset;
}

shared_ptr<Game> GameRoom::chooseGame(const vector<shared_ptr<Player>>& p){
	int s = p.size();
	Player* raw_ptr[s]; //т.к. MultiplayerGame::CanPlay работает с сырыми указателями
	for(int i = 0; i<s; i++){
		raw_ptr[i] = p[i].get();
	}

	for(auto g: games){
		if (g->whoami() == "Game" || ( g->whoami() == "MultiplayerGame" && dynamic_pointer_cast<MultiplayerGame>(g)->CanPlay(raw_ptr, s))) {
			return g;
		}
	}

	return nullptr;
}
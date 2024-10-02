#include "classes.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;
Game::Game(string name_, string instructions) {
	name = name_;
	instructions_filename = instructions;
	prepared = false;
}

string Game::get_name() const {
	return name;
}

string Game::get_instructions() const
{
	//cout << instructions_filename << endl;
	ifstream file(instructions_filename);
	if (file.fail()) {
		cout << "[ERR] opening file " << instructions_filename << endl;
	}
	string buff;
	string res = "";
	while (getline(file, buff)) {
		res += buff + "\n";
		//cout << buff;
	}
	//cout << res << endl;
	return res;
}

Game::Game()
{
	prepared = false;
	return;
}

Game::Game(const Game &game) {
	name = game.get_name();
	instructions_filename = game.instructions_filename;
	prepared = false;
}

bool Game::prepare() {
	if (prepared == false) {
		prepared = true;
		return true;
	}
	return false;
}

bool Game::put_back() {
	if (prepared == true) {
		prepared = false;
		return true;
	}
	return false;
}

bool Game::get_status() const
{
	return prepared;
}


void Game::set_name(string name_)
{
	name = name_;
}

void Game::set_instructions(string instructions )
{
	instructions_filename = instructions;
}

string Game::get_instructions_filename() const
{
	return instructions_filename;
}
#pragma once
#include <string>

using namespace std;
class Game {
private:
	string name;
	//в задании - для игры нужны правила
	// мне показалось что это лучше хранить отдельном файле, а не прописывать при создании обьекта (много текста?)
	string instructions_filename;
	bool prepared;
public:
	Game();
	Game(string, string);
	Game(const Game&);

	string get_name() const;
	string get_instructions_filename() const;
	string get_instructions() const;

	void set_name(string);
	void set_instructions(string);
	
	bool get_status() const;

	bool prepare();
	bool put_back();

};
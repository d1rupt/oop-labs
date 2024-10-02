#include "classes.h"
#include <assert.h>
#include <fstream>
#include <iostream>

using namespace std;

void test1() {
	Game* game1 = new Game("jenga", "jenga.txt");
	assert(game1->get_name() == "jenga");
	assert(game1->get_instructions_filename() == "jenga.txt");

	remove("jenga.txt");

	ofstream file("jenga.txt");
	file << "rules";
	file.close();
	//cout << game1.print_instructions();
	assert(game1->get_instructions() == "rules\n");
	assert(game1->get_status() == false);
	game1->prepare();
	assert(game1->get_status() == true);
	game1->put_back();
	assert(game1->get_status() == false);
	cout << "[TEST1] OK" << endl;
}

void test2() {
	Game* game2 = new Game("ouija", "666");
	assert(game2->get_instructions() == "");

	game2->prepare();
	Game* game3 = new Game(*game2);
	assert(game3->get_name() == "ouija");
	assert(game3->get_instructions_filename() == "666");
	assert(game3->get_instructions() == "");
	assert(game3->get_status() == false);
	cout << "[TEST2] OK" << endl;
}

void test3() {
	Game* game3 = new Game("dnd", "dnd.txt");

	game3->set_name("dnd 2");
	game3->set_instructions("dnd_2.txt");

	assert(game3->get_name() == "dnd 2");

	ofstream file("dnd_2.txt");
	file << "indevelopment\nsoon";
	file.close();
	assert(game3->get_instructions_filename() == "dnd_2.txt");
	assert(game3->get_instructions() == "indevelopment\nsoon\n");

	cout << "[TEST3] OK" << endl;

}

void test4() {
	Game* game4 = new Game("karnoffel", "rules");
	delete game4;
	cout << "[TEST4] OK" << endl;
}


int main() {
	test1();
	test2();
	test3();
	test4();
	return 0;
}
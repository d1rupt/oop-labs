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

    assert(game1->get_instructions() == "rules\n");
    assert(game1->get_status() == false);

    game1->prepare();
    assert(game1->get_status() == true);

    game1->put_back();
    assert(game1->get_status() == false);

    delete game1;

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

    delete game2;

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

    delete game3;

    cout << "[TEST3] OK" << endl;
}

void test4() {
    MultiplayerGame* mGame1 = new MultiplayerGame("Uno", "uno.txt", 2, 4, 5);
    //тестируем методы

    assert(mGame1->whoami() == "MultiplayerGame");
    Game* game5 = mGame1;
    assert(game5->whoami() == "MultiplayerGame"); //т.к. функция виртульная, то хотя и указатель на Game, все равно должен вызывается MultiplayerGame::whoami()

    assert(mGame1->get_name() == "Uno");
    assert(mGame1->get_instructions_filename() == "uno.txt");
    assert(mGame1->get_min_players() == 2);
    assert(mGame1->get_max_players() == 4);
    assert(mGame1->get_min_age() == 5);

    mGame1->set_name("Puzzle");
    mGame1->set_instructions("puzzle.txt");
    mGame1->set_min_players(1);
    mGame1->set_max_players(10);
    mGame1->set_min_age(6);

    assert(mGame1->get_name() == "Puzzle");
    assert(mGame1->get_instructions_filename() == "puzzle.txt");
    assert(mGame1->get_min_players() == 1);
    assert(mGame1->get_max_players() == 10);
    assert(mGame1->get_min_age() == 6);

    assert(mGame1->prepare() == true);
    assert(mGame1->get_status() == true);

    Player* players[2];
    players[0] = new Player(6);
    players[1] = new Player(10);

    assert(mGame1->CanPlay(players, 2) == true);
    assert(mGame1->CanPlay(players, 0) == false);

    mGame1->set_min_age(100);
    assert(mGame1->CanPlay(players, 2) == false);

    delete players[0];
    delete players[1];

    delete mGame1;

    cout << "[TEST4] OK" << endl;
}

void test5() {
    //тестируем конструкторы
    Game* game4 = new Game("Chess", "chess.txt");
    assert(game4->whoami() == "Game");

    MultiplayerGame* mGame2 = new MultiplayerGame(*game4, 2, 2, 6);

    assert(mGame2->get_name() == "Chess");
    assert(mGame2->get_instructions_filename() == "chess.txt");
    assert(mGame2->get_min_players() == 2);
    assert(mGame2->get_max_players() == 2);
    assert(mGame2->get_min_age() == 6);

    MultiplayerGame* mGame3 = new MultiplayerGame(*mGame2);

    assert(mGame3->get_name() == "Chess");
    assert(mGame3->get_instructions_filename() == "chess.txt");
    assert(mGame3->get_min_players() == 2);
    assert(mGame3->get_max_players() == 2);
    assert(mGame3->get_min_age() == 6);

    delete game4;
    delete mGame2;
    delete mGame3;

    cout << "[TEST5] OK" << endl;
}

int main() {
    cout << "TESTING GAME CLASS" << endl;
    test1();
    test2();
    test3();

    cout << "TESTING PLAYER AND MULTIPLAYERGAME CLASSES" << endl;

    test4();
    test5();
    return 0;
}
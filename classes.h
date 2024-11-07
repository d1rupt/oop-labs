#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace std;

class Game {
   private:
    string name;
    string instructions_filename;
    bool prepared;

   public:
    Game();
    Game(const string&, const string&);
    Game(const Game&);

    string get_name() const;
    string get_instructions_filename() const;
    string get_instructions() const;

    void set_name(const string&);
    void set_instructions(const string&);

    bool get_status() const;

    bool prepare();
    bool put_back();

    virtual string whoami() const;
};

class Player {
   public:
    Player();
    Player(int a, string n = "");
    Player(const Player&);

    int get_age() const;
    string get_name() const;

   private:
    int age;
    string name;
};

class MultiplayerGame : public Game {
   public:
    MultiplayerGame();
    MultiplayerGame(const string&, const string&, int, int, int);
    MultiplayerGame(const Game&, int, int, int);
    MultiplayerGame(const MultiplayerGame&);

    bool CanPlay(Player** players, int len) const;

    int get_min_players() const;
    int get_max_players() const;
    int get_min_age() const;

    void set_min_players(int);
    void set_max_players(int);
    void set_min_age(int);

    string whoami() const override;

   private:
    int min_players;
    int max_players;
    int min_age;
};

class GameRoom {
public:
    GameRoom();
    GameRoom(const vector<shared_ptr<Game>>&, const vector<shared_ptr<Player>>&);
    GameRoom(const GameRoom&);

    vector<shared_ptr<Player>> get_players_subset(const vector<string>&);

    shared_ptr<Game> chooseGame(const vector<shared_ptr<Player>>&); //const reference чтобы не копировать и не могли изменять 
    void add_game(shared_ptr<Game>); 
    void add_player(shared_ptr<Player>);
    void remove_game(string name);
    void remove_player(string name);
    const vector<shared_ptr<Game>>& get_games() const; //same
    const vector<shared_ptr<Player>>& get_players() const;
private:
    vector<shared_ptr<Game>> games;
    //shared чтобы не игры не удалялись вместе с gameroom и можно было еще использовать в main
    //хотя это замедлит программу..
    vector<shared_ptr<Player>> players; //same
};
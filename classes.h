#pragma once
#include <string>

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
    Player(int);
    Player(const Player&);

    int get_age() const;

   private:
    int age;
};

class MultiplayerGame : public Game {
   public:
    MultiplayerGame();
    MultiplayerGame(const string&, const string&, int, int, int);
    MultiplayerGame(const Game&, int, int, int);
    MultiplayerGame(const MultiplayerGame&);

    bool CanPlay(Player**, int) const;

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
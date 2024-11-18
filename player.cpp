#include "classes.h"

Player::Player() {
    return;
};

Player::Player(int age_, string name_) {
    age = age_;
    name = name_;
}

Player::Player(const Player &player) {
    age = player.age;
}

int Player::get_age() const {
    return age;
}


string Player::get_name() const {
	return name;
}
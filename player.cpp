#include "classes.h"

Player::Player(int age_){
	age = age_;
}

Player::Player(const Player &player){
	age = player.age;
}

int Player::get_age() const {
	return age;
}
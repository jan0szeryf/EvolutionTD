#pragma once

class PlayerStats {
private:
	int hp;
	int money;
	int score;

public:
	PlayerStats() : hp(100), money(100), score(0) {}
};
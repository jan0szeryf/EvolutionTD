#pragma once

class PlayerStats {
private:
	int hp;
	int money;
	int score;

public:
	PlayerStats() : hp(100), money(100), score(0) {}

	int getHp() const {
		return hp;
	}

	int getMoney() const {
		return money;
	}

	int getScore() const {
		return score;
	}
};
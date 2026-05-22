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

	void takeDamage(int damage) {
		hp -= damage;
		if (hp < 0) {
			hp = 0;
		}
	}

	void addMoney(int amount) {
		money += amount;
	}

	void addScore(int points) {
		score += points;
	}
};
#pragma once

class PlayerStats {
private:
	int hp;
	int gold;
	int score;

public:
	PlayerStats() : hp(100), gold(100), score(0) {}

	int getHp() const {
		return hp;
	}

	int getGold() const {
		return gold;
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
		gold += amount;
	}

	void addScore(int points) {
		score += points;
	}

	void spendGold(int amount) {
		if (gold >= amount) {
			gold -= amount;
		}
	}
};
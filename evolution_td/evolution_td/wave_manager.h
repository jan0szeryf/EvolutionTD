#pragma once
#include "enemy.h"
#include "fast_enemy.h"
#include "tank_enemy.h"
#include "asset_manager.h"

class WaveManager {
private:
	int id;
	int currentWave = 0;
	int enemiesToSpawn = 0;
	float spawnTimer = 0.f;
	float timeBetweenSpawns = 1.0f;
	sf::Vector2f startPos;

	const AssetManager& assetManager;

public:
	WaveManager(int _id, const AssetManager& assets) : id(_id), assetManager(assets) {}

	void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
		if (enemiesToSpawn > 0) {
			std::unique_ptr<Enemy> enemy = nullptr;
			spawnTimer += deltaTime;

			if (spawnTimer >= timeBetweenSpawns) {
				spawnTimer = 0.f;

				if (id == 1) {
					if (currentWave == 1) {
						enemy = createEnemy("dog");
					}
				}

				if (enemy) {
					if (id == 1) {
						startPos = { 270.f, -2 * enemy->getRadius() };
					}
					enemy->setVirtualPos(startPos);

					std::cout << "Spawned enemy " << enemy->getName() << " for wave " << currentWave << ", at " << startPos.x << ", " << startPos.y << "\n";

					enemies.push_back(std::move(enemy));
				}

				--enemiesToSpawn;
			}
		}
	}

	std::unique_ptr<Enemy> createEnemy(const std::string& type) {
		if (type == "dog") {
			return std::make_unique<FastEnemy>("dog", 50, 5, 100.f, 20.f, assetManager.getTexture("dog_stance"));
		}
		return nullptr;
	}

	void nextWave() {
		++currentWave;
		if (id == 1) {
			if (currentWave == 1) {
				enemiesToSpawn = 3;
			}
			else if (currentWave == 2) {
				enemiesToSpawn = 5;
			}
			else if (currentWave == 3) {
				enemiesToSpawn = 2;
			}
		}
	}
	
	int getCurrentWave() const { return currentWave; }
};
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
	int wavesQuantity = 3;
	float spawnTimer = 0.f;
	float timeBetweenSpawns = 1.0f;
	sf::Vector2f startPos;

	const AssetManager& assetManager;

public:
	WaveManager(int _id, const AssetManager& assets) : id(_id), assetManager(assets) {
		if (id == 1) {
			wavesQuantity = 5;
		}
	}

	bool update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
		if (currentWave == wavesQuantity && enemiesToSpawn == 0 && enemies.empty()) {
			return true;
		}
		if (enemiesToSpawn > 0) {
			std::unique_ptr<Enemy> enemy = nullptr;
			spawnTimer += deltaTime;

			if (spawnTimer >= timeBetweenSpawns) {
				spawnTimer = 0.f;

				if (id == 1) {
					enemy = createEnemy("dog");
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
		return false;
	}

	std::unique_ptr<Enemy> createEnemy(const std::string& type) {
		if (type == "dog") {
			return std::make_unique<FastEnemy>("dog", 50, 5, 25, 100.f, 20.f, assetManager.getTexture("dog_stance"));
		}
		return nullptr;
	}

	void nextWave(bool hasActiveEnemies) {
		if (currentWave > 0 && (enemiesToSpawn > 0 || hasActiveEnemies)) {
			std::cout << "Cannot start next wave yet, still have enemies to spawn or active enemies\n";
			return;
		}
		++currentWave;
		if (id == 1) {
			if (currentWave == 1) {
				enemiesToSpawn = 3;
				timeBetweenSpawns = 1.5f;
			}
			else if (currentWave == 2) {
				enemiesToSpawn = 5;
				timeBetweenSpawns = 1.2f;
			}
			else if (currentWave == 3) {
				enemiesToSpawn = 7;
				timeBetweenSpawns = 1.0f;
			}
			else if (currentWave == 4) {
				enemiesToSpawn = 9;
				timeBetweenSpawns = 0.8f;
			}
			else if (currentWave == 5) {
				enemiesToSpawn = 11;
				timeBetweenSpawns = 0.6f;
			}
		}
		std::cout << "Starting wave " << currentWave << " with " << enemiesToSpawn << " enemies\n";
	}
	
	int getCurrentWave() const { return currentWave; }
};
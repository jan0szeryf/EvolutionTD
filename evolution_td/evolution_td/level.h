#pragma once
#include "background.h"
#include "gamestate.h"
#include "asset_manager.h"
#include "tower.h"
#include "hashitbox.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include <cmath>

class Level {
private:
	int id;
	std::string name;
	std::unique_ptr<Background> background;
	std::unique_ptr<sf::Image> background_mask;
	std::vector<std::unique_ptr<Tower>> towers;

	float currentScale = 1.0f;
	float currentOffsetX = 0.f;

public:
	Level(int _id, const std::string& _name, const std::string& graphic_name, const AssetManager& assets) : id(_id), name(_name) {
		background = std::make_unique<Background>(assets.getTexture(graphic_name));
		try {
			if (_name != "menu") {
				background_mask = std::make_unique<sf::Image>(assets.getMask(graphic_name + "_mask"));
			}
		} catch (const std::out_of_range&) {
			std::clog << "No graphic such as " << graphic_name << "_mask\n";
			background_mask = nullptr;
		}
	}

	void updateLayout(const sf::Vector2u& windowSize) {
		background->updateLayout(windowSize);

		currentScale = static_cast<float>(windowSize.y) / 960.f;
		currentOffsetX = (static_cast<float>(windowSize.x) - 540.f * currentScale) / 2.f;
	}

	void draw(sf::RenderWindow& window) {
		background->draw(window);

		for (const auto& tower : towers) {
			const_cast<Tower*>(tower.get())->draw(window, currentScale, currentOffsetX);
		}
	}

	bool canPlaceTower(sf::Vector2i mousePos, sf::Vector2u windowSize) const {
		float scale = static_cast<float>(windowSize.y) / 960.f;

		float backgroundWidth = 540.f * scale;

		float offsetX = (static_cast<float>(windowSize.x) - backgroundWidth) / 2.f;

		int virtualX = static_cast<int>((mousePos.x - offsetX) / scale);
		int virtualY = static_cast<int>(mousePos.y / scale);

		if (virtualX < 0 || virtualX >= 540 || virtualY < 0 || virtualY >= 960) {
			return false;
		}

		return background_mask->getPixel({ static_cast<unsigned int>(virtualX), static_cast<unsigned int>(virtualY) }) == sf::Color::White;
	}

	bool canPlaceTower(sf::Vector2i mousePos, sf::Vector2u windowSize, int radius) const {
		float backgroundWidth = 540.f * currentScale;

		int centerX = static_cast<int>((mousePos.x - currentOffsetX) / currentScale);
		int centerY = static_cast<int>(mousePos.y / currentScale);

		for (int dy = -radius; dy <= radius; ++dy) {
			for (int dx = -radius; dx <= radius; ++dx) {
				if (dx * dx + dy * dy <= radius * radius) {
					int virtualX = centerX + dx;
					int virtualY = centerY + dy;

					if (virtualX < 0 || virtualX >= 540 || virtualY < 0 || virtualY >= 960) {
						return false;
					}

					sf::Color color = background_mask->getPixel({static_cast<unsigned int>(virtualX), static_cast<unsigned int>(virtualY)});

					if (color != sf::Color::White) {
						return false;
					}
				}
			}
		}

		struct {
			sf::Vector2f pos;
			float r;
			sf::Vector2f getVirtualPos() const { return pos; }
			float getRadius() const { return r; }
		} ghostHitbox = { { (float)centerX, (float)centerY }, (float)radius };

		for (const auto& tower : towers) {
			if (checkCollision(*tower, ghostHitbox)) {
				return false;
			}
		}

		return true;
	}

	void addTower(const Tower& tower) {
		towers.push_back(std::make_unique<Tower>(tower));
	}

	sf::Vector2f mapMouseToVirtual(sf::Vector2i mousePos, sf::Vector2u windowSize) {
		return { (mousePos.x - currentOffsetX) / currentScale , mousePos.y / currentScale };
	}

	float getCurrentScale() const {
		return currentScale;
	}

	float getCurrentOffsetX() const {
		return currentOffsetX;
	}
};
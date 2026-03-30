#pragma once

#include "asset_manager.h"
#include "background.h"

class MainMenu {
private:
	std::unique_ptr<Background> background;

public:
	MainMenu(const AssetManager& assets) {
		background = std::make_unique<Background>(assets.getTexture("menu_background"));
	}
	void updateLayout(const sf::Vector2u& windowSize) {
		background->updateLayout(windowSize);
	}
	void draw(sf::RenderWindow& window) const {
		background->draw(window);
	}
};
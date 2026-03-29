#pragma once
#include <memory>
#include "asset_manager.h"
#include "background.h"
#include "gamestate.h"
#include "leveldata.h"

class Game {
private:
	sf::RenderWindow window;
	AssetManager assetManager;
	std::unique_ptr<Background> background;
	GameState gameState;

	LevelData currentLevel;

public:
	Game() : window(sf::VideoMode({ 540, 960 }), "EvolutionTD"), gameState(GameState::MAIN_MENU) {
		assetManager.loadTextures("./assets/textures");
		background = std::make_unique<Background>(assetManager.getTexture("menu_background"));
		background->updateLayout(window.getSize());
	}

	void changeLevel(const LevelData& newLevel) {
		currentLevel = newLevel;

		background = std::make_unique<Background>(assetManager.getTexture(currentLevel.textureName));
		background->updateLayout(window.getSize());
	}

	void run() {
		while (window.isOpen()) {
			handleEvents();
			update();
			render();
		}
	}

private:
	void handleEvents() {
		while (auto event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (auto resized = event->getIf<sf::Event::Resized>()) {
				sf::FloatRect visibleArea({ 0, 0 }, {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)});
				window.setView(sf::View(visibleArea));
				background->updateLayout(event->getIf<sf::Event::Resized>()->size);
			}

			if (auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Num1 && gameState == GameState::MAIN_MENU) {
					changeLevel({ 1, "forest1" });
					gameState = GameState::PLAYING;
				}
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape && gameState == GameState::PLAYING) {
					changeLevel({ 0, "menu_background" });
					gameState = GameState::MAIN_MENU;
				}
			}
		}
	}

	void render() {
		window.clear(sf::Color(224, 224, 224));

		background->draw(window);

		window.display();
	}

	void update() {
		// Game logic updates would go here
	}
};
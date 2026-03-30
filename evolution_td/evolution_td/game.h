#pragma once
#include "level.h"
#include "main_menu.h"

class Game {
private:
	sf::RenderWindow window;
	AssetManager assetManager;
	std::unique_ptr<Level> currentLevel;
	std::unique_ptr<MainMenu> mainMenu;
	GameState gameState;

public:
	Game() : window(sf::VideoMode({ 540, 960 }), "EvolutionTD"), gameState(GameState::MAIN_MENU) {
		assetManager.loadTextures("./assets/textures");
		mainMenu = std::make_unique<MainMenu>(assetManager);
		mainMenu->updateLayout(window.getSize());
	}

	void changeLevel(int id, const std::string& name, const std::string& texture) {
		currentLevel = std::make_unique<Level>(id, name, texture, assetManager);

		currentLevel->updateLayout(window.getSize());
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
				if (gameState == GameState::MAIN_MENU) {
					mainMenu->updateLayout(resized->size);
				}
				else if (gameState == GameState::PLAYING) {
					currentLevel->updateLayout(resized->size);
				}
			}

			if (auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Num1 && gameState == GameState::MAIN_MENU) {
					changeLevel(1, "forest1", "forest1");
					gameState = GameState::PLAYING;
				}
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape && gameState == GameState::PLAYING) {
					gameState = GameState::MAIN_MENU;
				}
			}

			if (auto mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mousePressed->button == sf::Mouse::Button::Left && gameState == GameState::PLAYING) {
					if (currentLevel->canPlaceTower(mousePressed->position, window.getSize())) {
						std::clog << "Can place tower at " << mousePressed->position.x << ", " << mousePressed->position.y << "\n";
					}
					else {
						std::clog << "Cannot place tower at " << mousePressed->position.x << ", " << mousePressed->position.y << "\n";
					}
					if (currentLevel->canPlaceTower(mousePressed->position, window.getSize(), 3)) {
						std::clog << "Can place tower with radius 3 at " << mousePressed->position.x << ", " << mousePressed->position.y << "\n";
					}
					else {
						std::clog << "Cannot place tower with radius 3 at " << mousePressed->position.x << ", " << mousePressed->position.y << "\n";
					}
				}
			}
		}
	}

	void render() {
		window.clear(sf::Color(224, 224, 224));

		if (gameState == GameState::MAIN_MENU) {
			mainMenu->draw(window);
		}
		else if (gameState == GameState::PLAYING) {
			currentLevel->draw(window);
		}

		window.display();
	}

	void update() {
		// Game logic updates would go here
	}
};
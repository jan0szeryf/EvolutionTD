#pragma once
#include "level.h"
#include "main_menu.h"
#include "ingamegui.h"

class Game {
private:
	sf::RenderWindow window;
	AssetManager assetManager;
	std::unique_ptr<Level> currentLevel;
	std::unique_ptr<MainMenu> mainMenu;
	std::unique_ptr<InGameGUI> gui;
	GameState gameState;

	std::unique_ptr<Tower> pendingTower;

public:
	Game() : window(sf::VideoMode({ 540, 960 }), "EvolutionTD"), gameState(GameState::MAIN_MENU) {
		assetManager.loadTextures("./assets");

		mainMenu = std::make_unique<MainMenu>(assetManager);
		mainMenu->updateLayout(window.getSize());

		gui = std::make_unique<InGameGUI>(assetManager, assetManager.getFont("LilitaOne"), [this]() {
			if (this->gameState == GameState::PLAYING) {
				//this->gameState = GameState::PAUSED;
				std::cout << "Game paused\n";
			}
			else if (this->gameState == GameState::PAUSED) {
				//this->gameState = GameState::PLAYING;
				std::cout << "Game unpaused\n";
			}
			});
		gui->updateLayout(1.f, 1.f, window.getSize());
		gui->addTowerButton(assetManager.getTexture("thrower_stance"), assetManager.getFont("LilitaOne"), "", [this]() {
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			this->pendingTower = std::make_unique<Tower>(static_cast<sf::Vector2f>(mousePos), 10, 120.f, 1.f, 40.f, 100, assetManager.getTexture("thrower_stance"), assetManager.getTexture("thrower_projectile"));
			gui->toggleShop();
			});
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
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);

			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Num1 && gameState == GameState::MAIN_MENU) {
					changeLevel(1, "forest1", "forest1");
					gameState = GameState::PLAYING;

					gui->updateLayout(currentLevel->getCurrentScale(), currentLevel->getCurrentOffsetX(), window.getSize());
				}
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape && gameState == GameState::PLAYING) {
					currentLevel.reset();
					gameState = GameState::MAIN_MENU;
					gui->reset();
					mainMenu->updateLayout(window.getSize());
				}
			}

			if (auto resized = event->getIf<sf::Event::Resized>()) {
				sf::FloatRect visibleArea({ 0, 0 }, {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)});
				window.setView(sf::View(visibleArea));
				if (gameState == GameState::MAIN_MENU) {
					mainMenu->updateLayout(resized->size);
				}
				else if (gameState == GameState::PLAYING) {
					currentLevel->updateLayout(resized->size);
					gui->updateLayout(currentLevel->getCurrentScale(), currentLevel->getCurrentOffsetX(), resized->size);
				}
			}

			if (auto mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
				if (gameState == GameState::PLAYING && pendingTower) {
					sf::Vector2f virtualPos = currentLevel->mapMouseToVirtual(mouseMoved->position, window.getSize());
					pendingTower->setVirtualPos(virtualPos);

					if (currentLevel->canPlaceTower(mouseMoved->position, window.getSize(), static_cast<int>(pendingTower->getRadius()))) {
						pendingTower->setColor(sf::Color(255, 255, 255, 200));
					}
					else {
						pendingTower->setColor(sf::Color(255, 100, 100, 200));
					}
				}
			}

			if (gameState == GameState::PLAYING) {
				if (gui->handleEvent(*event, mousePos)) {
					continue;
				}
			}

			if (auto mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mousePressed->button == sf::Mouse::Button::Left && gameState == GameState::PLAYING) {
					float towerRadius = 32.f;
					if (pendingTower && currentLevel->canPlaceTower(mousePressed->position, window.getSize(), static_cast<int>(towerRadius))) {
						pendingTower->setColor(sf::Color(255, 255, 255, 255));
						currentLevel->addTower(*pendingTower);
					}
					pendingTower.reset();
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
			float scale = currentLevel->getCurrentScale();
			float offsetX = currentLevel->getCurrentOffsetX();

			currentLevel->draw(window);

			gui->draw(window);

			if (pendingTower)
			{
				pendingTower->drawRadius(window, scale, offsetX);
				pendingTower->drawRange(window, scale, offsetX);
				pendingTower->draw(window, scale, offsetX);
			}
		}

		window.display();
	}

	void update() {
		// Game logic updates would go here
	}
};
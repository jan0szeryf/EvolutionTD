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
	sf::Clock clock;

	std::unique_ptr<Tower> pendingTower;

public:
	Game() : window(sf::VideoMode({ 540, 960 }), "EvolutionTD"), gameState(GameState::MAIN_MENU) {
		assetManager.loadTextures("./assets");

		mainMenu = std::make_unique<MainMenu>(assetManager);
		mainMenu->updateLayout(window.getSize());
		mainMenu->addButton(assetManager.getTexture("play_button"), assetManager.getFont("LilitaOne"), "", [this]() {
			changeLevel(1, "forest1", "forest1");
			gameState = GameState::PLAYING;

			gui->updateLayout(currentLevel->getCurrentScale(), currentLevel->getCurrentOffsetX(), window.getSize());
			std::cout << "State changed to PLAYING (forest1)\n";
		});
		mainMenu->updateLayout(window.getSize());

		gui = std::make_unique<InGameGUI>(assetManager, assetManager.getFont("LilitaOne"), [&]() {
			if (this->gameState == GameState::PLAYING) {
				this->gameState = GameState::PAUSED;
				std::cout << "State changed to PAUSED\n";
			}
			else if (this->gameState == GameState::PAUSED) {
				this->gameState = GameState::PLAYING;
				std::cout << "State changed to PLAYING\n";
			}
			});
		gui->updateLayout(1.f, 1.f, window.getSize());
		gui->addTowerButton(assetManager.getTexture("thrower_stance"), assetManager.getFont("LilitaOne"), "50", [this]() {
			if (currentLevel->getPlayerStats().getGold() < 50) {
				std::cout << "Not enough gold to buy thrower tower\n";
				return;
			}
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f virtualPos = currentLevel->mapMouseToVirtual(mousePos, window.getSize());
			this->pendingTower = std::make_unique<Tower>("thrower", virtualPos, 10, 120.f, 1.f, 40.f, 50, assetManager.getTexture("thrower_stance"), assetManager.getTexture("thrower_projectile"));
			gui->toggleShop();
			std::cout << "Started placing thrower tower\n";
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
					std::cout << "State changed to PLAYING (forest1)\n";
				}
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape && (gameState == GameState::PLAYING || gameState == GameState::GAME_OVER)) {
					currentLevel.reset();
					gameState = GameState::MAIN_MENU;
					gui->reset();
					mainMenu->updateLayout(window.getSize());
					std::cout << "State changed to MAIN MENU\n";
				}
				if (keyPressed->scancode == sf::Keyboard::Scancode::Enter && gameState == GameState::PLAYING) {
					currentLevel->nextWave();
				}
			}

			if (auto resized = event->getIf<sf::Event::Resized>()) {
				sf::FloatRect visibleArea({ 0, 0 }, {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)});
				window.setView(sf::View(visibleArea));
				if (gameState == GameState::MAIN_MENU) {
					mainMenu->updateLayout(resized->size);
				}
				else {
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

			if (gameState == GameState::PLAYING || gameState == GameState::PAUSED) {
				if (gui->handleEvent(*event, mousePos, gameState == GameState::PAUSED)) {
					continue;
				}
			}

			if (gameState == GameState::MAIN_MENU) {
				if (mainMenu->handleEvent(*event, mousePos)) {
					continue;
				}
			}

			if (auto mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mousePressed->button == sf::Mouse::Button::Left && gameState == GameState::PLAYING) {
					if (pendingTower && currentLevel->canPlaceTower(mousePressed->position, window.getSize(), static_cast<int>(pendingTower->getRadius()))) {
						pendingTower->setColor(sf::Color(255, 255, 255, 255));
						currentLevel->addTower(*pendingTower);
						std::cout << "Placed tower " << pendingTower->getName() <<"at virtual position: (" << pendingTower->getVirtualPos().x << ", " << pendingTower->getVirtualPos().y << ")\n";
						currentLevel->getPlayerStats().spendGold(pendingTower->getCost());
						pendingTower.reset();
					}
					if (pendingTower && !currentLevel->canPlaceTower(mousePressed->position, window.getSize(), static_cast<int>(pendingTower->getRadius()))) {
						std::cout << "Unable to place tower at virtual position: (" << pendingTower->getVirtualPos().x << ", " << pendingTower->getVirtualPos().y << ")\n";
					}
				}
				if (mousePressed->button == sf::Mouse::Button::Right && pendingTower) {
					pendingTower.reset();
					std::cout << "Cancelled tower placement\n";
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
		else if (gameState == GameState::PAUSED) {
			currentLevel->draw(window);
			gui->draw(window);
			sf::RectangleShape overlay({ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) });
			overlay.setFillColor(sf::Color(0, 0, 0, 150));
			window.draw(overlay);
		}
		else if (gameState == GameState::GAME_OVER) {
			currentLevel->draw(window);
			gui->drawGameOver(window);
		}
		else if (gameState == GameState::VICTORY) {
			currentLevel->draw(window);
			gui->drawVictory(window);
		}

		window.display();
	}

	void update() {
		if(gameState == GameState::PLAYING) {
			float deltaTime = clock.restart().asSeconds();
			if(currentLevel) {
				if(currentLevel->update(deltaTime)) {
					gui->reset();
					std::cout << "Victory! Final Score: " << currentLevel->getPlayerStats().getScore() << "\n";
					gameState = GameState::VICTORY;
					std::cout << "State changed to VICTORY\n";
				}

				if (currentLevel->getPlayerStats().getHp() <= 0) {
					gui->reset();
					std::cout << "Game Over! Final Score: " << currentLevel->getPlayerStats().getScore() << "\n";
					gameState = GameState::GAME_OVER;
					std::cout << "State changed to GAME OVER\n";
				}
				gui->updateStats(currentLevel->getPlayerStats());
			}
		}
		else {
			clock.restart();
		}
	}
};
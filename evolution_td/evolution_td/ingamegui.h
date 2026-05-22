#pragma once
#include "button.h"
#include "asset_manager.h"
#include "player_stats.h"

#include <SFML/Graphics.hpp>
#include <vector>

class InGameGUI {
private:
	bool isShopOpen = false;
	sf::Vector2u lastWindowSize = {540, 960};
	float lastScale = 1.f;
	float lastOffsetX = 0.f;

	std::unique_ptr<Button> shopButton;
	std::unique_ptr<Button> pauseButton;
	std::unique_ptr<sf::Sprite> healthSprite;
	std::unique_ptr<sf::Sprite> goldSprite;
	sf::Text healthText;
	sf::Text goldText;

	sf::RectangleShape shopPanel;
	std::vector<std::unique_ptr<Button>> towerButtons;

	void updateShopPanel() {
		float h = static_cast<float>(lastWindowSize.y);
		float panelHeight = 300.f * lastScale;
		float panelWidth = 540.f * lastScale;

		shopPanel.setSize({ panelWidth, panelHeight });
		shopPanel.setPosition({ lastOffsetX, h - panelHeight });
	}

	void updateControlButtons() {
		float h = static_cast<float>(lastWindowSize.y);
		float buttonSize = 60.f * lastScale;

		shopButton->setSize({ buttonSize, buttonSize });
		pauseButton->setSize({ buttonSize, buttonSize });
		
		float shopBtnX = lastOffsetX + (460.f * lastScale);
		float shopBtnY = h - (80.f * lastScale);

		if (isShopOpen) {
			shopBtnY -= (300.f * lastScale);
		}
		shopButton->setPosition({ shopBtnX, shopBtnY });

		float pauseBtnX = lastOffsetX + (460.f * lastScale);
		float pauseBtnY = 20.f * lastScale;
		pauseButton->setPosition({ pauseBtnX, pauseBtnY });
	}

	void updateStatsLayout() {
		float iconSize = 40.f * lastScale;
		float goldY = 100.f * lastScale;
		float goldX = lastOffsetX + (20.f * lastScale);
		float healthY = 160.f * lastScale;
		float healthX = lastOffsetX + (20.f * lastScale);
		unsigned int fontSize = static_cast<unsigned int>(20.f * lastScale);

		healthSprite->setPosition({ healthX, healthY });
		float healthWidth = healthSprite->getLocalBounds().size.x;
		float healthHeight = healthSprite->getLocalBounds().size.y;
		if (healthWidth > 0 && healthHeight > 0) {
			healthSprite->setScale({ iconSize / healthWidth, iconSize / healthHeight });
		}
		healthText.setCharacterSize(fontSize);
		healthText.setOutlineColor(sf::Color::Black);
		healthText.setOutlineThickness(2.f * lastScale);
		healthText.setPosition({ healthX + iconSize + (10.f * lastScale), healthY + (5.f * lastScale) });

		goldSprite->setPosition({ goldX, goldY });
		float goldWidth = goldSprite->getLocalBounds().size.x;
		float goldHeight = goldSprite->getLocalBounds().size.y;
		if (goldWidth > 0 && goldHeight > 0) {
			goldSprite->setScale({ iconSize / goldWidth, iconSize / goldHeight });
		}
		goldText.setCharacterSize(fontSize);
		goldText.setOutlineColor(sf::Color::Black);
		goldText.setOutlineThickness(2.f * lastScale);
		goldText.setPosition({ goldX + iconSize + (10.f * lastScale), goldY + (5.f * lastScale) });
	}

	void updateTowerButtonsLayout() {
		float panelHeight = 300.f * lastScale;
		float panelWidth = 540.f * lastScale;
		float towerButtonSize = 80.f * lastScale;
		float startX = lastOffsetX + (20.f * lastScale);
		float startY = static_cast<float>(lastWindowSize.y) - panelHeight + (20.f * lastScale);
		float padding = 15.f * lastScale;
		for (size_t i = 0; i < towerButtons.size(); ++i) {
			float x = startX + i * (towerButtonSize + padding);
			float y = startY;
			towerButtons[i]->setPosition({ x, y });
			towerButtons[i]->setSize({ towerButtonSize, towerButtonSize });
		}
	}

public:
	InGameGUI(const AssetManager& assets, const sf::Font& font, std::function<void()> onPauseCallback) : healthText(font), goldText(font) {
		shopButton = std::make_unique<Button>(assets.getTexture("shop_icon"), sf::Vector2f(460.f, 880.f), font, "", [this]() {
			this->toggleShop();
		});
		
		pauseButton = std::make_unique<Button>(assets.getTexture("pause_icon"), sf::Vector2f(460.f, 20.f), font, "", onPauseCallback);
		healthSprite = std::make_unique<sf::Sprite>(assets.getTexture("health_icon"));
		goldSprite = std::make_unique<sf::Sprite>(assets.getTexture("gold_icon"));

		shopPanel.setFillColor(sf::Color(50, 50, 50, 200));

		healthText.setFont(font);
		healthText.setCharacterSize(18);
		healthText.setFillColor(sf::Color::White);
		goldText.setFont(font);
		goldText.setCharacterSize(18);
		goldText.setFillColor(sf::Color::White);
	}

	void toggleShop() {
		isShopOpen = !isShopOpen;
		if (isShopOpen) {
			std::cout << "Shop opened\n";
		} else {
			std::cout << "Shop closed\n";
		}
		updateLayout(lastScale, lastOffsetX, lastWindowSize);
	}

	void reset() {
		isShopOpen = false;
		updateLayout(lastScale, lastOffsetX, lastWindowSize);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(*healthSprite);
		window.draw(*goldSprite);
		window.draw(healthText);
		window.draw(goldText);

		if (isShopOpen) {
			window.draw(shopPanel);
			for (const auto& button : towerButtons) {
				button->draw(window);
			}
		}

		shopButton->draw(window);
		pauseButton->draw(window);
	}

	void drawGameOver(sf::RenderWindow& window) {
		sf::Text gameOverText(goldText.getFont());
		gameOverText.setString("Game Over");
		gameOverText.setCharacterSize(static_cast<unsigned int>(50.f * lastScale));
		gameOverText.setFillColor(sf::Color::Red);
		gameOverText.setOutlineColor(sf::Color::Black);
		gameOverText.setOutlineThickness(3.f * lastScale);	

		sf::Vector2u windowSize = window.getSize();
		sf::RectangleShape background(sf::Vector2f(windowSize.x, windowSize.y));
		background.setFillColor(sf::Color(0, 0, 0, 150));
		gameOverText.setOrigin({ gameOverText.getLocalBounds().size.x / 2.f, gameOverText.getLocalBounds().size.y / 2.f });
		gameOverText.setPosition({ lastOffsetX + (windowSize.x / 2.f), windowSize.y / 2.4f });

		window.draw(background);
		window.draw(gameOverText);
	}

	void updateLayout(float scale, float offsetX, const sf::Vector2u windowSize) {
		lastScale = scale;
		lastOffsetX = offsetX;
		lastWindowSize = windowSize;

		updateShopPanel();
		updateControlButtons();
		updateTowerButtonsLayout();
		updateStatsLayout();
	}

	bool handleEvent(const sf::Event& event, sf::Vector2i mousePos, bool isPaused) {
		if (shopButton->isClicked(mousePos)) {
			if (event.is<sf::Event::MouseButtonPressed>()) {
				if (isPaused) {
					return false;
				}
				shopButton->execute();
				return true;
			}
		}

		if (pauseButton->isClicked(mousePos)) {
			if (event.is<sf::Event::MouseButtonPressed>()) {
				pauseButton->execute();
				return true;
			}
		}

		if (isShopOpen) {
			if(shopPanel.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
				for (const auto& button : towerButtons) {
					if (button->isClicked(mousePos)) {
						if (event.is<sf::Event::MouseButtonPressed>()) {
							if (isPaused) {
								return false;
							}
							button->execute();
							return true;
						}
					}
				}
				return true;
			}
		}

		return false;
	}

	void addTowerButton(const sf::Texture& texture, const sf::Font& font, std::string text, std::function<void()> callback) {
		float buttonSize = 60.f * lastScale;
		float x = shopPanel.getPosition().x + 20.f * lastScale;
		float y = shopPanel.getPosition().y + 20.f * lastScale + towerButtons.size() * (buttonSize + 10.f * lastScale);
		auto button = std::make_unique<Button>(texture, sf::Vector2f(x, y), font, text, callback);
		button->setSize({ buttonSize, buttonSize });
		towerButtons.push_back(std::move(button));
	}

	void updateStats(const PlayerStats& stats) {
		goldText.setString(std::to_string(stats.getMoney()));
		healthText.setString(std::to_string(stats.getHp()));
	}
};
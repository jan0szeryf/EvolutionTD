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
	int currentWave = 0;

	std::unique_ptr<Button> shopButton;
	std::unique_ptr<Button> pauseButton;
	std::unique_ptr<Button> startWaveButton;
	std::unique_ptr<sf::Sprite> healthSprite;
	std::unique_ptr<sf::Sprite> goldSprite;
	sf::Text healthText;
	sf::Text goldText;
	sf::Text waveText;

	sf::RectangleShape waveBackground;
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
		startWaveButton->setSize({ buttonSize, buttonSize });
		
		float shopBtnX = lastOffsetX + (460.f * lastScale);
		float shopBtnY = h - (80.f * lastScale);

		if (isShopOpen) {
			shopBtnY -= (300.f * lastScale);
		}
		shopButton->setPosition({ shopBtnX, shopBtnY });

		float pauseBtnX = lastOffsetX + (460.f * lastScale);
		float pauseBtnY = 20.f * lastScale;
		pauseButton->setPosition({ pauseBtnX, pauseBtnY });

		float startWaveBtnX = lastOffsetX + (20.f * lastScale);
		float startWaveBtnY = 20.f * lastScale;
		startWaveButton->setPosition({ startWaveBtnX, startWaveBtnY });
		waveBackground.setSize({ 160.f * lastScale, 30 * lastScale });
		waveBackground.setPosition({ (startWaveBtnX + buttonSize / 2.f), (startWaveBtnY + buttonSize / 4.f) });
		waveText.setCharacterSize(static_cast<unsigned int>(18.f * lastScale));
		waveText.setOutlineThickness(2.f * lastScale);
		waveText.setString("Wave " + std::to_string(currentWave));
		waveText.setPosition({ (startWaveBtnX + buttonSize * 1.5f), (startWaveBtnY + buttonSize / 3.5f) });
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
		healthText.setOutlineThickness(2.f * lastScale);
		healthText.setPosition({ healthX + iconSize + (10.f * lastScale), healthY + (5.f * lastScale) });

		goldSprite->setPosition({ goldX, goldY });
		float goldWidth = goldSprite->getLocalBounds().size.x;
		float goldHeight = goldSprite->getLocalBounds().size.y;
		if (goldWidth > 0 && goldHeight > 0) {
			goldSprite->setScale({ iconSize / goldWidth, iconSize / goldHeight });
		}
		goldText.setCharacterSize(fontSize);
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
	InGameGUI(const AssetManager& assets, const sf::Font& font, std::function<void()> onPauseCallback, std::function<void()> nextWaveCallback) : healthText(font), goldText(font), waveText(font) {
		shopButton = std::make_unique<Button>(assets.getTexture("shop_icon"), sf::Vector2f(460.f, 880.f), font, "", [this]() {
			this->toggleShop();
		});
		
		pauseButton = std::make_unique<Button>(assets.getTexture("pause_icon"), sf::Vector2f(460.f, 20.f), font, "", onPauseCallback);
		startWaveButton = std::make_unique<Button>(assets.getTexture("nextWave_icon"), sf::Vector2f(20.f, 20.f), font, "", nextWaveCallback);
		healthSprite = std::make_unique<sf::Sprite>(assets.getTexture("health_icon"));
		goldSprite = std::make_unique<sf::Sprite>(assets.getTexture("gold_icon"));

		shopPanel.setFillColor(sf::Color(50, 50, 50, 200));
		waveBackground.setFillColor(sf::Color(0, 0, 0, 150));
		waveBackground.setOrigin({ 0, waveBackground.getLocalBounds().size.y / 2.f });

		healthText.setFont(font);
		healthText.setFillColor(sf::Color::White);
		goldText.setOutlineColor(sf::Color::Black);
		goldText.setFont(font);
		goldText.setFillColor(sf::Color::White);
		goldText.setOutlineColor(sf::Color::Black);
		waveText.setFont(font);
		waveText.setFillColor(sf::Color::White);
		goldText.setOutlineColor(sf::Color::Black);
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
		window.draw(waveBackground);
		window.draw(waveText);
		startWaveButton->draw(window);
	}

	void drawGameOver(sf::RenderWindow& window) {
		sf::Text gameOverText(goldText.getFont());
		gameOverText.setString("Game Over");
		gameOverText.setCharacterSize(static_cast<unsigned int>(50.f * lastScale));
		gameOverText.setFillColor(sf::Color::Red);
		gameOverText.setOutlineColor(sf::Color::Black);
		gameOverText.setOutlineThickness(3.f * lastScale);	

		sf::Vector2u windowSize = window.getSize();
		sf::RectangleShape background({ static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) });
		background.setFillColor(sf::Color(0, 0, 0, 150));
		gameOverText.setOrigin({ gameOverText.getLocalBounds().size.x / 2.f, gameOverText.getLocalBounds().size.y / 2.f });
		gameOverText.setPosition({ lastOffsetX + (windowSize.x / 2.f), windowSize.y / 2.4f });

		window.draw(background);
		window.draw(gameOverText);
	}

	void drawVictory(sf::RenderWindow& window) {
		sf::Text victoryText(goldText.getFont());
		victoryText.setString("Victory!");
		victoryText.setCharacterSize(static_cast<unsigned int>(50.f * lastScale));
		victoryText.setFillColor(sf::Color::Green);
		victoryText.setOutlineColor(sf::Color::Black);
		victoryText.setOutlineThickness(3.f * lastScale);

		sf::Vector2u windowSize = window.getSize();
		sf::RectangleShape background({ static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) });
		background.setFillColor(sf::Color(0, 0, 0, 150));
		victoryText.setOrigin({ victoryText.getLocalBounds().size.x / 2.f, victoryText.getLocalBounds().size.y / 2.f });
		victoryText.setPosition({ lastOffsetX + (windowSize.x / 2.f), windowSize.y / 2.4f });

		window.draw(background);
		window.draw(victoryText);
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

		if (startWaveButton->isClicked(mousePos)) {
			if (event.is<sf::Event::MouseButtonPressed>()) {
				if (isPaused) {
					return false;
				}
				startWaveButton->execute();
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
		goldText.setString(std::to_string(stats.getGold()));
		healthText.setString(std::to_string(stats.getHp()));
	}

	void setCurrentWave(int wave) {
		currentWave = wave;
		updateControlButtons();
	}
};
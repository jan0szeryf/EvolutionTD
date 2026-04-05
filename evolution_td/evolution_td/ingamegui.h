#pragma once
#include "button.h"
#include "asset_manager.h"

#include <SFML/Graphics.hpp>
#include <vector>

class InGameGUI {
private:
	bool isShopOpen = false;
	sf::Vector2u lastWindowSize = {540, 960};
	float lastScale = 1.f;
	float lastOffsetX = 0.f;

	std::unique_ptr<Button> shopButton;
	sf::Text statsText;

	sf::RectangleShape shopPanel;
	std::vector<std::unique_ptr<Button>> towerButtons;

public:
	InGameGUI(const AssetManager& assets, const sf::Font& font) : statsText(font) {
		shopButton = std::make_unique<Button>(assets.getTexture("shop_icon"), sf::Vector2f(460.f, 880.f), font, "", [this]() {
			this->toggleShop();
		});

		shopPanel.setFillColor(sf::Color(50, 50, 50, 200));

		statsText.setFont(font);
		statsText.setCharacterSize(18);
		statsText.setFillColor(sf::Color::White);
	}

	void toggleShop() {
		isShopOpen = !isShopOpen;
		updateLayout(lastScale, lastOffsetX, lastWindowSize);
	}

	void reset() {
		isShopOpen = false;
		updateLayout(lastScale, lastOffsetX, lastWindowSize);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(statsText);

		if (isShopOpen) {
			window.draw(shopPanel);
			for (const auto& button : towerButtons) {
				button->draw(window);
			}
		}

		shopButton->draw(window);
	}

	void updateLayout(float scale, float offsetX, const sf::Vector2u windowSize) {
		lastScale = scale;
		lastOffsetX = offsetX;
		lastWindowSize = windowSize;

		float w = static_cast<float>(windowSize.x);
		float h = static_cast<float>(windowSize.y);

		float panelHeight = 300.f * scale;
		float panelWidth = 540.f * scale;

		shopPanel.setSize({ panelWidth, panelHeight });
		shopPanel.setPosition({ offsetX, h - panelHeight });

		float buttonSize = 60.f * scale;
		shopButton->setSize({ buttonSize, buttonSize });

		float btnX = offsetX + (460.f * scale);
		float btnY = h - (80.f * scale);

		if (isShopOpen) {
			btnY -= (300.f * scale);
		}

		shopButton->setPosition({ btnX, btnY });

		statsText.setCharacterSize(static_cast<unsigned int>(24.f * scale));
		statsText.setPosition({ offsetX + (20.f * scale), 20.f * scale });

		float towerButtonSize = 80.f * scale;
		float startX = offsetX + (20.f * scale);
		float startY = h - panelHeight + (20.f * scale);
		float padding = 15.f * scale;

		for (size_t i = 0; i < towerButtons.size(); ++i) {
			float x = startX + i * (buttonSize + padding);
			float y = startY;

			towerButtons[i]->setPosition({ x, y });
			towerButtons[i]->setSize({ towerButtonSize, towerButtonSize });
		}
	}

	bool handleEvent(const sf::Event& event, sf::Vector2i mousePos) {
		if (shopButton->isClicked(mousePos)) {
			if (event.is<sf::Event::MouseButtonPressed>()) {
				shopButton->execute();
				return true;
			}
		}

		if (isShopOpen) {
			if(shopPanel.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
				for (const auto& button : towerButtons) {
					if (button->isClicked(mousePos)) {
						if (event.is<sf::Event::MouseButtonPressed>()) {
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
};
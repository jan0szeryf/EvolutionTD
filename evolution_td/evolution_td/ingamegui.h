#pragma once
#include "button.h"
#include "asset_manager.h"

#include <SFML/Graphics.hpp>
#include <vector>

class InGameGUI {
private:
	bool isShopOpen = false;

	std::unique_ptr<Button> shopButton;
	sf::Text statsText;

	sf::RectangleShape shopPanel;
	std::vector<std::unique_ptr<Button>> towerButtons;

public:
	InGameGUI(const AssetManager& assets, const sf::Font& font) : statsText(font) {
		shopButton = std::make_unique<Button>(sf::Vector2f(60.f, 60.f), sf::Vector2f(460.f, 880.f), sf::Color(150, 150, 150), font, "Shop", [this]() {
			this->toggleShop();
			});

		shopPanel.setSize({ 540.f, 300.f });
		shopPanel.setPosition({ 0.f, 660.f });
		shopPanel.setFillColor(sf::Color(50, 50, 50, 200));
	}

	void toggleShop() {
		isShopOpen = !isShopOpen;
		if (!isShopOpen) {
			shopButton->setPosition({ 460.f, 880.f });
		} else {
			shopButton->setPosition({ 460.f, 880.f - shopPanel.getSize().y});
		}
	}

	void draw(sf::RenderWindow& window) {
		shopButton->draw(window);
		window.draw(statsText);

		if (isShopOpen) {
			window.draw(shopPanel);
			for (const auto& button : towerButtons) {
				button->draw(window);
			}
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
			for (const auto& button : towerButtons) {
				if (button->isClicked(mousePos)) {
					if (event.is<sf::Event::MouseButtonPressed>()) {
						button->execute();
						return true;
					}
				}
			}
		}

		if (shopPanel.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
			return true;
		}

		return false;
	}
};
#pragma once

#include "asset_manager.h"
#include "background.h"
#include "button.h"

class MainMenu {
private:
	std::unique_ptr<Background> background;
	std::vector<std::unique_ptr<Button>> buttons;
	std::unique_ptr<sf::Sprite> titleSprite;

	void updateTitleSprite(const sf::Vector2u& windowSize) {
		float scale = static_cast<float>(windowSize.y) / 960.f;
		titleSprite->setScale({ 400.f / titleSprite->getLocalBounds().size.x * scale , 80.f / titleSprite->getLocalBounds().size.y * scale });
		sf::FloatRect titleBounds = titleSprite->getGlobalBounds();
		titleSprite->setPosition({ (windowSize.x - titleBounds.size.x) / 2.f, windowSize.y / 6.f });
	}

	void updateButtons(const sf::Vector2u& windowSize) {
		float scale = static_cast<float>(windowSize.y) / 960.f;
		float buttonWidth = 270.f * scale;
		float buttonHeight = 120.f * scale;
		unsigned int fontSize = static_cast<unsigned int>(30.f * scale);
		float spacing = 20.f * scale;

		float startY = static_cast<float>(windowSize.y) / 3.6f;

		for (std::size_t i = 0; i < buttons.size(); ++i) {
			float x = (windowSize.x - buttonWidth) / 2.f;
			float y = startY + i * (buttonHeight + spacing);
			buttons[i]->setPosition({ x, y });
			buttons[i]->setSize({ buttonWidth, buttonHeight }, fontSize);
		}
	}
	
public:
	MainMenu(const AssetManager& assets) {
		background = std::make_unique<Background>(assets.getTexture("menu_background"));
		titleSprite = std::make_unique<sf::Sprite>(assets.getTexture("logo"));
	}

	void updateLayout(const sf::Vector2u& windowSize) {
		background->updateLayout(windowSize);
		updateButtons(windowSize);
		updateTitleSprite(windowSize);
	}

	void draw(sf::RenderWindow& window) const {
		background->draw(window);
		window.draw(*titleSprite);

		for (const auto& button : buttons) {
			button->draw(window);
		}
	}

	void addButton(const sf::Texture& texture, const sf::Font& font, std::string text, std::function<void()> callback) {
		auto button = std::make_unique<Button>(texture, sf::Vector2f(0.f, 0.f), font, text, callback);
		buttons.push_back(std::move(button));
	}

	bool handleEvent(const sf::Event& event, sf::Vector2i mousePos) {
		for (const auto& button : buttons) {
			if (button->isClicked(mousePos)) {
				if (auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
					if (mouseEvent->button == sf::Mouse::Button::Left) {
						button->execute();
						return true;
					}
				}
			}
		}
		return false;
	}
};
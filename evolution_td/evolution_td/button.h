#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
private:
	sf::Sprite sprite;
	sf::Text label;
	std::function<void()> onClick;

	void updateLabelPosition() {
		sf::FloatRect textBounds = label.getLocalBounds();
		label.setOrigin({
			textBounds.position.x + textBounds.size.x / 2.f,
			textBounds.position.y + textBounds.size.y / 2.f
			});

		sf::FloatRect spriteBounds = sprite.getGlobalBounds();
		label.setPosition({
			spriteBounds.position.x + spriteBounds.size.x / 2.f,
			spriteBounds.position.y + spriteBounds.size.y / 2.f
			});
	}

public:
    Button(const sf::Texture& texture, sf::Vector2f pos, const sf::Font& font, std::string text, std::function<void()> callback) : sprite(texture), label(font, text), onClick(callback) {
		sprite.setPosition(pos);

        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(18);
        label.setFillColor(sf::Color::White);
		updateLabelPosition();
    }

	void draw(sf::RenderWindow& window) {
		window.draw(sprite);
		if (!label.getString().isEmpty()) {
			window.draw(label);
		}
	}

	bool isClicked(sf::Vector2i mousePos) {
		return sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
	}

	void execute() {
		if (onClick) {
			onClick();
		}
	}

	void setPosition(sf::Vector2f pos) {
		sprite.setPosition(pos);
		updateLabelPosition();
	}
	
	void setSize(sf::Vector2f size, unsigned int fontSize = 18) {
		sf::FloatRect bounds = sprite.getLocalBounds();
		sprite.setScale({ size.x / bounds.size.x, size.y / bounds.size.y });

		label.setCharacterSize(fontSize);
		updateLabelPosition();
	}
};
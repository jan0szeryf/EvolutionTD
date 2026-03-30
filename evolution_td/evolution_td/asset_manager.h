#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <unordered_map>
#include <string>
#include <iostream>

class AssetManager {
private:
	std::unordered_map<std::string, sf::Texture> textures;
	std::unordered_map<std::string, sf::Image> masks;

public:
	AssetManager() = default;

	void loadTextures(const std::filesystem::path& path) {
		if (!std::filesystem::exists(path)) {
			std::cerr << "Texture directory does not exist: " << path << std::endl;
			return;
		}

		for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
			if (entry.is_regular_file() && entry.path().extension() == ".png") {
				sf::Texture texture;
				if (texture.loadFromFile(entry.path().string())) {
					texture.setSmooth(true);
					std::string name = entry.path().stem().string();
					if(!(name.find("_mask") != std::string::npos)) {
						textures[name] = std::move(texture);
						std::clog << "Loaded texture: " << name << std::endl;
					}
					else {
						masks[name] = std::move(texture.copyToImage());
						std::clog << "Loaded image: " << name << std::endl;
					}
				}
			}
		}
	}

	const sf::Texture& getTexture(const std::string& name) const {
		return textures.at(name);
	}

	const sf::Image& getMask(const std::string& name) const {
		return masks.at(name);
	}
};
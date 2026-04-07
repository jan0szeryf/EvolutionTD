classDiagram
    class AssetManager {
      - textures : unordered_map<string,sf::Texture>
      - masks : unordered_map<string,sf::Image>
      - fonts : unordered_map<string,sf::Font>
      + loadTextures(path)
      + getTexture(name)
      + getMask(name)
      + getFont(name)
    }

    class Background {
      - sprite : sf::Sprite
      - originalSize : sf::Vector2f
      + updateLayout(windowSize)
      + draw(window)
    }

    class Button {
      - sprite : sf::Sprite
      - label : sf::Text
      - onClick : function
      + isClicked(mousePos)
      + execute()
      + setPosition(pos)
      + setSize(size)
    }

    class Enemy {
      - hp : int
      - speed : float
      - radius : float
      - sprite : sf::Sprite
      + move(deltaTime)
      + updateLayout(scale, offsetX)
      + draw(target)
    }

    class FastEnemy
    class TankEnemy

    class Projectile {
      - speed : float
      - damage : float
      - target : shared_ptr<Enemy>
      - sprite : sf::Sprite
    }

    class Tower {
      - virtualPos : sf::Vector2f
      - damage : int
      - range : float
      - fireRate : float
      - radius : float
      - cost : int
      - sprite : sf::Sprite
      - bulletSprite : sf::Sprite
      + getRadius()
      + getVirtualPos()
      + draw(window, scale, offsetX)
      + drawRange(window, scale, offsetX)
      + drawRadius(window, scale, offsetX)
      + setVirtualPos(pos)
      + setColor(color)
    }

    class WaveManager
    class PlayerStats {
      - hp : int
      - money : int
      - score : int
    }

    class Level {
      - id : int
      - name : string
      - background : Background
      - background_mask : sf::Image
      - towers : vector<Tower>
      - enemies : vector<Enemy>
      - projectiles : vector<Projectile>
      - waveManager : WaveManager
      - playerStats : PlayerStats
      - currentScale : float
      - currentOffsetX : float
      + updateLayout(windowSize)
      + draw(window)
      + canPlaceTower(...)
      + addTower(tower)
      + mapMouseToVirtual(mousePos, windowSize)
      + getCurrentScale()
      + getCurrentOffsetX()
    }

    class MainMenu {
      - background : Background
      + updateLayout(windowSize)
      + draw(window)
    }

    class InGameGUI {
      - isShopOpen : bool
      - shopButton : Button
      - statsText : sf::Text
      - shopPanel : sf::RectangleShape
      - towerButtons : vector<Button>
      + toggleShop()
      + reset()
      + draw(window)
      + updateLayout(scale, offsetX, windowSize)
      + handleEvent(event, mousePos)
      + addTowerButton(texture,font,text,callback)
    }

    class Game {
      - window : sf::RenderWindow
      - assetManager : AssetManager
      - currentLevel : Level
      - mainMenu : MainMenu
      - gui : InGameGUI
      - gameState : GameState
      - pendingTower : Tower
      + run()
      + changeLevel(id,name,texture)
    }

    class Hashitbox {
      <<template>>
      + checkCollision(a,b)
    }

    %% Inheritance
    Enemy <|-- FastEnemy
    Enemy <|-- TankEnemy

    %% Ownership / composition
    Game *-- AssetManager
    Game *-- Level
    Game *-- MainMenu
    Game *-- InGameGUI
    Game *-- Tower : pendingTower

    Level *-- Background
    Level *-- PlayerStats
    Level *-- WaveManager
    Level *-- Tower
    Level *-- Enemy
    Level *-- Projectile

    MainMenu *-- Background

    InGameGUI *-- Button
    Level ..> Hashitbox : uses checkCollision
    Projectile ..> Enemy : target
    MainMenu ..> AssetManager
    InGameGUI ..> AssetManager
    Level ..> AssetManager
    Background ..> AssetManager : uses textures
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <vector>
#include <map>


template <typename Tile>
class Tileset {
public:
    Tileset(const sf::Texture& texture, size_t tilesize,
            const std::vector<std::vector<Tile>>& spec) :
        tilesize(tilesize),
        _texture(texture)
    {
        size_t i, j;
        i = 0;
        for (auto row : spec) {
            j = 0;
            for (auto tile : row) {
                _rects[tile] = sf::IntRect(j, i, tilesize, tilesize);
                j += tilesize;
            }
            i += tilesize;
        }
    }

    sf::Sprite getSprite(Tile id) const {
        //std::cerr << _rects.at(id) << std::endl;
        return sf::Sprite(_texture, _rects.at(id));
    }

    const size_t tilesize;
private:
    const sf::Texture& _texture;
    std::map<Tile, sf::IntRect> _rects;
};


//template <typename T>
//std::ostream& operator<<(std::ostream& stream, const sf::Rect<T>& rect) {
//    return stream <<
//        '[' << rect.left <<
//        ',' << rect.top <<
//        ',' << rect.width <<
//        ',' << rect.height <<
//        ']';
//}


template <typename Tile>
class Tilemap : public sf::Drawable, public sf::Transformable {
public:
    Tilemap(const Tileset<Tile>& tileset, size_t twidth, size_t theight) :
        _map(theight, std::vector<Tile>(twidth, Tile::EMPTY)),
        _tileset(tileset)
    {
        
    }

    Tile& operator[](sf::Vector2u pos) {
        return _map[pos.y][pos.x];
    }

    void draw (sf::RenderTarget &target, sf::RenderStates states) const {
        size_t i, j;
        i = 0;
        for (auto row : _map) {
            j = 0;
            for (auto tile : row) {
                if (tile != Tile::EMPTY) {
                    sf::Sprite sprite = _tileset.getSprite(tile);
                    sprite.move(j, i);
                    target.draw(sprite, states);
                }
                j += _tileset.tilesize;
            }
            i += _tileset.tilesize;
        }
    }

private:
    Tileset<Tile> _tileset;
    std::vector<std::vector<Tile>> _map;
};


enum class Tile {
    EMPTY,
    RED,
    BLUE,
    GREEN,
    YELLOW,
    BLACK,
};


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), __FILE__);

    sf::Texture texture;
    texture.loadFromFile("tileset.png");
    Tileset<Tile> tileset(texture, 64, {{Tile::RED, Tile::BLUE, Tile::GREEN, Tile::YELLOW}, {Tile::BLACK}});

    Tilemap<Tile> tilemap(tileset, 30, 30);

    tilemap[{10, 2}] = Tile::BLUE;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            } else if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        sf::Transform tr;
        tr.scale(0.3, 0.3);
        window.draw(tilemap, tr);
        window.display();
    }

    return 0;
}

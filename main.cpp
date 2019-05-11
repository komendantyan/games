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
        for (size_t i = 0; i < spec.size(); ++i) {
            for (size_t j = 0; j < spec[0].size(); ++j) {
                _rects[spec[i][j]] = sf::IntRect(j * tilesize, i * tilesize,
                                           tilesize, tilesize);
            }
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
        // pass
    }

    Tile& operator[](sf::Vector2u pos) {
        return _map[pos.y][pos.x];
    }

    void draw (sf::RenderTarget &target, sf::RenderStates states) const {
        for (size_t i = 0; i < _map.size(); ++i) {
            for (size_t j = 0; j < _map[0].size(); ++j) {
                const Tile& tile = _map[i][j];
                if (tile != Tile::EMPTY) {
                    sf::Sprite sprite = _tileset.getSprite(tile);
                    sprite.move(j * _tileset.tilesize, i * _tileset.tilesize);
                    target.draw(sprite, states);
                }
            }
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

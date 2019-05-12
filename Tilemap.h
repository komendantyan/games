#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <map>


template <typename Tile>
class Tileset {
public:
    Tileset(const sf::Texture& texture, size_t tilesize,
            const std::vector<std::vector<Tile>>& spec) :
        tilesize(tilesize),
        texture(texture)
    {
        for (size_t i = 0; i < spec.size(); ++i) {
            for (size_t j = 0; j < spec[0].size(); ++j) {
                Tile current = spec[i][j];
                if (current != Tile::EMPTY) {
                    _rects[current] = sf::IntRect(j * tilesize, i * tilesize,
                                               tilesize, tilesize);
                }
            }
        }
    }

    sf::Sprite getSprite(Tile id) const {
        return sf::Sprite(texture, _rects.at(id));
    }

    const size_t tilesize;
    const sf::Texture texture;  // TODO store or reference?
private:
    std::map<Tile, sf::IntRect> _rects;
};


template <typename Tile>
class Tilemap : public sf::Drawable, public sf::Transformable {
public:
    Tilemap(const Tileset<Tile>& tileset, size_t twidth, size_t theight,
            Tile fill = Tile::EMPTY) :
        _map(theight, std::vector<Tile>(twidth, fill)),
        _tileset(tileset)
    {
        // pass
    }

    Tilemap(const Tileset<Tile>& tileset, std::vector<std::vector<Tile>> map) :
        _map(map),
        _tileset(tileset)
    {
        // pass
    }

    Tile& operator[](sf::Vector2u pos) {
        return _map[pos.y][pos.x];
    }

    void setMap(const std::vector<std::vector<Tile>>& new_map) {
        _map = new_map;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
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

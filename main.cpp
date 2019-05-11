#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Tilemap.h"

#include <iostream>
#include <deque>
#include <set>
#include <algorithm>
#include <random>


std::set<sf::Keyboard::Key> ARROW_KEYS = {
    sf::Keyboard::Left,
    sf::Keyboard::Up,
    sf::Keyboard::Right,
    sf::Keyboard::Down
};

enum class Tile {
    EMPTY,  // EMPTY,
    APPLE,  // RED,
    SNAKE,  // BLUE,
    HEAD,  // GREEN,
    FIELD,  // YELLOW,
    NIGHT  // BLACK,
};


Tileset<Tile> loadTileset() {
    sf::Texture texture;
    texture.loadFromFile("tileset.png");
    return {
        std::move(texture), 64,
        {
            //{Tile::RED, Tile::BLUE, Tile::GREEN, Tile::YELLOW},
            //{Tile::BLACK}
            {Tile::APPLE, Tile::SNAKE, Tile::HEAD, Tile::FIELD},
            {Tile::NIGHT}
        }
    };
}

enum class SnakeStep {
    LEFT,
    UP,
    RIGHT,
    DOWN,
};



// TODO think about queue of action concept ?
class Environment {
public:
    using Battlefield = std::vector<std::vector<Tile>>;
    using State = Battlefield;

    enum class Action {
        TURN_RIGHT,
        TURN_UP,
        TURN_LEFT,
        TURN_DOWN,
        SWAP_HEAD
    };

    enum class Direction {
        RIGHT,
        UP,
        LEFT,
        DOWN,
    };

public:
    Environment(size_t width, size_t height) :
        _width(width),
        _height(height),
        battlefield(height, std::vector<Tile>(width, Tile::FIELD)),
        game_over(false),
        block_action(false)
    {
        _snake = {{2, 0}, {1, 0}, {0, 0}};
        last_tail_pos = {0, 1};

        for (auto pos : _snake) {
            battlefield[pos.y][pos.x] = Tile::SNAKE;
        }
        battlefield[_snake.front().y][_snake.front().x] = Tile::HEAD;

        std::mt19937 mersenne(std::random_device{}());
        std::uniform_int_distribution<size_t> distr_width(0, width - 1),
                                              distr_height(1, height - 1);

        for (int i = 0; i < 15; ++i) {
            size_t y = distr_height(mersenne);
            size_t x = distr_width(mersenne);
            battlefield[y][x] = Tile::APPLE;
        }

        current_direction = Direction::RIGHT;
    }

    void act(Action action) {
        if (block_action) {
            return;
        }

        if (action == Action::SWAP_HEAD) {
            std::reverse(_snake.begin(), _snake.end());
            current_direction = getDirectionFromTwoVector(_snake.front(),
                                                          last_tail_pos);
        } else {
            const Direction forbidden_direction = aboutFace(current_direction);
            Direction proposal_direction;
            switch (action) {
                case Action::TURN_RIGHT: {
                     proposal_direction = Direction::RIGHT;
                     break;
                }
                case Action::TURN_UP: {
                     proposal_direction = Direction::UP;
                     break;
                }
                case Action::TURN_LEFT: {
                     proposal_direction = Direction::LEFT;
                     break;
                }
                case Action::TURN_DOWN: {
                     proposal_direction = Direction::DOWN;
                     break;
                }
            }

            if (forbidden_direction != proposal_direction) {
                current_direction = proposal_direction;
            }
        }

        block_action = true;
    }

    void step() {
        if (game_over) {
            return;
        }

        const sf::Vector2u& head = _snake.front();
        sf::Vector2u new_head;

        switch (current_direction) {
            case Direction::RIGHT: {
                if (head.x == _width - 1) {
                    game_over = true;
                    return;
                }
                new_head = _snake.front() + sf::Vector2u{1, 0};
                break;
            }
            case Direction::UP: {
                if (head.y == 0) {
                    game_over = true;
                    return;
                }
                new_head = _snake.front() - sf::Vector2u{0, 1};
                break;
            }
            case Direction::LEFT: {
                if (head.x == 0) {
                    game_over = true;
                    return;
                }
                new_head = _snake.front() - sf::Vector2u{1, 0};
                break;
            }
            case Direction::DOWN: {
                if (head.y == _height - 1) {
                    game_over = true;
                    return;
                }
                new_head = _snake.front() + sf::Vector2u{0, 1};
                break;
            }
        }

        switch (battlefield[new_head.y][new_head.x]) {
            case Tile::SNAKE: {
                game_over = true;
                break;
            }
            case Tile::APPLE: {
                battlefield[_snake.front().y][_snake.front().x] = Tile::SNAKE;
                _snake.push_front(new_head);
                battlefield[new_head.y][new_head.x] = Tile::HEAD;
                break;
            }
            case Tile::FIELD: {
                battlefield[_snake.back().y][_snake.back().x] = Tile::FIELD;
                last_tail_pos = _snake.back();
                _snake.pop_back();
                battlefield[_snake.front().y][_snake.front().x] = Tile::SNAKE;
                _snake.push_front(new_head);
                battlefield[new_head.y][new_head.x] = Tile::HEAD;
                break;
            }
        }
        block_action = false;
    }

    State getState() {
        return battlefield;
    }

private:
    std::deque<sf::Vector2u> _snake;
    Battlefield battlefield;
    size_t _width;
    size_t _height;
    Direction current_direction;
    bool game_over;
    bool block_action;
    sf::Vector2u last_tail_pos;

private:
    static Direction aboutFace(Direction dir) {
        return static_cast<Direction>(
            (static_cast<size_t>(dir) + 2) % 4);
    }

    static Direction getDirectionFromTwoVector(const sf::Vector2u& lhs,
                                               const sf::Vector2u& rhs) {
        if (lhs.x == rhs.x) {
            if (lhs.y == rhs.y) {
                throw std::invalid_argument("Equal lhs and rhs");
            } else if (lhs.y < rhs.y) {
                return Direction::DOWN;
            } else {
                return Direction::UP;
            }
        } else if (lhs.y != rhs.y) {
            throw std::invalid_argument("lhs and rhs not on same horizontal or vertical");
        } else if (lhs.x < rhs.x) {
            return Direction::RIGHT;
        } else {
            return Direction::LEFT;
        }
    }
};



int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), __FILE__);

    Environment env(40, 30);

    auto tileset = loadTileset();
    Tilemap<Tile> tilemap(tileset, 0, 0);
    tilemap.scale(0.25, 0.25);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Escape: {
                        window.close();
                        break;
                    }
                    case sf::Keyboard::Right: {
                        env.act(Environment::Action::TURN_RIGHT);
                        break;
                    }
                    case sf::Keyboard::Up: {
                        env.act(Environment::Action::TURN_UP);
                        break;
                    }
                    case sf::Keyboard::Left: {
                        env.act(Environment::Action::TURN_LEFT);
                        break;
                    }
                    case sf::Keyboard::Down: {
                        env.act(Environment::Action::TURN_DOWN);
                        break;
                    }
                    case sf::Keyboard::Space: {
                        env.act(Environment::Action::SWAP_HEAD);
                        break;
                    }
                }
            } else if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        env.step();
        tilemap.setMap(env.getState());

        window.clear();
        window.draw(tilemap);
        window.display();

        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}

#pragma once

#include <SFML/System.hpp>

#include <deque>
#include <algorithm>
#include <random>

#include "Messages.h"

namespace SnakeGame {

// TODO think about queue of action concept ?
class Environment {
public:
    const size_t width;
    const size_t height;

private:
    enum class Direction {
        RIGHT,
        UP,
        LEFT,
        DOWN,
    };

private:
    std::deque<sf::Vector2u> snake;
    Battlefield battlefield;
    unsigned int apples_count;
    Direction current_direction;
    bool game_over;
    bool is_winner;
    bool block_action;  // TODO comment
    sf::Vector2u last_tail_pos;

public:
    //typedef Tile tile_type;  // TODO think about it

    Environment(size_t width, size_t height, unsigned int apples_count) :
        width(width),
        height(height),
        apples_count(apples_count),
        battlefield(height, std::vector<Tile>(width, Tile::FIELD)),

        game_over(false),
        is_winner(false),
        block_action(false)
    {
        snake = {{2, 0}, {1, 0}, {0, 0}};
        last_tail_pos = {0, 1};

        for (auto pos : snake) {
            battlefield[pos.y][pos.x] = Tile::SNAKE;
        }
        battlefield[snake.front().y][snake.front().x] = Tile::HEAD;

        std::mt19937 mersenne(std::random_device{}());
        std::uniform_int_distribution<size_t> distr_width(0, width - 1),
                                              distr_height(1, height - 1);

        for (int i = 0; i < apples_count; ++i) {
            size_t x, y;

            do {
                y = distr_height(mersenne);
                x = distr_width(mersenne);
            } while (battlefield[y][x] == Tile::APPLE);
            battlefield[y][x] = Tile::APPLE;
        }

        current_direction = Direction::RIGHT;
    }

    void act(Action action) {
        if (block_action) {
            return;
        }

        if (action == Action::SWAP_HEAD) {
            std::reverse(snake.begin(), snake.end());
            current_direction = getDirectionFromTwoVector(snake.front(),
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

        const sf::Vector2u& head = snake.front();
        sf::Vector2u new_head;

        switch (current_direction) {
            case Direction::RIGHT: {
                if (head.x == width - 1) {
                    game_over = true;
                    return;
                }
                new_head = snake.front() + sf::Vector2u{1, 0};
                break;
            }
            case Direction::UP: {
                if (head.y == 0) {
                    game_over = true;
                    return;
                }
                new_head = snake.front() - sf::Vector2u{0, 1};
                break;
            }
            case Direction::LEFT: {
                if (head.x == 0) {
                    game_over = true;
                    return;
                }
                new_head = snake.front() - sf::Vector2u{1, 0};
                break;
            }
            case Direction::DOWN: {
                if (head.y == height - 1) {
                    game_over = true;
                    return;
                }
                new_head = snake.front() + sf::Vector2u{0, 1};
                break;
            }
        }

        switch (battlefield[new_head.y][new_head.x]) {
            case Tile::SNAKE: {
                game_over = true;
                break;
            }
            case Tile::APPLE: {
                battlefield[snake.front().y][snake.front().x] = Tile::SNAKE;
                snake.push_front(new_head);
                battlefield[new_head.y][new_head.x] = Tile::HEAD;
                if (--apples_count == 0) {
                    is_winner = true;
                }
                break;
            }
            case Tile::FIELD: {
                battlefield[snake.back().y][snake.back().x] = Tile::FIELD;
                last_tail_pos = snake.back();
                snake.pop_back();
                battlefield[snake.front().y][snake.front().x] = Tile::SNAKE;
                snake.push_front(new_head);
                battlefield[new_head.y][new_head.x] = Tile::HEAD;
                break;
            }
        }
        block_action = false;
    }

    State getState() const {
        return {
            battlefield,
            game_over,
            is_winner
        };
    }

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

}  // namespace SnakeGame

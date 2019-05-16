#pragma once

#include <SFML/System.hpp>

#include <deque>
#include <algorithm>
#include <random>

#include "Messages.h"

namespace TetrisGame {

class Environment {
public:
    const size_t width;
    const size_t height;

private:
    Battlefield<Tile> battlefield;
    std::vector<sf::Vector2u> current_tetromino;
    bool game_over;
    bool is_winner;

    enum class Direction {
        RIGHT,
        UP,
        LEFT,
        DOWN,
    };

public:
    Environment(size_t width, size_t height) :
        width(width),
        height(height),
        battlefield(height, std::vector<Tile>(width, Tile::VACANT)),

        game_over(false),
        is_winner(false)
    {
        createTetromino();
        //std::mt19937 mersenne(std::random_device{}());
        //std::uniform_int_distribution<size_t> distr_width(0, width - 1),
        //                                      distr_height(1, height - 1);
    }

    void act(Action action) {  // TODO mapping?
        switch (action) {
            case Action::MOVE_LEFT: {
                tryMove(Direction::LEFT);
                break;
            }
            case Action::MOVE_RIGHT: {
                tryMove(Direction::RIGHT);
                break;
            }
            case Action::MOVE_DOWN: {
                tryMove(Direction::DOWN);
                break;
            }
            case Action::ROTATE: {
                tryRotate(Direction::RIGHT);
                break;
            }
            case Action::FAST_MOVE_DOWN: {
                while (tryMove(Direction::DOWN)) {

                }
                break;
            }
        }
    }

    void step() {
        if (game_over) {
            return;
        }

        if (!tryMove(Direction::DOWN)) {
            nextTetromino();
        }
    }

    State getState() const {
        return {
            battlefield,
            game_over,
            is_winner
        };
    }

private:
    bool tryMove(Direction dir) {
        auto tetromino = current_tetromino;
        for (auto& square : tetromino) {
            switch (dir) {
                case Direction::RIGHT: {
                    if (square.x == width - 1) {
                        return false;
                    }
                    square += sf::Vector2u{1, 0};
                    break;
                }
                case Direction::UP: {  // TODO WTF?
                    if (square.y == 0) {
                        return false;
                    }
                    square -= sf::Vector2u{0, 1};
                    break;
                }
                case Direction::LEFT: {
                    if (square.x == 0) {
                        return false;
                    }
                    square -= sf::Vector2u{1, 0};
                    break;
                }
                case Direction::DOWN: {
                    if (square.y == height - 1) {
                        return false;
                    }
                    square += sf::Vector2u{0, 1};
                    break;
                }
            }

            if (battlefield[square.y][square.x] == Tile::OCCUPIED) {
                return false;
            }
        }
        for (auto square : current_tetromino) {
            battlefield[square.y][square.x] = Tile::VACANT;
        }
        for (auto square : tetromino) {
            battlefield[square.y][square.x] = Tile::CURRENT;
        }
        current_tetromino = tetromino;
        return true;
    }

    bool tryRotate(Direction dir) {  // TODO Direction -> RotationSide
        auto tetromino = current_tetromino;
        sf::Vector2u center = current_tetromino[0];

        for (auto& square : tetromino) {
            switch (dir) {
                case Direction::RIGHT: {
                    int x = static_cast<int>(center.x) + (static_cast<int>(square.y) - static_cast<int>(center.y));
                    int y = static_cast<int>(center.y) - (static_cast<int>(square.x) - static_cast<int>(center.x));
                    if (x < 0 || x + 1 > width || y < 0 || y + 1 > height) {
                        return false;
                    }
                    square.x = x;
                    square.y = y;
                    break;
                }
                case Direction::LEFT: {
                    int x = static_cast<int>(center.x) + (static_cast<int>(square.y) - static_cast<int>(center.y));
                    int y = static_cast<int>(center.y) + (static_cast<int>(square.x) - static_cast<int>(center.x));
                    if (x < 0 || x + 1 > width || y < 0 || y + 1 > height) {
                        return false;
                    }
                    square.x = x;
                    square.y = y;
                    break;
                }
            }

            if (battlefield[square.y][square.x] == Tile::OCCUPIED) {
                return false;
            }
        }
        for (auto square : current_tetromino) {
            battlefield[square.y][square.x] = Tile::VACANT;
        }
        for (auto square : tetromino) {
            battlefield[square.y][square.x] = Tile::CURRENT;
        }
        current_tetromino = tetromino;
        return true;
    }


    void nextTetromino() {
        freezeTetromino();
        tryDisapearLines();
        createTetromino();
    }

    void freezeTetromino() {
        for (auto square : current_tetromino) {
            battlefield[square.y][square.x] = Tile::OCCUPIED;
        }
    }

    unsigned int tryDisapearLines() {  // TODO it's not optimal
        auto new_end = std::remove_if(
            battlefield.begin(),
            battlefield.end(),
            [](auto& row){return std::all_of(
                row.begin(),
                row.end(),
                [](auto tile){return tile == Tile::OCCUPIED;}
            );}
        );
        size_t lines = battlefield.end() - new_end;
        battlefield.erase(new_end, battlefield.end());
        for (size_t j = 0; j < lines; ++j) {
            battlefield.insert(battlefield.begin(), std::vector<Tile>(width, Tile::VACANT));
        }
        return lines;
    }

    void createTetromino() {  // TODO randomize
        current_tetromino = {{6, 0}, {5, 0}, {5, 1}, {7, 0}};
        for (auto square : current_tetromino) {
            battlefield[square.y][square.x] = Tile::CURRENT;
        }
    }
};

}  // namespace TetrisGame

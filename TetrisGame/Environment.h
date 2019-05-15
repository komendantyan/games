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

    void act(Action action) {
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

    void nextTetromino() {
        freezeTetromino();
        createTetromino();
    }

    void freezeTetromino() {
        for (auto square : current_tetromino) {
            battlefield[square.y][square.x] = Tile::OCCUPIED;
        }
    }

    void createTetromino() {
        current_tetromino = {{5, 0}, {5, 1}, {6, 0}, {7, 0}};
        for (auto square : current_tetromino) {
            battlefield[square.y][square.x] = Tile::CURRENT;
        }
    }
};

}  // namespace TetrisGame

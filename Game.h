#pragma once

#include <map>
#include "Tilemap.h"

template <typename Action>
using KeyboardMapping = std::map<sf::Keyboard::Key, Action>;

template <typename Environment, typename Action, typename Tile>
class Game {
private:
    sf::RenderWindow& window;
    Environment env;
    Tilemap<Tile> tilemap;
    KeyboardMapping<Action> keyboardMapping;
    sf::Clock clock;
    sf::Time lastStepTime;
    sf::Time stepDuration;

public:
    Game(
            sf::RenderWindow& window,
            Environment env,
            KeyboardMapping<Action> keyboardMapping,
            Tileset<Tile> tileset,
            sf::Time stepDuration
    ) :
        window(window),
        env(env),
        keyboardMapping(keyboardMapping),
        tilemap(tileset, env.width, env.height),
        stepDuration(stepDuration)
    {
        tilemap.scale(0.25, 0.25);  // TODO
    }

    void processEvent(sf::Event event) {
        switch (event.type) {
            case sf::Event::KeyPressed: {
                keyPressed(event.key);
                break;
            }
        }
    }

    void step() {
        if (clock.getElapsedTime() - lastStepTime > stepDuration) {
            lastStepTime += stepDuration;
            env.step();
        }
    }

    void draw() {  // not about sf::Drawable
        window.clear();
        tilemap.setMap(env.getState().battlefield);
        window.draw(tilemap);
        window.display();
    }

private:
    void keyPressed(sf::Event::KeyEvent key) {
        auto it = keyboardMapping.find(key.code);
        if (it != keyboardMapping.end()) {
            env.act(it->second);
        }
    }
};

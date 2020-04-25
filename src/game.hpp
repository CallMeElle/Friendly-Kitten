#pragma once

#include "redhand/engine.hpp"
#include "redhand/world.hpp"

int game_init(
    std::shared_ptr<redhand::world> startWorld
);

int main_game_logic(
    redhand::engine* gameEngine
);

int createTestworld(std::shared_ptr<redhand::world> testWorld);
void processWorldInput(GLFWwindow* window, std::shared_ptr<redhand::world> activeWorld);
void processGlobalInput(redhand::engine* game);
void processKittenInput(redhand::engine* game);
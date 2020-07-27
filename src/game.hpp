#pragma once

#include <redhand/all.hpp>

using namespace redhand;

class kitten_world : public redhand::complex_world{
    public:
        int onCreate(event<engine> evt);

        void tick(game_loop_event evt);
};

class kitten : public redhand::Actor{
    public:
        kitten();

        void act(game_loop_event evt);
};

int processGlobalInput(redhand::game_loop_event evt);

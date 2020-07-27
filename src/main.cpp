#include <iostream>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "redhand/all.hpp"
#include "game.hpp"

using namespace redhand;

int main(){
    int exitCode = 0;

    //create the engine object
    auto gameEngine = std::make_shared<engine>();

    //get the current config of the engine
    engine_config conf = gameEngine->getConfig();

    //change the configuration and set the new config
    conf.title = "Friendly Kitten";
    conf.RESIZABLE = true;
    gameEngine->setConfig(conf);

    //set the function which handles all the controls and physics computation
    gameEngine->addGameLoopHandler(processGlobalInput, "global_input");

    //initilize the game engine
    gameEngine->init();

    gameEngine->changeWorld(std::make_shared<kitten_world>());

    //set the exit flags in case something went wrong 
    exitCode = gameEngine->getErrorCode();
    if(exitCode < 0){
        gameEngine->stopGame();
    }else{        
        //run the game
        exitCode = gameEngine->runGame(); 
    }
    
    //return the error code if something bad happened or 0 if everything is fine
    return abs(exitCode);
    
}

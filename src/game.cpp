#include "game.hpp"
#include <redhand/types.hpp>

using namespace redhand;

int kitten_world::onCreate(event<engine>){
    //creating the objects and add them to the world
    int edges = 240;

    // Background rectangle 
    if( this->addObject(
        redhand::createRectangle(
            {-100.0f, -100.0f},
            200.0f,
            200.0f,
            {1.0f, 0.0f, 0.0f},
            nullptr,
            nullptr,
            STATIC_DRAW,
            "background",
            1.0f
        )
    ) < 0){
        return -3;
    }

     // sun two
    if( this->addObject(
        redhand::createCircle(
            {0.6f, 0.6f},
            0.6f,
            edges,
            {0.0f, 0.8f, 1.0f},
            {0.8f, 0.0f, 1.0f},
            nullptr,
            nullptr,
            "sun2"
        )
    ) < 0){
        return -3;
    }

    // sun one
    if( this->addObject(redhand::createCircle({0.8f,0.45f}, 0.35f, edges, {1.0f,1.0f,0.0f}, {1.0f,0.5f,0.0f}, nullptr, nullptr, "sun1")) < 0){
        return -3;
    }

    this->add(new kitten());

    return 0;
}

void kitten_world::tick(redhand::game_loop_event evt) {
    complex_world::tick(evt);

    //move the camera
    std::array<float,2> deltaCamera = {0.0f,0.0f};

    if( redhand::input_system::static_isKeyPressed(redhand::KEY_D) ){
        deltaCamera.at(0) = 0.02f;
    }else if( redhand::input_system::static_isKeyPressed(redhand::KEY_A) ){
        deltaCamera.at(0) = -0.02f;
    }

    if( redhand::input_system::static_isKeyPressed(redhand::KEY_W) ){
        deltaCamera.at(1) = 0.02f;
    }else if( redhand::input_system::static_isKeyPressed(redhand::KEY_S) ){
        deltaCamera.at(1) = -0.02f;
    }

    this->moveCamera(deltaCamera.at(0),deltaCamera.at(1));

    return;
}

kitten::kitten(){

    redhand::image_properties prop;
    prop.file_location = "Textures/kitten.png";
    prop.wrap_S = GL_CLAMP_TO_EDGE;
    prop.wrap_T = GL_CLAMP_TO_EDGE;
    prop.name = "kitten";
    setImage(new texture2D(prop));
    setPosition( {-0.5f, +0.5f});
    scaleActor(0.5f);
    setColorAlpha(1.0f);

}

void kitten::act(redhand::game_loop_event evt){
    auto window = evt.getRaiser()->getWindow();

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS){
        std::cout<<"Mouse key pressed\n";
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout<<"Cursor position x: "<< xpos <<"\n";
        std::cout<<"Cursor position y: "<< ypos <<"\n";
    }
}

int processGlobalInput(redhand::game_loop_event evt){
    
    auto window = evt.getRaiser()->getWindow();

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        evt.getRaiser()->stopGame();
    }

    if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    return 0;

}


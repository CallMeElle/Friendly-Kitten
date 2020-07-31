#include "game.hpp"
#include "redhand/input.hpp"
#include <iostream>
#include <memory>
#include <redhand/types.hpp>

using namespace redhand;

int kitten_world::onCreate(event<engine>){
    //creating the objects and add them to the world
    int edges = 240;

    // Background rectangle 
    if( this->addObject(
        createRectangle(
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
        createCircle(
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
    if( this->addObject(createCircle({0.8f,0.45f}, 0.35f, edges, {1.0f,1.0f,0.0f}, {1.0f,0.5f,0.0f}, nullptr, nullptr, "sun1")) < 0){
        return -3;
    }

    this->add(std::make_shared<kitten>() );
    this->add(std::make_shared<seal>());

    return 0;
}

void kitten_world::tick(game_loop_event evt) {
    complex_world::tick(evt);

    //move the camera
    std::array<float,2> deltaCamera = {0.0f,0.0f};

    if( input_system::static_isKeyPressed(KEY_D) ){
        deltaCamera.at(0) = 0.02f;
    }else if( input_system::static_isKeyPressed(KEY_A) ){
        deltaCamera.at(0) = -0.02f;
    }

    if( input_system::static_isKeyPressed(KEY_W) ){
        deltaCamera.at(1) = 0.02f;
    }else if( input_system::static_isKeyPressed(KEY_S) ){
        deltaCamera.at(1) = -0.02f;
    }

    this->moveCamera(deltaCamera.at(0),deltaCamera.at(1));

    return;
}

kitten::kitten(){

    setName("kitten");

    image_properties prop;
    prop.file_location = "Textures/kitten.png";
    prop.wrap_S = GL_CLAMP_TO_EDGE;
    prop.wrap_T = GL_CLAMP_TO_EDGE;
    prop.name = "kitten";
    prop.keep_image_data = true;
    setImage(prop);
    setPosition( {-0.5f, +0.5f});
    scaleActor(0.5f);
    setColorAlpha(1.0f);

}

void kitten::act(game_loop_event evt){
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


seal::seal(){

    setName("seal");

    image_properties prop;
    prop.file_location = "Textures/kitten.png";
    prop.wrap_S = GL_CLAMP_TO_EDGE;
    prop.wrap_T = GL_CLAMP_TO_EDGE;
    prop.name = "seal";
    prop.keep_image_data = true;
    setImage(prop);
    setPosition( {-0.5f, +0.5f});
    scaleActor(0.5f);
    setColorAlpha(1.0f);

}

void seal::act(game_loop_event evt){
    if(input_system::static_isKeyPressed(KEY_UP)) {
        move({0.0,0.01});
    }
    if(input_system::static_isKeyPressed(redhand::KEY_DOWN)) {
        move({0.0,-0.01});
    }
    if(input_system::static_isKeyPressed(redhand::KEY_LEFT)) {
        move({-0.01,0.0});
    }
    if(input_system::static_isKeyPressed(redhand::KEY_RIGHT)) {
        move({0.01,0.0});
    }
    if(input_system::static_isKeyPressed(redhand::KEY_T)) {
        std::cout<< "You don't undertstand this game? I don't either.."<< std::endl;
    }

    auto kit = evt.getRaiser()->getActiveWorld()->get("kitten");

    if (kit != nullptr){

        auto size = getSize();
        auto pos = getPosition();

        auto pos_k = kit->getPosition();
        auto size_k = kit->getSize();

        auto topRight = pos+size;
        auto topRight_k = pos_k+size_k;

        bool debug = true;
        bool collision = false;

        bool overlap_x = false;
        bool overlap_y = false;

        if (pos.x < pos_k.x  || pos.x < topRight_k.x) {
            if (pos_k.x < topRight.x || topRight_k.x < topRight.x) {
                overlap_x = true;
            }
        }

        if (pos.y < pos_k.y || pos.y < topRight_k.y) {
            if (pos_k.y < topRight.y || topRight_k.y < topRight.y) {
                overlap_y = true;
            }
        }

        collision = overlap_x && overlap_y;

        if(debug){
            if(collision){
                std::cout<<"Collision between seal and cat"<<std::endl;
            }else{
                std::cout<<"No collision"<<std::endl;
            }
        }

    }
        
}


int processGlobalInput(game_loop_event evt){
    
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


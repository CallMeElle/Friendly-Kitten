#include "game.hpp"

int game_init(
    std::shared_ptr<redhand::world> startWorld
){
    int exitCode = 0;

    exitCode = createTestworld(startWorld);
    if(exitCode < 0){
        return exitCode;
    }

    return 0;   
}

int main_game_logic(
    redhand::engine* gameEngine
){
    //process the input
    processGlobalInput(gameEngine);
    processWorldInput(gameEngine->getWindow(), gameEngine->getActiveWorld());

    auto time = std::chrono::high_resolution_clock::now();
    static decltype(time) last_pressed;

    if(std::chrono::duration_cast<std::chrono::milliseconds>(time-last_pressed).count() > 100){
        processKittenInput(gameEngine);
        last_pressed = std::chrono::high_resolution_clock::now();
    }

    //get the current window size
    int width, height;
    glfwGetWindowSize(gameEngine->getWindow(), &width, &height);
    gameEngine->getActiveWorld()->setWindowSize(width, height);

    //tick the active world
    gameEngine->getActiveWorld()->tick(gameEngine->getWindow());

    return 0;

}

int createTestworld(std::shared_ptr<redhand::world> testWorld){
    //add shaders to world

    auto shader1 = std::unique_ptr<redhand::shader>(new redhand::shader());
    if( testWorld->addShader(std::move(shader1)) < 0){
        std::cerr << "Got error while adding shader" << std::endl;
        return -10;
    }

    if(testWorld->getShaderByName("default") == nullptr){
        std::cerr << "Got nullpointer as shader" << std::endl;
        return -11;
    }

    auto kitten_texture_properties = redhand::DEFAULT_IMAGE_PROPERTIES;
    kitten_texture_properties.file_location = "Textures/kitten-1.png";
    kitten_texture_properties.name = "kitten";
    auto kitten = std::unique_ptr<redhand::texture2D> (new redhand::texture2D(kitten_texture_properties));
    if(testWorld->addTexture(std::move(kitten)) < 0){
        std::cerr << "Got error while adding texture" << std::endl;
        return -12;
    }

    //creating the objects and add them to the world
    int edges = 240;

    // Background rectangle 
    if( testWorld->addObject(
        redhand::createRectangle(
            {-100.0f, -100.0f},
            200.0f,
            200.0f,
            {1.0f, 0.0f, 0.0f},
            testWorld->getShaderByName("default"),
            nullptr,
            GL_STATIC_DRAW,
            "background",
            1.0f
        )
    ) < 0){
        return -3;
    }

     // sun two
    if( testWorld->addObject(
        redhand::createCircle(
            {0.6f, 0.6f},
            0.6f,
            edges,
            {0.0f, 0.8f, 1.0f},
            {0.8f, 0.0f, 1.0f},
            testWorld->getShaderByName("default"),
            nullptr,
            "sun2"
        )
    ) < 0){
        return -3;
    }

    // sun one
    if( testWorld->addObject(redhand::createCircle({0.8f,0.45f}, 0.35f, edges, {1.0f,1.0f,0.0f}, {1.0f,0.5f,0.0f}, testWorld->getShaderByName("default"), nullptr, "sun1")) < 0){
        return -3;
    }

    //triangle
    auto trig_properties = redhand::DEFAULT_GAME_OBJECT_PROPERTIES;
    trig_properties.points_coordinates = {{0.0f,0.0f}, {1.0f,0.0f}, {0.5f,1.0f}};
    trig_properties.triangle_indices = {{0,1,2}};
    trig_properties.point_colors = {{1.0f,0.0f,0.0f}, {0.0f,1.0f,0.0f}, {0.0f,0.0f,1.0f}};
    trig_properties.texture_coordinates = {{0.0f,0.0f}, {1.0f*10.0f,0.0f}, {0.5f*10.0f,1.0f*10.0f}};
    trig_properties.postition = {-0.4f,-0.4f};
    trig_properties.name = "trig";
    trig_properties.attached_shader = testWorld->getShaderByName("default");
    trig_properties.scale = {0.5f,0.5f};
    
    if( testWorld->addObject(std::unique_ptr<redhand::game_object>(new redhand::game_object(trig_properties)) ) < 0){
        return -3;
    }

    testWorld->getObjectByName("trig")->setLoopFunction([](GLFWwindow*, redhand::game_object* obj){
        obj->setRotation((float)glfwGetTime()*20.0f);
    });
    
    auto kitten_properties = redhand::DEFAULT_GAME_OBJECT_PROPERTIES;
    kitten_properties.name = "cat";
    kitten_properties.points_coordinates = {
        {0.0f,0.0f},
        {1.0f,0.0f},
        {0.0f,1.0f},
        {1.0f,1.0f}
    };
    kitten_properties.texture_coordinates = {
        {0.0f,0.0f},
        {1.0f,0.0f},
        {0.0f,1.0f},
        {1.0f,1.0f}
    };
    kitten_properties.point_colors = {
        {1.0f,1.0f,1.0f},
        {1.0f,1.0f,1.0f},
        {1.0f,1.0f,1.0f},
        {1.0f,1.0f,1.0f}
    };
    kitten_properties.triangle_indices = {
        {0,1,2},
        {1,2,3}
    };
    kitten_properties.attached_shader = testWorld->getShaderByName("default");
    kitten_properties.attached_texture = testWorld->getTextureByName("kitten");
    //kitten_properties.attached_texture = nullptr;
    kitten_properties.scale = {0.7f,0.7f};
    kitten_properties.postition = {-0.5f, -0.5f};
    kitten_properties.texture_scale = {1.0f,1.0f};

    if( testWorld->addObject(std::unique_ptr<redhand::game_object>(new redhand::game_object(kitten_properties))) < 0){
        return -3;
    }

    auto cat = testWorld->getObjectByName("cat");
    cat->setColorAlpha(0.0f);

    return 0;
}

void processWorldInput(GLFWwindow* window, std::shared_ptr<redhand::world> activeWorld){
    //move the camera
    std::array<float,2> deltaCamera = {0.0f,0.0f};

    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
        deltaCamera.at(0) = 0.002f;
    }else if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
        deltaCamera.at(0) = -0.002f;
    }

    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
        deltaCamera.at(1) = 0.002f;
    }else if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){
        deltaCamera.at(1) = -0.002f;
    }

    activeWorld->moveCamera(deltaCamera.at(0),deltaCamera.at(1));

}

void processGlobalInput(redhand::engine* game){

    auto window = game->getWindow();

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        game->stopGame();
    }

    if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}

void processKittenInput(redhand::engine* game) {
    auto window = game->getWindow();

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS){
        std::cout<<"Mouse key pressed\n";
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout<<"Cursor position x: "<< xpos <<"\n";
        std::cout<<"Cursor position y: "<< ypos <<"\n";
    }
}

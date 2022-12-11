#include<GL/glew.h>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<stdio.h>
#include<chrono>
#include<math.h>

#include"Shader.h"
#include"Camera.h"

int main()
{
    bool frameDrawingMode = false;

    sf::Clock clock;
    sf::Time time = clock.restart();

    //--------------------------------------------------------------
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 4;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(800, 600), "OpenGL 4.3", sf::Style::Default, settings);

    window.setVerticalSyncEnabled(false);

    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
        printf("glew init fail");
        return -1;
    }

    window.setActive(true);

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 800, 600);

    //--------------------------------------------------------------

    Shader shdBase("vertex.glsl", "fragment.glsl");
    Shader lightShd("light.vertex.glsl", "light.fragment.glsl");

    glm::vec3 lightPos = glm::vec3(1.2f, 1.f, 2.f);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    //setup buffers
    unsigned int VBO;
    unsigned int cubeVAO;

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //--------------------------------------------------------------

    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    //--------------------------------------------------------------
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    while (window.isOpen())
    {
        time = clock.getElapsedTime();

        float currentFrame = time.asSeconds();
        deltaTime = currentFrame - lastTime;
        lastTime = currentFrame;

        sf::Event windowEvent;
        while (window.pollEvent(windowEvent))
        {
            switch (windowEvent.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                }
                break;
                case sf::Event::Resized:
                {
                    glViewport(0, 0, window.getSize().x, window.getSize().y);
                }
                break;
                case sf::Event::MouseMoved:
                {
                    float sens = 2.0f;
                    float xpos = 0;
                    float ypos = 0;

                    if(sf::Mouse::getPosition(window).x >= 401)
                    {
                        xpos = sens;
                    }
                    else if(sf::Mouse::getPosition(window).x <= 399) 
                    {
                        xpos = -sens;
                    }

                    if(sf::Mouse::getPosition(window).y >= 301)
                    {
                        ypos = -(sens - .5f);
                    }
                    else if(sf::Mouse::getPosition(window).y <= 299)
                    {
                        ypos = (sens - .5f);
                    }
                    camera.ProcessMouseMovent(xpos, ypos, true);

                    sf::Mouse::setPosition(sf::Vector2i(400, 300), window);
                }
                break;
                case sf::Event::MouseWheelScrolled:
                {
                    float sens = 5.0f;
                    
                    camera.processMouseScroll((float)windowEvent.mouseWheelScroll.delta * sens);
                }
                break;
                default:
                    break;
            }
        }
        //--------------------------------------------------------------
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            window.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tilde))
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            camera.processKeyboard(FORWARD, deltaTime);   
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            camera.processKeyboard(LEFT, deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            camera.processKeyboard(BACKWARD, deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            camera.processKeyboard(RIGHT, deltaTime);

        //--------------------------------------------------------------
        glClearColor(.1f, .1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //--------------------------------------------------------------
        
        lightPos.x = (cos(time.asSeconds() * glm::radians(150.0f)));
        lightPos.y = (sin(time.asSeconds() * glm::radians(150.0f)));
        lightPos.z = (cos(time.asSeconds() * glm::radians(150.0f)) + sin(time.asSeconds() * glm::radians(150.0f)));
        
        //lighting
        shdBase.Use();
        shdBase.setVec3("objectColor", 1.f, .5f, .31f);
        shdBase.setVec3("lightColor", 1.f, 1.f, 1.f);
        shdBase.setVec3("lightPos", lightPos);
        shdBase.setVec3("viewPos", camera.Position);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shdBase.setMat4("projection", projection);
        shdBase.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        shdBase.setMat4("model", model);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //--------------------------------------------------------------
        //lightCube
        lightShd.Use();
        lightShd.setMat4("projection", projection);
        lightShd.setMat4("view", view);
        model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(
            cos(time.asSeconds() * glm::radians(150.0f)),
            sin(time.asSeconds() * glm::radians(150.0f)),
            cos(time.asSeconds() * glm::radians(150.0f)) + sin(time.asSeconds() * glm::radians(150.0f))
        ));

        //model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(.2f));      
        lightShd.setMat4("model", model);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
       

//--------------------------------------------------------------
        window.display();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

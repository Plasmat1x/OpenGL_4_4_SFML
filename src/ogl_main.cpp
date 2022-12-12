#include<GL/glew.h>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<stdio.h>
#include<chrono>
#include<math.h>
#include<string>

#include"Shader.h"
#include"Camera.h"
#include"Model.h"

unsigned int loadTexture(char const* path);

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

    stbi_set_flip_vertically_on_load(false);

    window.setActive(true);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glViewport(0, 0, 800, 600);

    //--------------------------------------------------------------

    Shader baseShd("vertex.glsl", "fragment.glsl");
    Model ourmodel("resources/source/BOSS_model_final.fbx");

    //--------------------------------------------------------------

    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    //--------------------------------------------------------------
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

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
        glClearColor(.5f, .5f, .5f, .5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //--------------------------------------------------------------
        baseShd.Use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        baseShd.setMat4("projection", projection);
        baseShd.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
        model = glm::rotate(model, time.asSeconds() * glm::radians(25.f), glm::vec3(0.f, -1.f, 0.f));
        baseShd.setMat4("model", model);

        ourmodel.Draw(baseShd);

        window.display();
    }

    return 0;
}

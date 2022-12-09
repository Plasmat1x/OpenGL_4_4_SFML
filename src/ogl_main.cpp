#include<GL/glew.h>
#include<SFML/Window.hpp>

#include<printf.h>

int main(int argc, char **argv) 
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4; // Optional
    settings.majorVersion = 4;
    settings.minorVersion = 4;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(800,600), "OpenGL 4.4", sf::Style::Close, settings);

    window.setVerticalSyncEnabled(false);

    GLenum err = glewInit();  

    if(GLEW_OK != err)
    {
        printf("glew init fail");
        return -1;
    }

    window.setActive(true);

    while (window.isOpen())
    {
        sf::Event windowEvent;
        while(window.pollEvent(windowEvent))
        {
            switch (windowEvent.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            default:
                break;
            }
        }

        
        glClearColor(0.5,0.5,0.5,1);
        glClear(GL_COLOR_BUFFER_BIT);



        //draw
        window.display();
    }
    
    return 0;
}

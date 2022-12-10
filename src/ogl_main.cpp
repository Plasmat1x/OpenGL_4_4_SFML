#include<GL/glew.h>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>

#include<printf.h>
#include<chrono>
#include<math.h>

#include"Shader.h"

bool frameDrawingMode = false;

int main()  
{

//--------------------------------------------------------------
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 4;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(800,600), "OpenGL 4.3", sf::Style::Close, settings);

    window.setVerticalSyncEnabled(false);

    GLenum err = glewInit();  

    if(GLEW_OK != err)
    {
        printf("glew init fail");
        return -1;
    }

    window.setActive(true);

    glViewport(0,0,800,600);
//--------------------------------------------------------------

    Shader shdBase("vertex.glsl", "fragment.glsl");

    //init geometry
    float vertices[] = {
        //pos               //color
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,            
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,            
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    //setup buffers
    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1,&VBO);
    
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    //pos attrib
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attrib
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    shdBase.Use();

//--------------------------------------------------------------
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
            
            case sf::Event::Resized:
                glViewport(0,0,window.getSize().x, window.getSize().y);
                break;

            default:
                break;
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tilde))
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

//--------------------------------------------------------------
        glClearColor(0.5,0.5,0.5,1);
        glClear(GL_COLOR_BUFFER_BIT);
//--------------------------------------------------------------
        
        //drawing
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0,3);   
  
//--------------------------------------------------------------
        window.display();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

#include<GL/glew.h>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>

#include<stb_image.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<stdio.h>
#include<chrono>
#include<math.h>
#include<string>

#include"Shader.h"
#include"Camera.h"

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

    window.setActive(true);

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 800, 600);

    //--------------------------------------------------------------

    Shader baseShd("vertex.glsl", "fragment.glsl");
    Shader lightShd("light.vertex.glsl", "light.fragment.glsl");

    glm::vec3 lightPos = glm::vec3(1.2f, 1.f, 2.f);

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
    };

    //setup buffers
    unsigned int VBO;
    unsigned int cubeVAO;

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //--------------------------------------------------------------
    
    unsigned int diffuseMap = loadTexture("container2.png");
    unsigned int specularMap = loadTexture("container2_specular.png");

    baseShd.Use();
    baseShd.setInt("material.diffuse", 0);
    baseShd.setInt("material.specular", 1);
    
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
        
        glm::vec3 pointLightColors[] = {
    glm::vec3(0.2f, 0.2f, 0.6f),
    glm::vec3(0.3f, 0.3f, 0.7f),
    glm::vec3(0.0f, 0.0f, 0.3f),
    glm::vec3(0.4f, 0.4f, 0.4f)
        };

        lightPos.x = (cos(time.asSeconds() * glm::radians(50.0f)));
        lightPos.y = (sin(time.asSeconds() * glm::radians(50.0f)));
        lightPos.z = (cos(time.asSeconds() * glm::radians(50.0f)) + sin(time.asSeconds() * glm::radians(50.0f)));

        //lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

        //lighting
        baseShd.Use();
        baseShd.setVec3("viewPos", camera.Position);
        baseShd.setFloat("material.shininess", 32.f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        baseShd.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        baseShd.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        baseShd.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        baseShd.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // Directional light
        glUniform3f(glGetUniformLocation(baseShd.ID, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(baseShd.ID, "dirLight.ambient"), 0.05f, 0.05f, 0.1f);
        glUniform3f(glGetUniformLocation(baseShd.ID, "dirLight.diffuse"), 0.2f, 0.2f, 0.7);
        glUniform3f(glGetUniformLocation(baseShd.ID, "dirLight.specular"), 0.7f, 0.7f, 0.7f);
        // Point light 1                 baseShd ID
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[0].ambient"), pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[0].diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[0].specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[0].linear"), 0.09);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[0].quadratic"), 0.032);
        // Point light 2                 baseShd ID
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[1].ambient"), pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[1].diffuse"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[1].specular"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[1].linear"), 0.09);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[1].quadratic"), 0.032);
        // Point light 3                 baseShd ID
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[2].ambient"), pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[2].diffuse"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[2].specular"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[2].linear"), 0.09);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[2].quadratic"), 0.032);
        // Point light 4                 baseShd ID
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[3].ambient"), pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[3].diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
        glUniform3f(glGetUniformLocation(baseShd.ID, "pointLights[3].specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[3].linear"), 0.09);
        glUniform1f(glGetUniformLocation(baseShd.ID, "pointLights[3].quadratic"), 0.032);
        // SpotLight                     baseShd ID
        glUniform3f(glGetUniformLocation(baseShd.ID, "spotLight.position"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform3f(glGetUniformLocation(baseShd.ID, "spotLight.direction"), camera.Front.x, camera.Front.y, camera.Front.z);
        glUniform3f(glGetUniformLocation(baseShd.ID, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(baseShd.ID, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(baseShd.ID, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(baseShd.ID, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(baseShd.ID, "spotLight.linear"), 0.009);
        glUniform1f(glGetUniformLocation(baseShd.ID, "spotLight.quadratic"), 0.0032);
        glUniform1f(glGetUniformLocation(baseShd.ID, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
        glUniform1f(glGetUniformLocation(baseShd.ID, "spotLight.outerCutOff"), glm::cos(glm::radians(12.5f)));
        // == ==============================================================================================

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        baseShd.setMat4("projection", projection);
        baseShd.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        baseShd.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // render containers
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            baseShd.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp object(s)
        lightShd.Use();
        lightShd.setMat4("projection", projection);
        lightShd.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightShd.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        window.display();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    bool gEnabled = false;
    if(geometryPath)
        gEnabled = true;

    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    unsigned int vertex; 
    unsigned int fragment;
    unsigned int geometry;

    std::thread vertexThread([&]()->void
    {
        try
        {
            vShaderFile.open(vertexPath);
            std::stringstream vShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            vShaderFile.close();
            vertexCode = vShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::Vertex::FILE_NOT_SUCCESFULLY_READ\n";
        }

        int success;
        char infoLog[512];
        const char* vShaderCode = vertexCode.c_str();

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex,GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ER::SHD::V::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

    });

    std::thread fragmentThread([&]()->void
    {
        try
        {
            fShaderFile.open(fragmentPath);
            std::stringstream fShaderStream;
            fShaderStream << fShaderFile.rdbuf();
            fShaderFile.close();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::Fragment::FILE_NOT_SUCCESFULLY_READ\n";
        }

        int success;
        char infoLog[512];
        const char* fShaderCode = fragmentCode.c_str();

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ER::SHD::F::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    });

    std::thread* geometryThread;

    if(gEnabled)
    {
        geometryThread = new std::thread([&]()->void
        {
            try
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
            catch (std::ifstream::failure e)
            {
                std::cout << "ERROR::SHADER::Geometry::FILE_NOT_SUCCESFULLY_READ\n";
            }

            int success;
            char infoLog[512];
            const char* gShaderCode = geometryCode.c_str();

            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);

            if(!success)
            {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "RE::SHD::G::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
        });
    }

    vertexThread.join();
    fragmentThread.join();
    if(gEnabled)
        geometryThread->join();


    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);

    if(gEnabled)
        glAttachShader(ID, geometry);

    int success;
    char infoLog[512];
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ER::SHD::PROG::LNK_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(gEnabled)
        glDeleteShader(geometry);

    if(geometryThread)
        delete geometryThread; 
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}


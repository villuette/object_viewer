#include "shaderProgram.h"
#include <iostream>
namespace Renderer {
    ShaderProgram::ShaderProgram(const std::string& vertext_shader, const std::string& fragment_shader){
        GLuint vertex_shaderID;
        if(!createShader(vertext_shader, GL_VERTEX_SHADER, vertex_shaderID)){
            std::cerr << "Error creating vertex shader" << std::endl;
            return;
        }
        GLuint fragment_shaderID;
        if(!createShader(fragment_shader, GL_FRAGMENT_SHADER, fragment_shaderID)){
            std::cerr << "Error creating fragment shader" << std::endl;
            return;
        }
        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex_shaderID);
        glAttachShader(m_ID, fragment_shaderID);
        glLinkProgram(m_ID);
        GLint success;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if(!success){
            GLchar infolog[1024];
            glGetProgramInfoLog(m_ID, sizeof(infolog), NULL, infolog);
            std::cerr << "Error linking program: " << infolog << std::endl;
        } else{
            m_isCompiled = true;
        }
        glDeleteShader(vertex_shaderID);
        glDeleteShader(fragment_shaderID);

        
        
    }
    bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID){
        shaderID = glCreateShader(shaderType);
        const char* source_cstr = source.c_str();
        glShaderSource(shaderID, 1, &source_cstr, NULL);
        glCompileShader(shaderID);
        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if(!success){
            GLchar infolog[1024];
            glGetShaderInfoLog(shaderID, sizeof(infolog), NULL, infolog);
            std::cerr << "Error compiling shader: " << infolog << std::endl;
            return false;
        }
        return true;
    }
    ShaderProgram::~ShaderProgram(){
        if(m_isCompiled){
            glDeleteProgram(m_ID);
        }
    }
    void ShaderProgram::use() const {
        glUseProgram(m_ID);
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
        glDeleteProgram(m_ID);
        m_ID = other.m_ID;
        m_isCompiled = other.m_isCompiled;

        other.m_ID = 0;
        other.m_isCompiled = false;
        return *this;
    }
    ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept{
        m_ID = other.m_ID;
        m_isCompiled = other.m_isCompiled;
        other.m_ID = 0;
        other.m_isCompiled = false;
    }
    bool ShaderProgram::isCompiled(){
        return m_isCompiled;
    }
}
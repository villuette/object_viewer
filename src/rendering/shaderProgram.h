#pragma once
#include <string>
#include <glad/glad.h> 
namespace Renderer{
    class ShaderProgram {
        bool m_isCompiled = false; //program correct when is compiled
        GLuint m_ID; //program
        
        public:
        ShaderProgram(const std::string& vertext_shader, const std::string& fragment_shader);
        bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
        ShaderProgram() = delete;
        ShaderProgram(ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&)= delete;
        ShaderProgram& operator=(ShaderProgram&&) noexcept;
        ShaderProgram(ShaderProgram&&) noexcept;
        ~ShaderProgram();
        bool isCompiled() const { return m_isCompiled; }
        void use() const;
        
    };
};

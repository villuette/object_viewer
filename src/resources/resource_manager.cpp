#include "resource_manager.h"
#include "../rendering/shaderProgram.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

ResourceManager::ResourceManager(const std::string &executable_path) {
    // Get the directory of the executable
    size_t lastSlash = executable_path.find_last_of("/\\");
    m_path = executable_path.substr(0, lastSlash);
}
std::string ResourceManager::getFileString(const std::string &relativePath) const {
    std::fstream f;
    f.open(m_path + "/" + relativePath.c_str(),
           std::ios::in | std::ios::binary);
    if (!f.is_open()) {
        std::cerr << "Can't open file: " << m_path + "/" + relativePath.c_str()
                  << std::endl;
        return std::string{};
    }
    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}
std::shared_ptr<Renderer::ShaderProgram>
ResourceManager::loadShaders(const std::string &shaderName,
                            const std::string &vertexPath,
                            const std::string &fragmentPath) {
    std::string vertex_str = getFileString(vertexPath);
    std::string fragment_str = getFileString(fragmentPath);
    //both shaders must exist
    if (vertex_str.empty() || fragment_str.empty()) {
        std::cerr << "No " << (vertex_str.empty() ? "vertex" : "fragment")
                  << " shader" << std::endl;
        return nullptr;
    }
    std::shared_ptr<Renderer::ShaderProgram> &_newShader =
        m_shaderPrograms
            .emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(
                                     vertex_str, fragment_str))
            .first->second;
    if (!_newShader->isCompiled()) {
        std::cerr << "shader was not compiled" << std::endl;
        return nullptr;
    }
    return _newShader;
}
std::shared_ptr<Renderer::ShaderProgram>
ResourceManager::getShaderProgram(const std::string &shaderName) {
    //or better to use `auto` keyword
    ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
    if (it == m_shaderPrograms.end()) {
        std::cerr << " shader cannot be found" << std::endl;
        return nullptr;
    }
    return it->second;
}

#pragma once
#include <map>
#include <memory>
#include <string>
namespace Renderer {
class ShaderProgram;
}

class ResourceManager {
    typedef std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>>
        ShaderProgramsMap;
    ShaderProgramsMap m_shaderPrograms;
    std::string m_path;

  public:
    ResourceManager(const std::string &executable_path);
    std::string getFileString(const std::string &relativePath) const;
    // ResourceManager(const ResourceManager&) = delete;
    // ResourceManager& operator=(const ResourceManager&) = delete;
    // ResourceManager(ResourceManager&&) = delete;

    std::shared_ptr<Renderer::ShaderProgram>
    loadShaders(const std::string &shaderName, const std::string &vertexPath,
               const std::string &fragmentPath);
    std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string&
    shaderName);

    ~ResourceManager() = default;
};
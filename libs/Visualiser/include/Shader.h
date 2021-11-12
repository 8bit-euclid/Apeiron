#pragma once

#include "../../../include/Global.h"
#include "GLDebug.h"
#include "GLTypes.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
//#include "../resources/external/glm/glm.hpp"

namespace Apeiron {

struct ShaderSourceCode
{
  std::string Vertex;
  std::string Fragment;
};

class Shader
{
private:
  UInt RendererID;
  std::string FilePath;
  std::unordered_map<std::string, int> UniformLocationCache;

public:
  Shader(const std::string& _file_path);
  ~Shader();

  inline void Bind() const { GLCall(glUseProgram(RendererID)); }

  inline void Unbind() const { GLCall(glUseProgram(0)); }

  void SetUniform1i(const std::string& _name, int _value);

  void SetUniform1f(const std::string& _name, float _value);

  void SetUniform2f(const std::string& _name, float _value0, float _value1);

  void SetUniform4f(const std::string& _name, float _value0, float _value1, float _value2, float _value3);

  void SetUniformMatrix4f(const std::string& _name, const glm::mat4& _proj_matrix);

private:
  ShaderSourceCode Parse(const std::string& _file_path);

  UInt Compile(UInt _type, const std::string& _source);

  UInt Create(const std::string& vertexShader, const std::string& fragmentShader);

  int GetUniformLocation(const std::string& _name);
};

}

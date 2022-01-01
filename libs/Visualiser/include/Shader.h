#pragma once

#include "../../../include/Global.h"
#include "GLDebug.h"
#include "GLTypes.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Model.h"
#include "Texture.h"

#include <fstream>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Apeiron {

enum class ShaderType
{
  Vertex,
  Geometry,
  Fragment,
  nTypes
};

struct ShaderSourceCode
{
  std::string Vertex;
  std::string Geometry;
  std::string Fragment;
};

class Shader
{
public:
  /** Module Interface ****************************************************************************************************************************************/
  Shader();

  Shader(const std::string& _file_path);

  ~Shader();

  void ReadFromFile(const std::string& _file_path);

  inline void Bind() const { GLCall(glUseProgram(ID)); }

  inline void Unbind() const { GLCall(glUseProgram(0)); }

  void UseModel(const Model& _model);

  void UseMaterial(const Material& _material);

  void UseTexture(const Texture& _texture, const UInt _slot);

  void UseCamera(Camera& _camera);

  void UseLight(const Light& _light);

  void SetDirectionalShadowMap(const UInt _slot);

  void SetDirectionalLightTransform(const glm::mat4& _tranform_matrix);

  /** Setting Shader Uniforms *********************************************************************************************************************************/
  void SetUniform1i(const std::string& _name, GLint _value);

  void SetUniform1f(const std::string& _name, GLfloat _value);

  void SetUniform2f(const std::string& _name, GLfloat _value0, GLfloat _value1);

  void SetUniform3f(const std::string& _name, GLfloat _value0, GLfloat _value1, GLfloat _value2);

  void SetUniform4f(const std::string& _name, GLfloat _value0, GLfloat _value1, GLfloat _value2, GLfloat _value3);

  void SetUniformMatrix4f(const std::string& _name, const glm::mat4& _proj_matrix);

private:
  GLuint ID;
  std::unordered_map<std::string, int> UniformLocationCache;

  /** Shader Parsing, Compilation, Installation, and Deletion *************************************************************************************************/
  ShaderSourceCode Parse(const std::string& _file_path);

  void Create(const std::string& _vertex_shader, const std::string& _geometry_shader, const std::string& _fragment_shader);

  GLuint Compile(GLuint _type, const std::string& _source);

  void Attach(GLuint _program, GLuint _shader);

  void Delete();

  /** Uniform Support Functions *******************************************************************************************************************************/
  int GetUniformLocation(const std::string& _name);
};

}

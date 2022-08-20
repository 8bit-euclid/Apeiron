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

namespace aprn::vis {

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
   /** Public interface
   ************************************************************************************************************************************************************/
 public:
   /** Module Interface */
   Shader();

   Shader(const std::string& file_path);

   ~Shader();

   void Read(const std::string& file_path);

   inline void Bind() const { GLCall(glUseProgram(ID)); }

   inline void Unbind() const { GLCall(glUseProgram(0)); }

   inline void SetWarnings(const bool _is_on) { areWarningsOn = _is_on; }

   void UseModel(const Model& model);

   void UseMaterial(const Material& material);

   void UseTexture(const Texture& texture, const std::string& _uniform_name, const UInt slot);

   void UseCamera(Camera& camera);

   void UseLight(const Light& light);

   void SetDirectionalShadowMap(const UInt slot);

   void SetPointShadowMap(const size_t _i_point_light, const UInt slot);

   void SetPointPosition(const glm::vec3& _position);

   void SetPointFarPlane(GLfloat _far_plane);

   void SetDirectionalLightSpaceMatrix(const glm::mat4& _light_space_matrix);

   void SetPointLightSpaceMatrices(const StaticArray<glm::mat4, 6>& _light_space_matrices);

   /** Setting Shader Uniforms */
   void SetUniform1i(const std::string& name, GLint value);

   void SetUniform1f(const std::string& name, GLfloat value);

   void SetUniform2f(const std::string& name, GLfloat _value0, GLfloat _value1);

   void SetUniform3f(const std::string& name, GLfloat _value0, GLfloat _value1, GLfloat _value2);

   void SetUniform4f(const std::string& name, GLfloat _value0, GLfloat _value1, GLfloat _value2, GLfloat _value3);

   void SetUniformMatrix4f(const std::string& name, const glm::mat4& _proj_matrix);

private:
   /** Private interface
   ************************************************************************************************************************************************************/

   /** Shader Parsing, Compilation, Installation, and Deletion functions. */
   ShaderSourceCode Parse(const std::string& file_path);

   void Create(const std::string& _vertex_shader, const std::string& _geometry_shader, const std::string& _fragment_shader);

   GLuint Compile(GLuint type, const std::string& _source);

   void Attach(GLuint _program, GLuint _shader);

   void Delete();

   /** Uniform Support Functions */
   int GetUniformLocation(const std::string& name);

   /** Private members
   ************************************************************************************************************************************************************/
   GLuint                               ID;
   std::unordered_map<std::string, int> UniformLocationCache;
   bool                                 areWarningsOn{false};
};

}

#include "../include/Shader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Apeiron {

/** Module Interface ******************************************************************************************************************************************/
Shader::Shader()
  : ID(0)
{
  GLCall(ID = glCreateProgram());
  ASSERT(ID, "Could not create shader program.")
}

Shader::Shader(const std::string& _file_path)
  : Shader()
{
  ReadFromFile(_file_path);
}

Shader::~Shader()
{
  Delete();
}

void Shader::ReadFromFile(const std::string &_file_path)
{
  auto source = Parse(_file_path);
  Create(source.Vertex, source.Geometry, source.Fragment);
}

void Shader::UseModel(const Model& _model)
{
  SetUniformMatrix4f("u_model_matrix", _model.GetModelMatrix());
}

void Shader::UseMaterial(const Material& _material)
{
  SetUniform1f("u_material.SpecularIntensity", _material.SpecularIntensity);
  SetUniform1f("u_material.Smoothness", _material.Smoothness);
}

void Shader::UseTexture(const Texture& _texture, const UInt _slot)
{
  SetUniform1i("u_texture", _slot);
  _texture.Bind(_slot);
}

void Shader::UseCamera(Camera& _camera)
{
  _camera.UpdateViewMatrix();
  _camera.UpdateProjectionMatrix();

  SetUniformMatrix4f("u_view_matrix", _camera.GetViewMatrix());
  SetUniformMatrix4f("u_projection_matrix", _camera.GetProjectionMatrix());

  const glm::vec3& camera_position = _camera.GetPosition();
  SetUniform3f("u_camera_position", camera_position.x, camera_position.y, camera_position.z);
}

void Shader::UseLight(const Light& _light)
{
  const LightType type = _light.Type;
  const UInt index = _light.GetIndex();
  std::string uniform_name = type == LightType::Directional ? "u_directional_light" :
                             type == LightType::Point ? "u_point_lights[" + To_Str(index) + "]" :
                             type == LightType::Spot ? "u_spot_lights[" + To_Str(index) + "]" :
                             throw std::invalid_argument("The lighting type was either not recognised or not specified.");
  std::string light_position = type == LightType::Point ? "u_point_light_positions[" + To_Str(index) + "]" :
                               type == LightType::Spot ? "u_spot_light_positions[" + To_Str(index) + "]" :
                               type == LightType::Directional ? "\0" :
                               throw std::invalid_argument("The lighting type was either not recognised or not specified.");
  std::string base_name = type == LightType::Spot ? ".Point" : "\0";

  SetUniform4f(uniform_name + base_name + ".Base.Colour", _light.Colour.r, _light.Colour.g, _light.Colour.b, _light.Colour.a);
  SetUniform1f(uniform_name + base_name + ".Base.AmbientIntensity", _light.AmbientIntensity);
  SetUniform1f(uniform_name + base_name + ".Base.DiffuseIntensity", _light.DiffuseIntensity);

  if(type == LightType::Directional)
  {
    const glm::vec3& direction = static_cast<const DirectionalLight&>(_light).Direction;
    SetUniform3f(uniform_name + ".Direction", direction.x, direction.y, direction.z);
  }
  else if(type == LightType::Point)
  {
    const PointLight& point_light = static_cast<const PointLight&>(_light);

    SetUniform1i("u_point_light_count", _light.GetLightCount());
    SetUniform3f(light_position, point_light.Position.x, point_light.Position.y, point_light.Position.z);
    SetUniform3f(uniform_name + ".AttenuationCoefficients",
                 point_light.AttenuationCoefficients[0], point_light.AttenuationCoefficients[1], point_light.AttenuationCoefficients[2]);
  }
  else if(type == LightType::Spot)
  {
    const SpotLight& spot_light = static_cast<const SpotLight&>(_light);

    SetUniform1i("u_spot_light_count", _light.GetLightCount());
    SetUniform3f(light_position, spot_light.Position.x, spot_light.Position.y, spot_light.Position.z);
    SetUniform3f(uniform_name + ".Point.AttenuationCoefficients",
                 spot_light.AttenuationCoefficients[0], spot_light.AttenuationCoefficients[1], spot_light.AttenuationCoefficients[2]);
    SetUniform3f(uniform_name + ".Direction", spot_light.Direction.x, spot_light.Direction.y, spot_light.Direction.z);
    SetUniform1f(uniform_name + ".CosConeAngle", spot_light.CosConeAngle);
  }
  else EXIT("Cannot yet handle the given light type.")
}

void Shader::SetDirectionalShadowMap(const UInt _slot)
{
  SetUniform1i("u_direc_shadow", _slot);
}

void Shader::SetPointShadowMap(const size_t _i_point_light, const UInt _slot)
{
  SetUniform1i("u_point_lights["+To_Str(_i_point_light)+"].Shadow", _slot);
}

void Shader::SetPointPosition(const glm::vec3& _position)
{
  SetUniform3f("u_plight_position", _position.x, _position.y, _position.z);
}

void Shader::SetPointFarPlane(const float _far_plane)
{
  SetUniform1f("u_plight_far_plane", _far_plane);
}

void Shader::SetDirectionalLightSpaceMatrix(const glm::mat4& _light_space_matrix)
{
  SetUniformMatrix4f("u_dlight_space_matrix", _light_space_matrix);
}

void Shader::SetPointLightSpaceMatrices(const StaticArray<glm::mat4, 6>& _light_space_matrices)
{
  FOR(i, 6) SetUniformMatrix4f("u_plight_space_matrices["+To_Str(i)+"]", _light_space_matrices[i]);
}

/***************************************************************************************************************************************************************
* Setting Shader Uniforms
***************************************************************************************************************************************************************/
void Shader::SetUniform1i(const std::string& _name, GLint _value)
{
  GLCall(glUniform1i(GetUniformLocation(_name), _value));
}

void Shader::SetUniform1f(const std::string& _name, GLfloat _value)
{
  GLCall(glUniform1f(GetUniformLocation(_name), _value));
}

void Shader::SetUniform2f(const std::string& _name, GLfloat _value0, GLfloat _value1)
{
  GLCall(glUniform2f(GetUniformLocation(_name), _value0, _value1));
}

void Shader::SetUniform3f(const std::string& _name, GLfloat _value0, GLfloat _value1, GLfloat _value2)
{
  GLCall(glUniform3f(GetUniformLocation(_name), _value0, _value1, _value2));
}

void Shader::SetUniform4f(const std::string& _name, GLfloat _value0, GLfloat _value1, GLfloat _value2, GLfloat _value3)
{
  GLCall(glUniform4f(GetUniformLocation(_name), _value0, _value1, _value2, _value3));
}

void Shader::SetUniformMatrix4f(const std::string& _name, const glm::mat4& _proj_matrix)
{
  GLCall(glUniformMatrix4fv(GetUniformLocation(_name), 1, GL_FALSE, glm::value_ptr(_proj_matrix)));
}

/***************************************************************************************************************************************************************
* Shader Parsing, Compilation, and Installation
***************************************************************************************************************************************************************/
ShaderSourceCode Shader::Parse(const std::string& _file_path)
{
  std::ifstream stream(_file_path);

  std::string line;
  std::stringstream string_stream[static_cast<UInt>(ShaderType::nTypes)];
  ShaderType type;
  while(getline(stream, line))
  {
    if(line.find("#shader") != std::string::npos)
    {
      if(line.find("vertex") != std::string::npos) type = ShaderType::Vertex;
      else if(line.find("geometry") != std::string::npos) type = ShaderType::Geometry;
      else if(line.find("fragment") != std::string::npos) type = ShaderType::Fragment;
    }
    else string_stream[static_cast<int>(type)] << line << '\n';
  }

  return { string_stream[0].str(), string_stream[1].str(), string_stream[2].str() };
}

void Shader::Create(const std::string& _vertex_shader, const std::string& _geometry_shader, const std::string& _fragment_shader)
{
  // Create and attach vertex shader.
  GLuint vs_id = Compile(GL_VERTEX_SHADER, _vertex_shader);
  Attach(ID, vs_id);

  // Create and attach geometry shader if it exists.
  GLuint gs_id(-1);
  if(!_geometry_shader.empty())
  {
    gs_id = Compile(GL_GEOMETRY_SHADER, _geometry_shader);
    Attach(ID, gs_id);
  }

  // Create and attach fragment shader.
  GLuint fs_id = Compile(GL_FRAGMENT_SHADER, _fragment_shader);
  Attach(ID, fs_id);

  // Link shader program
  GLint result = 0;
  GLchar error_log[1024] = {0};
  GLCall(glLinkProgram(ID));
  GLCall(glGetProgramiv(ID, GL_LINK_STATUS, &result));
  if(!result)
  {
    GLCall(glGetProgramInfoLog(ID, sizeof(error_log), nullptr, error_log));
    EXIT("Could not link shader program.")
  }

  // Validate shader program
  GLCall(glValidateProgram(ID));
  GLCall(glGetProgramiv(ID, GL_VALIDATE_STATUS, &result));
  if(!result)
  {
    GLCall(glGetProgramInfoLog(ID, sizeof(error_log), nullptr, error_log));
    EXIT("Could not validate shader program.")
  }

  // Delete shaders
  GLCall(glDeleteShader(vs_id));
  if(!_geometry_shader.empty()) { GLCall(glDeleteShader(gs_id)); }
  GLCall(glDeleteShader(fs_id));
}

UInt Shader::Compile(unsigned int _type, const std::string& _source)
{
  GLCall(GLuint shader_ID = glCreateShader(_type));
  const GLchar* src = _source.c_str();

  GLCall(glShaderSource(shader_ID, 1, &src, nullptr));
  GLCall(glCompileShader(shader_ID));

  // Check that shader compiled
  GLint result = 0;
  GLchar error_log[1024] = { 0 };
  GLCall(glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &result));
  if(!result)
  {
    GLCall(glGetShaderInfoLog(shader_ID, sizeof(error_log), nullptr, error_log));
    EXIT("Failed to compile ", (_type == GL_VERTEX_SHADER ? "vertex" : _type == GL_GEOMETRY_SHADER ? "geometry" : "fragment"), " shader:\n ", error_log)
  }

  return shader_ID;
}

void Shader::Attach(GLuint _program, GLuint _shader)
{
  GLCall(glAttachShader(_program, _shader));
}

void Shader::Delete()
{
  GLCall(glDeleteProgram(ID));
}

/***************************************************************************************************************************************************************
* Uniform Support Functions
***************************************************************************************************************************************************************/
int Shader::GetUniformLocation(const std::string& _name)
{
  if(UniformLocationCache.contains(_name)) return UniformLocationCache[_name];

  GLCall(int location = glGetUniformLocation(ID, _name.c_str()));
  if(location < 0) WARNING("Could not find the location for uniform ", _name)
  UniformLocationCache[_name] = location;

  return location;
}

}


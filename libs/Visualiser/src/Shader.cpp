#include "../include/Shader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Apeiron {

Shader::Shader(const std::string& _file_path)
  : ID(0), FilePath(_file_path)
{
  // Create shader program
  GLCall(ID = glCreateProgram());
  ASSERT(ID, "Could not create shader program.")

  auto source = Parse(_file_path);
  Create(source.Vertex, source.Fragment);
}

Shader::~Shader()
{
  Delete();
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

void Shader::SetUniform4f(const std::string& _name, GLfloat _value0, GLfloat _value1, GLfloat _value2, GLfloat _value3)
{
  GLCall(glUniform4f(GetUniformLocation(_name), _value0, _value1, _value2, _value3));
}

void Shader::SetUniformMatrix4f(const std::string& _name, const glm::mat4& _proj_matrix)
{
  GLCall(glUniformMatrix4fv(GetUniformLocation(_name), 1, GL_FALSE, glm::value_ptr(_proj_matrix)));
}

/***************************************************************************************************************************************************************
* Vertex Array Supporting Classes
***************************************************************************************************************************************************************/

ShaderSourceCode Shader::Parse(const std::string& _file_path)
{
  std::ifstream stream(_file_path);

  enum class ShaderType { None = -1, Vertex = 0, Fragment = 1 };

  std::string line;
  std::stringstream string_stream[2];
  ShaderType type = ShaderType::None;
  while(getline(stream, line))
  {
    if(line.find("#shader") != std::string::npos)
    {
      if(line.find("vertex") != std::string::npos) type = ShaderType::Vertex;
      else if(line.find("fragment") != std::string::npos) type = ShaderType::Fragment;
    }
    else string_stream[static_cast<int>(type)] << line << '\n';
  }

  return { string_stream[0].str(), string_stream[1].str() };
}

void Shader::Create(const std::string& vertex_shader, const std::string& fragment_shader)
{
  // Create and attach shaders
  GLuint vs_id = Compile(GL_VERTEX_SHADER, vertex_shader);
  GLuint fs_id = Compile(GL_FRAGMENT_SHADER, fragment_shader);
  Attach(ID, vs_id);
  Attach(ID, fs_id);

  GLint result = 0;
  GLchar error_log[1024] = { 0 };

  // Link shader program
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
    EXIT("Failed to compile ", (_type == GL_VERTEX_SHADER ? "vertex" : "fragment"), " shader:\n", error_log)
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

int Shader::GetUniformLocation(const std::string& _name)
{
  if(UniformLocationCache.contains(_name)) return UniformLocationCache[_name];

  GLCall(int location = glGetUniformLocation(ID, _name.c_str()));
  UniformLocationCache[_name] = location;

  return location;
}

}


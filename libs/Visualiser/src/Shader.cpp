#include "../include/Shader.h"

namespace Apeiron{

Shader::Shader(const std::string& _file_path)
  : RendererID(0), FilePath(_file_path)
{
  auto source = Parse(_file_path);
  RendererID = Create(source.Vertex, source.Fragment);
}

Shader::~Shader()
{
  GLCall(glDeleteProgram(RendererID));
}

void Shader::SetUniform1i(const std::string& _name, int _value)
{
  GLCall(glUniform1i(GetUniformLocation(_name), _value));
}

void Shader::SetUniform1f(const std::string& _name, float _value)
{
  GLCall(glUniform1f(GetUniformLocation(_name), _value));
}

void Shader::SetUniform4f(const std::string& _name, float _value0, float _value1, float _value2, float _value3)
{
  GLCall(glUniform4f(GetUniformLocation(_name), _value0, _value1, _value2, _value3));
}

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

UInt Shader::Compile(unsigned int _type, const std::string& _source)
{
  unsigned int id = glCreateShader(_type);
  const char* src = _source.c_str();

  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    EXIT("Failed to compile ", (_type == GL_VERTEX_SHADER ? "vertex" : "fragment"), " shader:\n", message);
  }

  return id;
}

UInt Shader::Create(const std::string& vertexShader, const std::string& fragmentShader)
{
  unsigned int program = glCreateProgram();
  unsigned int vs = Compile(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = Compile(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int Shader::GetUniformLocation(const std::string& _name)
{
  if(UniformLocationCache.contains(_name)) return UniformLocationCache[_name];

  GLCall(int location = glGetUniformLocation(RendererID, _name.c_str()));
//  if(location != -1) ERROR("Could not find location for uniform ", _name, ".")

  UniformLocationCache[_name] = location;
  return location;
}

}


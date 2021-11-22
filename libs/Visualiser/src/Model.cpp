#include "../include/Model.h"

namespace Apeiron {

Model::Model(const DynamicArray<Vertex>& _vertices, const DynamicArray<GLuint>& _indices)
  : VAO(), VBO(_vertices), IBO(_indices)
{
  VertexBufferLayout layout;
  layout.AddEntity<float>(2);
  VAO.AddBuffer(VBO, layout);
}

Model::~Model()
{
  Delete();
}

void Model::Draw()
{
  VAO.Bind();
  IBO.Bind();

  GLCall(glDrawElements(GL_TRIANGLES, IBO.nIndices, GL_UNSIGNED_INT, nullptr));

  IBO.Unbind();
  VAO.Unbind();
}

void Model::Delete()
{
  IBO.Delete();
  IBO.nIndices = 0;
  VBO.Delete();
  VAO.Delete();
}

//void Model::Draw(Shader& _shader, Camera& _camera)
//{
//  _shader.Bind();
//  VAO.Bind();
//
//  UInt n_diffuse = 0;
//  UInt n_specular = 0;
//
//  FOR(i, Textures.size())
//  {
//    std::string number;
//    const std::string& type = Textures[i].Type;
//    if(type == "diffuse") number = std::to_string(n_diffuse++);
//    else if(type == "specular") number = std::to_string(n_specular++);
//
////    Textures[i].Unit(_shader, (type + number).c_str(), i);
//    Textures[i].Bind();
//  }
//}

}


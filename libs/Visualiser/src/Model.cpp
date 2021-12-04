#include "../include/Model.h"

namespace Apeiron {

void Model::Load()
{
  // Compute vertex normals
  Geometry.ComputeVertexNormals();

  // Load vertex and index buffer objects
  VAO.Bind();
  IBO.Init(Geometry.Indices);
  VBO.Init(Geometry.Vertices);

  // Add vertex buffer to vertex array object
  VAO.AddBuffer(VBO, Geometry.GetVertexLayout());
  VAO.Unbind();
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


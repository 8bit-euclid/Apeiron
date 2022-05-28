#pragma once

#include "../../../include/Global.h"
#include "../../DataContainer/include/Array.h"
#include "../../DataContainer/include/List.h"

#include "Action.h"
#include "Buffers.h"
#include "Colour.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

#include <map>
#include <memory>
#include <optional>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace aprn::vis {

class Model
{
 public:
   Model();

   Model(const Model& _model);

   Model(Model&& _model) noexcept;

   ~Model();

   void Init();

   void Animate(const Float _global_time);

   void Render();

   void Delete();

   /** Set Model Attributes
   ************************************************************************************************************************************************************/
   Model& SetTexture(const std::string& _material, const std::string& _item, size_t _index, size_t _resolution);

   Model& SetMaterial(const std::string& _name, Float _specular_intensity, Float _smoothness);

   /** Set Model Actions
   ************************************************************************************************************************************************************/
   Model& Scale(Float _factor, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam = Linear);

   Model& Scale(const SVectorF3& _factors, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam = Linear);

   Model& OffsetPosition(const SVectorF3& _displacement);

   Model& OffsetOrientation(Float _angle, const SVectorF3& _axis);

   Model& Rotate(Float _angle, const SVectorF3& _axis, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam = Linear);

   Model& RotateAbout(Float _angle, const SVectorF3& _axis, const SVectorF3& _refe_point, Float _start_time, Float _end_time,
                      const std::function<Float(Float)>& _reparam = Linear);

   Model& Move(const SVectorF3& _displacement, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam = Linear);

   Model& MoveTo(const SVectorF3& _position, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam = Linear);

   template<class D>
   Model& MoveAlong(const mnfld::Curve<D, 3>& _path, Float _start_time, Float _end_time, const std::function<Float(Float)>& _reparam = Linear);

   /** Getters
   ************************************************************************************************************************************************************/
   const glm::mat4&
   GetModelMatrix() const;

   /** Assignment Operators
   ************************************************************************************************************************************************************/
   Model& operator=(const Model& _model);

   Model& operator=(Model&& _model) noexcept;

 private:
   friend class Visualiser;
   friend class TestTexture2D;
   friend class ModelFactory;
   friend class ActionBase;
   template<ActionType type> friend class Action;

   void ComputeLifespan();

   void Reset();

   void Scale(const glm::vec3& _factors);

   void Translate(const glm::vec3& _displacement);

   void Rotate(const GLfloat _angle, const glm::vec3& _axis);

   /** Model Properties
   ************************************************************************************************************************************************************/
   Mesh                                   Geometry;
   List<SPtr<Model>>                      SubModels;
   std::optional<std::string>             TextureSpec;
   std::optional<Material>                MaterialSpec;
   std::map<ActionType, SPtr<ActionBase>> Actions;
   glm::vec3                              Centroid;
   glm::vec4                              StrokeColour;
   glm::vec4                              FillColour;
   glm::mat4                              ModelMatrix{1.0f};
   glm::mat4                              PreviousActions{1.0f};
   Float                                  EntryTime{Zero};
   Float                                  ExitTime{InfFloat<>};
   bool                                   isInitialised{false};

   /** Data Buffer Objects
   ************************************************************************************************************************************************************/
   VertexArray                            VAO;
   VertexBuffer                           VBO;
   IndexBuffer                            EBO;
   std::optional<ShaderStorageBuffer>     SSBO;
};

}

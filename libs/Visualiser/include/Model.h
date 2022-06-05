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

   Model(const Model& model);

   Model(Model&& model) noexcept;

   ~Model();

   void Init();

   void Update(Float global_time);

   void Render();

   void Delete();

   /** Set Model Attributes
   ************************************************************************************************************************************************************/
   Model& SetColour(const SVectorF3& _rgb_colour);

   Model& SetMaterial(const std::string& name, Float _specular_intensity, Float _smoothness);

   Model& SetTexture(const std::string& _material, const std::string& _item, size_t _index, size_t _resolution);

   /** Set Model Actions
   ************************************************************************************************************************************************************/
   Model& OffsetPosition(const SVectorF3& displacement);

   Model& OffsetOrientation(Float angle, const SVectorF3& axis);

   Model& Scale(Float factor, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& Scale(const SVectorF3& factors, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& MoveBy(const SVectorF3& _displacement, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& MoveTo(const SVectorF3& position, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& MoveAt(const SVectorF3& velocity, Float start_time = Zero, const std::function<Float(Float)>& ramp = Identity);

   Model& Trace(StaticArray<std::function<Float(Float)>, 3> path, Float start_time, Float end_time = InfFloat<>);

   template<class D>
   Model& Trace(const mnfld::Curve<D, 3>& path, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& RotateBy(Float angle, const SVectorF3& axis, Float start_time, Float end_time, const std::function<Float(Float)>& reparam = Linear);

   Model& RotateAt(const SVectorF3& angular_velocity, Float start_time = Zero, const std::function<Float(Float)>& ramp = Identity);

   Model& RevolveBy(Float angle, const SVectorF3& axis, const SVectorF3& refe_point, Float start_time, Float end_time,
                    const std::function<Float(Float)>& reparam = Linear);

   Model& RevolveAt(const SVectorF3& angular_velocity, const SVectorF3& refe_point, Float start_time = Zero,
                    const std::function<Float(Float)>& ramp = Identity);

   /** Getters
   ************************************************************************************************************************************************************/
   const glm::mat4&
   GetModelMatrix() const;

   /** Assignment Operators
   ************************************************************************************************************************************************************/
   Model& operator=(const Model& model);

   Model& operator=(Model&& model) noexcept;

 private:
   friend class Visualiser;
   friend class TestTexture2D;
   friend class ModelFactory;
   friend class ActionBase;
   template<ActionType type> friend class Action;

   void ComputeLifespan();

   void Reset();

   void Scale(const glm::vec3& factors);

   void Translate(const glm::vec3& displacement);

   void Rotate(const GLfloat angle, const glm::vec3& axis);

   /** Model Properties
   ************************************************************************************************************************************************************/
   Mesh                                                         Geometry;
   List<SPtr<Model>>                                            SubModels;
   std::optional<std::string>                                   TextureSpec;
   std::optional<Material>                                      MaterialSpec;
   std::map<ActionType, SPtr<ActionBase>, ActionTypeComparator> Actions;
   glm::vec3                                                    Centroid;
   glm::vec4                                                    StrokeColour;
   glm::vec4                                                    FillColour;
   glm::mat4                                                    ModelMatrix{1.0f};
   glm::mat4                                                    PreviousActions{1.0f};
   Float                                                        EntryTime{Zero};
   Float                                                        ExitTime{InfFloat<>};
   bool                                                         isInitialised{false};

   /** Data Buffer Objects
   ************************************************************************************************************************************************************/
   VertexArray                            VAO;
   VertexBuffer                           VBO;
   IndexBuffer                            EBO;
   std::optional<ShaderStorageBuffer>     SSBO;
};

}

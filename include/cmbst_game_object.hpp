#pragma once

#include "cmbst_model.hpp"

// std
#include <memory>

namespace cmbst
{

  struct Transform2dComponent
  {
    glm::vec2 translation{};
    glm::vec2 scale{1.f, 1.f};
    float rotation;

    glm::mat2 mat2() 
    {
      const float s = glm::sin(rotation);
      const float c = glm::cos(rotation);
      glm::mat2 rotMatrix{ {c, s}, {-s, c} };

      glm::mat2 scaleMat{ {scale.x, .0f}, {.0f, scale.y} };
      return rotMatrix * scaleMat;
    }
  };

  class CmbstGameObject
  {

    public:
      using id_t = unsigned int;

      static CmbstGameObject createGameObject()
      {
        static id_t currentId = 0;
        return CmbstGameObject{currentId++};
      }

      CmbstGameObject(const CmbstGameObject &) = delete;
      CmbstGameObject &operator=(const CmbstGameObject &) = delete;
      CmbstGameObject(CmbstGameObject &&) = default;
      CmbstGameObject &operator=(CmbstGameObject &&) = default;

      id_t getId() { return id; }

      std::shared_ptr<CmbstModel> model{};
      glm::vec3 color{};
      Transform2dComponent transform2d{};

    private:
      CmbstGameObject(id_t objId) : id{objId} {}

      id_t id;
  };
} // namespace cmbst

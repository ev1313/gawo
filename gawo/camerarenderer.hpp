#pragma once

#include <gawo/spriterenderer.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

class CameraRenderer : public SpriteRenderer {
private:
  std::weak_ptr<SceneGraph> translation;
  std::weak_ptr<SceneGraph> rotation;
  std::weak_ptr<SceneGraph> scalation;

  // position, rotation & zoom
  int m_x = 0.0f;
  int m_y = 0.0f;
  float m_r = 0.0f;
  float m_z = 1.0f;

public:
  CameraRenderer();

  /*
      modifies the translation matrix accordingly.
      x and y are cached, so the movement is relative to the old position.
   */
  void move(int x, int y);
  /*
      modifies the rotation matrix accordingly
      r is cached, so the rotation is relative to the old one.
   */
  void rotate(float r);
  /*
      modifies the scalation matrix accordingly
      z is cached, so the scalation is relative to the old one
      TODO: fix this scale@position thing
   */
  void zoom(float z, glm::vec3 pos);
  /*
      returns a weak ptr to the scalation node.
   */
  std::weak_ptr<SceneGraph> getGraph() override;
};

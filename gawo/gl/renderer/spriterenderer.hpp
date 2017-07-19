#pragma once

#include <memory>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

#include <gawo/gl/wrapper/glclasses.hpp>
#include <gawo/datastructures/scenegraph.hpp>
#include <gawo/gl/window/sdlwrapper/sdlwrapper.hpp>
#include "gawo/gl/wrapper/shader.hpp"

/*
  this struct represents a single sprite which can be rendered with the sprite renderer.
 */
struct Sprite {
private:
  unsigned int m_width = 0;
  unsigned int m_height = 0;
  
  // position including "layer" (=z position)
  int m_x = 0;
  int m_y = 0;
  unsigned int m_layer = 0;
  
  std::shared_ptr <Sprite> m_parent;

protected:
public:
  Sprite();
  
  Sprite(std::shared_ptr <Texture> texture);
  
  Sprite(std::shared_ptr <Texture> texture, std::weak_ptr <SceneGraph> parent);
  
  Sprite(std::shared_ptr <Texture> texture, std::shared_ptr <Sprite> parent);
  
  void setPosition(glm::ivec3 pos);
  
  void setPosition(int x, int y);
  
  void setPosition(int x, int y, unsigned int layer);
  
  void setRotation(GLfloat rot);
  
  void setScale(glm::ivec3 scale);
  
  void setScale(unsigned int w, unsigned int h);
  
  void setParent(std::shared_ptr <Sprite> parent);
  
  void setWidth(int w);
  
  void setHeight(int h);
  
  void setX(int x);
  
  void setY(int y);
  
  void setLayer(int z);
  
  unsigned int getWidth();
  
  unsigned int getHeight();
  
  int getX();
  
  int getY();
  
  glm::ivec2 getPosition();
  
  unsigned int getLayer();
  
  /*
    returns the relative position from an absolute input position.

    (goes recursively through all parents and returns absolute-pos)
  */
  glm::ivec2 getRelativePosition(glm::ivec2 absolute);
  
  /*
    returns the absolute position from a given relative input position.

    (goes recursively through all parents and returns relative+pos)
  */
  glm::ivec2 getAbsolutePosition(glm::ivec2 relative);
  
  // the matrices of a sprite are stored in a scenegraph (see scenegraph.hpp)
  std::weak_ptr <SceneGraph> translation;
  std::weak_ptr <SceneGraph> rotation;
  std::weak_ptr <SceneGraph> scalation;
  
  // texture
  std::shared_ptr <Texture> tex;
  
  // returns the position in the scenegraph a child will be appended to
  // obviously the rotation matrix, as scalation is object dependent
  std::weak_ptr <SceneGraph> getGraph();
  
  // renders the tvs matrix (translation * rotation * scalation, so essentially the matrix of the scalation scenegraph node)
  glm::mat4x4 getMatrix();
};

class SpriteRenderer {
private:
  // for storing the simple sprite data
  //(a normal quad with texcoords)
  VAO m_vao;
  VBO m_vbo;
  Shader m_normal_shader;
  GLint m_normal_shader_mvp;
  GLint m_normal_shader_tex;
  Shader m_colorpicking_shader;
  GLint m_colorpicking_shader_mvp;
  GLint m_colorpicking_shader_color;
  
  unsigned int m_click_x = 0;
  unsigned int m_click_y = 0;
  
  std::vector <std::shared_ptr <Texture>> m_textures;

protected:
  // if width||height are 0 the matrix kills everything (singularity in the coordinate system). therefore init it with 1
  unsigned int m_width = 1;
  unsigned int m_height = 1;
  
  std::shared_ptr <Texture> loadTexture(sdl2::SurfacePtr surface);
  
  /*
    root node storing e.g. the projection matrix
  */
  std::shared_ptr <SceneGraph> m_rootnode;

public:
  SpriteRenderer();
  
  SpriteRenderer(std::string normal_vs, std::string normal_fs, std::string color_vs, std::string color_fs);
  
  ~SpriteRenderer();
  
  std::shared_ptr <Texture> loadTexture(std::string path);
  
  std::shared_ptr <Texture> loadFont(std::string text);
  
  /*
      called on resize, recalculates the projections matrix.
  */
  void resize(unsigned int w, unsigned int h);
  
  /*
      called before/for/after "normal" rendering
  */
  void initialize();
  
  void render(Sprite& sprite);
  
  void finish();
  
  /*
      called before "colorpicking" rendering with the coordinates
  */
  void initializeClick(unsigned int x, unsigned int y);
  
  /*
    called for each sprite with the identifier
   */
  void renderClick(Sprite& sprite, unsigned int i);
  
  /*
    finishClick returns the clicked sprite or $FFFFFF if nothing clicked/background clicked/etc
  */
  unsigned int finishClick();
  
  /*
      returns a shared ptr to the root node of the scene graph
   */
  virtual std::weak_ptr <SceneGraph> getGraph();
};

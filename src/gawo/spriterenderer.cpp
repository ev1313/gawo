#include <gawo/spriterenderer.hpp>

// static paths for the shaders
// todo: load from config file in the data dir or so?
static const char* vss_normal = "../data/shaders/sprite.vs";

static const char* fss_normal = "../data/shaders/sprite.fs";

static const char* vss_colorpicking = "../data/shaders/colorpicking.vs";

static const char* fss_colorpicking = "../data/shaders/colorpicking.fs";

Sprite::Sprite() {}

Sprite::Sprite(std::shared_ptr<Texture> texture)
    : Sprite(texture, std::weak_ptr<SceneGraph>()) {}

Sprite::Sprite(std::shared_ptr<Texture> texture, std::weak_ptr<SceneGraph> parent)
    : translation(std::make_shared<SceneGraph>(parent, glm::mat4x4()))
    , rotation(std::make_shared<SceneGraph>(translation, glm::mat4x4()))
    , scalation(std::make_shared<SceneGraph>(rotation, glm::mat4x4()))
    , tex(texture) {
  translation->addChild(rotation);
  rotation->addChild(scalation);

  if (auto p = parent.lock()) {
    p->addChild(translation);
  }

  // this has to be done here, as a nasty hack, because the rotation won't work correctly
  //(m_width and m_height are set in setScale)
  setPosition(m_x, m_y);
  setScale(glm::vec3(m_width, m_height, 0.0f));
}

Sprite::Sprite(std::shared_ptr<Texture> texture, std::weak_ptr<Sprite> parent)
    : m_parent(parent) {

  if (auto p = parent.lock())
    Sprite(texture, p->getGraph());
  else
    Sprite(texture, std::weak_ptr<Sprite>());
}

void Sprite::setPosition(glm::ivec3 pos) {
  m_x = pos[0];
  m_y = pos[1];
  m_layer = pos[2];
  translation->changeMatrix(glm::translate(pos));
}

void Sprite::setPosition(int x, int y) { setPosition(glm::vec3(x, y, m_layer)); }

void Sprite::setPosition(int x, int y, unsigned int layer) { setPosition(glm::vec3(x, y, layer)); }

void Sprite::setRotation(GLfloat rot) {
  auto m = glm::translate(glm::vec3(m_width * 0.5f, m_height * 0.5f, 0.0f));
  m = m * glm::rotate(rot, glm::vec3(0.0f, 0.0f, 1.0f));
  m = glm::translate(m, glm::vec3(m_width * -0.5f, m_height * -0.5f, 0.0f));

  rotation->changeMatrix(m);
}

void Sprite::setScale(glm::ivec3 scale) {
  m_width = scale[0];
  m_height = scale[1];
  scalation->changeMatrix(glm::scale(scale));
}

void Sprite::setScale(unsigned int w, unsigned int h) { setScale(glm::ivec3(w, h, 1.0f)); }

void Sprite::setParent(std::weak_ptr<Sprite> parent) { m_parent = parent; }

glm::ivec2 Sprite::getRelativePosition(glm::ivec2 absolute) {
  if (auto p = m_parent.lock())
    return p->getRelativePosition(absolute - getPosition());

  return absolute - getPosition();
}

glm::ivec2 Sprite::getAbsolutePosition(glm::ivec2 relative) {
  if (auto p = m_parent.lock())
    return p->getRelativePosition(relative + getPosition());

  return relative + getPosition();
}

unsigned int Sprite::getWidth() { return m_width; }

unsigned int Sprite::getHeight() { return m_height; }

int Sprite::getX() { return m_x; }

int Sprite::getY() { return m_y; }

unsigned int Sprite::getLayer() { return m_layer; }

glm::ivec2 Sprite::getPosition() { return glm::ivec2(m_x, m_y); }

std::weak_ptr<SceneGraph> Sprite::getGraph() { return rotation; }

glm::mat4x4 Sprite::getMatrix() {
  if (scalation)
    return scalation->getMatrix();

  return glm::mat4x4();
}

SpriteRenderer::SpriteRenderer() {
  m_rootnode = std::make_shared<SceneGraph>(std::weak_ptr<SceneGraph>(), glm::ortho(0.0f, (float)m_width, 0.0f, (float)m_height, -1000.0f, 1.0f));

  // vertices (also directly used as tex coords)
  GLfloat vertices[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

  m_vbo.init();
  m_vbo.bind();
  m_vbo.fill(GL_STATIC_DRAW, sizeof(vertices), vertices);

  m_vao.init();
  m_vao.bind();
  m_vao.fill(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

  m_vao.unbind();
  m_vbo.unbind();

  // todo check if shader location are there, else warn || error
  m_normal_shader.init();
  m_normal_shader.loadFile(vss_normal, GL_VERTEX_SHADER);
  m_normal_shader.loadFile(fss_normal, GL_FRAGMENT_SHADER);
  m_normal_shader_mvp = m_normal_shader.location("mvp");
  m_normal_shader_tex = m_normal_shader.location("tex");

  m_colorpicking_shader.init();
  m_colorpicking_shader.loadFile(vss_colorpicking, GL_VERTEX_SHADER);
  m_colorpicking_shader.loadFile(fss_colorpicking, GL_FRAGMENT_SHADER);
  m_colorpicking_shader_mvp = m_colorpicking_shader.location("mvp");
  m_colorpicking_shader_color = m_colorpicking_shader.location("color");
}

std::shared_ptr<Texture> SpriteRenderer::loadTexture(sdl2::SurfacePtr surface) {
  auto tex = std::make_shared<Texture>();
  tex->init();
  tex->bind();
  tex->fill(0, 4, surface.get()->w, surface.get()->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface.get()->pixels);
  m_textures.push_back(tex);

  return tex;
}

std::shared_ptr<Texture> SpriteRenderer::loadFont(std::string text) {
  auto font = sdl2::FontPtr(TTF_OpenFont("../data/fonts/cmunti.ttf", 23));
  if (!font) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "failed loading font ../data/fonts/cmunti.ttf");
  }
  auto surface = sdl2::SurfacePtr(TTF_RenderText_Blended(font.get(), text.c_str(), SDL_Color{0, 0, 0, 255}));
  if (!surface) {
    SDL_Log("failed rendering font surface");
    throw std::runtime_error(SDL_GetError());
  }

  return loadTexture(std::move(surface));
}

std::shared_ptr<Texture> SpriteRenderer::loadTexture(std::string path) {
  auto surface = sdl2::SurfacePtr(IMG_Load(path.c_str()));
  if (!surface) {
    std::string error = "couldn't load image '" + path + "'.";
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "loading image failed", error.c_str(), NULL);

    SDL_LogError(SDL_LOG_CATEGORY_ERROR, error.c_str());
  }

  return loadTexture(std::move(surface));
}

void SpriteRenderer::resize(unsigned int w, unsigned int h) {
  m_width = w;
  m_height = h;
  glViewport(0, 0, m_width, m_height);
  m_rootnode->changeMatrix(glm::ortho(0.0f, (float)m_width, 0.0f, (float)m_height, -1000.0f, 1.0f));
}

void SpriteRenderer::initializeClick(unsigned int x, unsigned int y) {
  m_colorpicking_shader.bind();
  m_vao.bind();

  m_click_x = x;
  m_click_y = y;

  // just render a single pixel at the pickpoint
  //(hopefully antialiasing does not fuck up)
  glEnable(GL_SCISSOR_TEST);
  glScissor(m_click_x, m_click_y, 1, 1);

  // white = FFFFFF, therefore it shouldn't be reached
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

unsigned int SpriteRenderer::finishClick() {
  glDisable(GL_SCISSOR_TEST);
  m_vao.unbind();
  m_colorpicking_shader.unbind();

  // get pixels now
  unsigned int data;
  glReadPixels(m_click_x, m_click_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &data);

  glClearColor(0.39f, 0.58f, 0.93f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  return data;
}

void SpriteRenderer::renderClick(Sprite& sprite, unsigned int i) {
  glUniformMatrix4fv(m_colorpicking_shader_mvp, 1, GL_FALSE, glm::value_ptr(sprite.getMatrix()));

  unsigned char r = (i & 0x000000FF) >> 0;
  unsigned char g = (i & 0x0000FF00) >> 8;
  unsigned char b = (i & 0x00FF0000) >> 16;

  glUniform3f(m_colorpicking_shader_color, r / 255.0f, g / 255.0f, b / 255.0f);

  sprite.tex->activate(GL_TEXTURE0);
  glUniform1f(m_normal_shader_tex, 0);

  glDrawArrays(GL_QUADS, 0, 4);

  sprite.tex->unbind();
}

void SpriteRenderer::initialize() {
  m_normal_shader.bind();
  m_vao.bind();
}

void SpriteRenderer::render(Sprite& sprite) {
  glUniformMatrix4fv(m_normal_shader_mvp, 1, GL_FALSE, glm::value_ptr(sprite.getMatrix()));

  sprite.tex->activate(GL_TEXTURE0);
  glUniform1f(m_normal_shader_tex, 0);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  sprite.tex->unbind();
}

void SpriteRenderer::finish() {
  m_vao.unbind();
  m_normal_shader.unbind();
}

std::weak_ptr<SceneGraph> SpriteRenderer::getGraph() { return m_rootnode; }

#pragma once

#include <epoxy/gl.h>

#include <string>

template<GLenum T>
class BufferObject {
private:
  GLuint m_name;
  GLvoid* m_mappointer;

protected:
public:
  BufferObject();
  
  ~BufferObject();
  
  void init();
  
  void bind();
  
  void bind(GLuint index, GLintptr offset = 0, GLsizeiptr size = 0);
  
  void unbind();
  
  void fill(GLenum usage, GLsizei size = 0, GLvoid* data = NULL);
  
  void subfill(GLintptr offset, GLsizei size, const GLvoid* data);
  
  void map(GLenum access);
  
  void unmap();
};

typedef BufferObject <GL_ARRAY_BUFFER> VBO;
typedef BufferObject <GL_ELEMENT_ARRAY_BUFFER> IBO;

class VAO {
private:
  GLuint m_name;

protected:
public:
  VAO();
  
  ~VAO();
  
  void init();
  
  void bind();
  
  void unbind();
  
  void
  fill(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride = 0, const GLvoid* pointer = NULL);
};

class Texture {
private:
  GLuint m_name;

protected:
public:
  Texture();
  
  ~Texture();
  
  void init();
  
  void bind();
  
  void activate(GLenum slot);
  
  void unbind();
  
  void fill(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type,
            const GLvoid* data);
  
  GLuint getName();
};

class Framebuffer {
private:
  GLuint m_name;

protected:
public:
  Framebuffer();
  
  ~Framebuffer();
  
  void init();
  
  void bind();
  
  void unbind();
  
  void attachRenderbuffer(GLenum attachment, GLuint rbo);
  
  void attachTexture(GLenum attachment, GLuint tex);
  
  bool check();
};

class Renderbuffer {
private:
  GLuint m_name;
protected:
public:
  Renderbuffer();
  
  ~Renderbuffer();
  
  void init();
  
  void bind();
  
  void unbind();
  
  void create(GLenum internalformat, GLsizei width, GLsizei height);
  
  GLuint getName();
};

class RenderToTexture {
private:
  GLenum m_attachment;
  Texture m_tex;
  Framebuffer m_fbo;
  Renderbuffer m_rbo;
public:
  RenderToTexture(GLenum attachment);
  
  void init(GLint w, GLint h);
  
  void bind();
  
  void unbind();
  
  GLuint getTexture();
};

/*
  Implementation of the bufferobject
  with a little bit of preprocessor magic.
 */

#define TBufferObject_(pre, post) template <GLenum T> pre BufferObject<T>::post
#define TBufferObject(...) TBufferObject_(__VA_ARGS__)

TBufferObject(, BufferObject)() {}

TBufferObject(, ~BufferObject)() { glDeleteBuffers(1, &m_name); }

TBufferObject(void, init)() { glGenBuffers(1, &m_name); }

TBufferObject(void, bind)() { glBindBuffer(T, m_name); }

TBufferObject(void, bind)(GLuint index, GLintptr offset, GLsizeiptr size) {
  // todo
}

TBufferObject(void, unbind)() { glBindBuffer(T, 0); }

TBufferObject(void, fill)(GLenum usage, GLsizei size, GLvoid* data) { glBufferData(T, size, data, usage); }

TBufferObject(void, subfill)(GLintptr offset, GLsizei size, const GLvoid* data) {
  glBufferSubData(T, offset, size, data);
}

TBufferObject(void, map)(GLenum access) {
  // todo
}

TBufferObject(void, unmap)() {
  // todo
}

void discardLastGlError(bool print = false);

// return false if there's an error
bool checkAndPrintGlError();

void printGlError(GLenum err);

const char* stringFromGlError(GLenum err);

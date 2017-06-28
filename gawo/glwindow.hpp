#pragma once

#include <sig11/sig11.hpp>

namespace gawo {
/*
abstract class that provides an interface for the simple creation of a window with opengl support & event handling
 */

class GLWindow {
private:
protected:
public:
  virtual void open() = 0;
  virtual void poll() = 0;
  virtual void swap() = 0;
  virtual void setTitle(std::string title) = 0;
  virtual bool initializedGL() = 0;
  virtual unsigned int getWidth() = 0;
  virtual unsigned int getHeight() = 0;
  virtual std::string getTitle() = 0;
  virtual uint8_t getMouseButton() = 0;
  virtual uint8_t getMouseState() = 0;

  sig11::signal<void(unsigned int x, unsigned int y)> onResize;
  sig11::signal<void(bool up, bool pressed, unsigned int scancode)> onKey;
  sig11::signal<void(int x, int y, int xrel, int yrel)> onMove;
  sig11::signal<void(bool up, int x, int y, uint8_t button, uint8_t state, uint8_t clicks)> onClick;
};
}

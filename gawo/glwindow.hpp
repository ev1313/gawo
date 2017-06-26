#pragma once

#include "sig11/signal.hpp"

/*
abstract class that provides an interface for the simple creation of a window with opengl support & event handling
 */

class GLWindow {
private:
protected:
public:
  virtual void poll() = 0;
  virtual void swap() = 0;
  virtual void setTitle(std::string title) = 0;
  virtual bool initializedGL() = 0;
  virtual unsigned int getWidth() = 0;
  virtual unsigned int getHeight() = 0;
  virtual std::string getTitle() = 0;
  // TODO
  // button states, etc
  virtual unsigned byte getMouseButton() = 0;
  virtual unsigned byte getMouseState() = 0;

  sig11::signal < void(bool up, bool pressed, unsigned int scancode) onKey;
  sig11::signal<void(int x, int y, int xrel, int yrel)> onMove;
  sig11::signal<void(bool up, int x, int y, unsigned byte button, unsigned byte state, unsigned byte clicks)> onClick;
};

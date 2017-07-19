#include <catch.hpp>

#include "gawo/gl/renderer/spriterenderer.hpp"

#include "glits/glits.hpp"

#if (USE_SDL)
#include "gawo/gl/window/sdlwrapper/sdlwindow.hpp"
#else
#include "x11wrapper/x11window.hpp"
#endif

TEST_CASE("gawo/spriterenderer", "general tests") {

#if (USE_SDL)
  sdl2::SDLWindow wnd;
#else
  x11::X11Window wnd;
#endif
  wnd.open();

  SECTION("check renderer") {
    auto renderer = std::make_shared<SpriteRenderer>();
    REQUIRE(renderer->getGraph().lock());
    // apparently the creation of a texture with w < h crashes on some chipsets/drivers
    glViewport(0, 0, 2048, 2048);
    renderer->resize(2048, 2048);
    RenderToTexture rtt(GL_COLOR_ATTACHMENT0);
    rtt.init(2048, 2048);

    SECTION("load sprite textures") {
      auto t1 = renderer->loadTexture("../gawo-testdata/sprite1.png");
      REQUIRE(t1);
      REQUIRE(glits::check_texture("../gawo-testdata/sprite1.png", t1->getName(), 0.0f, 0.0f, false));

      auto t2 = renderer->loadTexture("../gawo-testdata/sprite2.png");
      REQUIRE(t2);
      REQUIRE(glits::check_texture("../gawo-testdata/sprite2.png", t2->getName(), 0.0f, 0.0f, false));

      SECTION("create Sprite") {
        auto s1 = std::make_shared<Sprite>(t1, renderer->getGraph());

        SECTION("uninitialized sprite") {
          REQUIRE(s1);
          REQUIRE(s1->getWidth() == 0);
          REQUIRE(s1->getHeight() == 0);
          REQUIRE(s1->getLayer() == 0);
          REQUIRE(s1->getX() == 0);
          REQUIRE(s1->getY() == 0);
        }

        SECTION("change sprite-width") {
          s1->setWidth(100);
          REQUIRE(s1->getWidth() == 100);
        }
        SECTION("change sprite-height") {
          s1->setHeight(50);
          REQUIRE(s1->getHeight() == 50);
        }
        SECTION("change sprite-layer") {
          s1->setLayer(1);
          REQUIRE(s1->getLayer() == 1);
        }
        SECTION("change sprite-x") {
          s1->setX(10);
          REQUIRE(s1->getX() == 10);
        }
        SECTION("change sprite-y") {
          s1->setY(20);
          REQUIRE(s1->getY() == 20);
        }

        s1->setWidth(50);
        s1->setHeight(100);
        s1->setLayer(0);
        s1->setX(100);
        s1->setY(50);

        SECTION("render single sprite") {
          glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
          glClear(GL_COLOR_BUFFER_BIT);
          renderer->initialize();
          renderer->render(*s1);
          renderer->finish();

          GLenum buf[1] = {GL_COLOR_ATTACHMENT0};
          glDrawBuffers(1, buf);
          REQUIRE(glits::check_texture("../gawo-testdata/test-spriterenderer-single_sprite.png", rtt.getTexture(), 0.01f, 0.01f, GAWO_GENERATE_TESTDATA));
        }

        auto s2 = std::make_shared<Sprite>(t2, renderer->getGraph());

        s2->setWidth(100);
        s2->setHeight(50);
        s2->setLayer(1);
        s2->setX(500);
        s2->setY(400);

        SECTION("render second sprite (without the first one)") {
          glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
          glClear(GL_COLOR_BUFFER_BIT);
          renderer->initialize();
          renderer->render(*s2);
          renderer->finish();

          GLenum buf[1] = {GL_COLOR_ATTACHMENT0};
          glDrawBuffers(1, buf);
          REQUIRE(glits::check_texture("../gawo-testdata/test-spriterenderer-second_sprite.png", rtt.getTexture(), 0.01f, 0.01f, GAWO_GENERATE_TESTDATA));
        }

        SECTION("render two sprites (without overlap)") {
          glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
          glClear(GL_COLOR_BUFFER_BIT);
          renderer->initialize();
          renderer->render(*s1);
          renderer->render(*s2);
          renderer->finish();

          GLenum buf[1] = {GL_COLOR_ATTACHMENT0};
          glDrawBuffers(1, buf);
          REQUIRE(
              glits::check_texture("../gawo-testdata/test-spriterenderer-two_sprites_no_overlap.png", rtt.getTexture(), 0.01f, 0.01f, GAWO_GENERATE_TESTDATA));
        }

        s2->setX(120);
        s2->setY(60);

        SECTION("render two sprites (with overlap)") {
          glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
          glClear(GL_COLOR_BUFFER_BIT);
          renderer->initialize();
          renderer->render(*s1);
          renderer->render(*s2);
          renderer->finish();

          GLenum buf[1] = {GL_COLOR_ATTACHMENT0};
          glDrawBuffers(1, buf);
          REQUIRE(
              glits::check_texture("../gawo-testdata/test-spriterenderer-two_sprites_with_overlap.png", rtt.getTexture(), 0.01f, 0.01f, GAWO_GENERATE_TESTDATA));
        }
      }
    }
  }
}

#include <catch.hpp>

#include "gawo/datastructures/scenegraph.hpp"

#include <iostream>

TEST_CASE("gawo/scenegraph/", "general tests") {
  glm::mat4x4 a;
  glm::mat4x4 b;
  glm::mat4x4 c;
  glm::mat4x4 d;

  b = glm::translate(glm::mat4x4(), glm::vec3(1.0f, 2.0f, 3.0f));
  c = glm::rotate(glm::mat4x4(), 45.0f, glm::vec3(1.0f, 2.0f, 3.0f));
  d = glm::scale(glm::mat4x4(), glm::vec3(100.0f, 400.0f, 1.0f));

  // first initialize all matrices in the constructor
  auto sc_a = std::make_shared<SceneGraph>(a);
  auto sc_ab = sc_a->addChild(b);
  auto sc_abc = sc_ab.lock()->addChild(c);
  auto sc_abcd = sc_abc.lock()->addChild(d);

  SECTION("matrices initialization in constructor") {
    REQUIRE(sc_a->getMatrix() == a);
    REQUIRE(sc_ab.lock()->getMatrix() == a * b);
    REQUIRE(sc_abc.lock()->getMatrix() == a * b * c);
    REQUIRE(sc_abcd.lock()->getMatrix() == a * b * c * d);
  }

  SECTION("change matrices") {
    sc_ab.lock()->changeMatrix(glm::mat4x4());

    REQUIRE(sc_a->getMatrix() == a);
    REQUIRE(sc_ab.lock()->getMatrix() == a);
    REQUIRE(sc_abc.lock()->getMatrix() == a * c);
    REQUIRE(sc_abcd.lock()->getMatrix() == a * c * d);

    sc_a->changeMatrix(b);

    REQUIRE(sc_a->getMatrix() == b);
    REQUIRE(sc_ab.lock()->getMatrix() == b);
    REQUIRE(sc_abc.lock()->getMatrix() == b * c);
    REQUIRE(sc_abcd.lock()->getMatrix() == b * c * d);

    sc_abcd.lock()->changeMatrix(d);
    sc_abc.lock()->changeMatrix(c);
    sc_ab.lock()->changeMatrix(b);
    sc_a->changeMatrix(a);

    REQUIRE(sc_a->getMatrix() == a);
    REQUIRE(sc_ab.lock()->getMatrix() == a * b);
    REQUIRE(sc_abc.lock()->getMatrix() == a * b * c);
    REQUIRE(sc_abcd.lock()->getMatrix() == a * b * c * d);
  }

  auto e = glm::translate(glm::mat4x4(), glm::vec3(43123.0f, 2222.0f, 44.0f));
  a = glm::translate(glm::mat4x4(), glm::vec3(43.0f, 234.0f, 44.0f));

  auto sc_ac = sc_a->addChild(c);
  auto sc_acb = sc_ac.lock()->addChild(b);
  auto sc_acbd = sc_acb.lock()->addChild(d);
  auto sc_ace = sc_ac.lock()->addChild(e);

  sc_a->changeMatrix(a);
  SECTION("multiple children") {
    REQUIRE(sc_a->getMatrix() == a);
    REQUIRE(sc_ab.lock()->getMatrix() == a * b);
    REQUIRE(sc_abc.lock()->getMatrix() == a * b * c);
    REQUIRE(sc_abcd.lock()->getMatrix() == a * b * c * d);

    REQUIRE(sc_ac.lock()->getMatrix() == a * c);
    REQUIRE(sc_acb.lock()->getMatrix() == a * c * b);
    REQUIRE(sc_acbd.lock()->getMatrix() == a * c * b * d);
    REQUIRE(sc_ace.lock()->getMatrix() == a * c * e);
  }
}

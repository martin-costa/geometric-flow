#include "main.hpp"

int main() {

  // create the window for sfml
  window.create(sf::VideoMode(WIDTH, HEIGHT), "Geometric Flow", sf::Style::Titlebar);

  // set normal coord system ( (0,0) bottom-left and (WIDTH, HEIGHT) top-right )
  window.setView(sf::View(sf::FloatRect(0, HEIGHT, WIDTH, -HEIGHT)));

  // main loop of progam
  while (window.isOpen()) {

    sf::Event evnt;
    while (window.pollEvent(evnt)) { // check if program should close
      if (sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
    }

    framerate(FPS, false);

    mainLoop();

    window.display();
    window.clear(sf::Color::Black);
  }

  return 0;
}

void mainLoop() {

  static sf::Vector2i pos = mouse.pos();

  static bool makingCurve = false;
  if (mouse.clicked()) {
    makingCurve = true;
    curve = Curve();
  }

  if (mouse.down()) {
    if (pos != mouse.pos()) {
      pos = mouse.pos();
      curve.addPoint(pos.x, HEIGHT - pos.y);
    }
  }
  else if (makingCurve) {
    makingCurve = false;
    curve.finishCurve();
  }

  if (!makingCurve) curve.update(1);

  // std::cout << curve.curveLength() << std::endl;
  curve.draw(&window);
}

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

  // std::cout << "\r" << curves.size() << "     ";

  static sf::Vector2i pos = mouse.pos();

  static bool makingCurve = false;
  if (mouse.clicked()) {
    makingCurve = true;
    curves.push_back(Curve());
  }

  if (mouse.down()) {
    if (pos != mouse.pos()) {
      pos = mouse.pos();
      curves.back().addPoint(pos.x, HEIGHT - pos.y);
    }
  }
  else if (makingCurve) {
    makingCurve = false;
    curves.back().finishCurve();
  }

  for (int i = 0; i < curves.size(); i++) {
    if (curves[i].update(1)) curves.erase(curves.begin() + i);
    curves[i].draw(&window);
  }

  for (int i = 0; i < curves.size(); i++) {
    curves[i].draw(&window);
  }
}

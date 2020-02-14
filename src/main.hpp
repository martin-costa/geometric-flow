#include <SFML/Graphics.hpp>
#include <iostream>

#include "inputdetector.hpp"
#include "framerate.hpp"

#include "curve.hpp"

#define WIDTH 900
#define HEIGHT 600

#define FPS 600

int main();
void mainLoop();

std::vector<Curve> curves = std::vector<Curve>(0);

sf::RenderWindow window;

MouseDetector mouse(sf::Mouse::Button::Left, &window);
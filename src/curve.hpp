#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

#include "graphics-vectors.hpp"

// create a curve object
class Curve {
private:

  int pointCount;

  bool finished = false;

  std::vector<sf::Vertex> vertices;

  std::vector<Vector2> vectors;
  std::vector<Vector2> vectors_temp;

  void removeVertex(int i);

  void addVertex(int i, Vector2 pos);

  sf::Color getColor(double kappa);

  int I(int i);

public:

  Curve();

  double curveLength();

  sf::Vector2f& operator[](int i);

  void addPoint(double x, double y);

  void addPoint(sf::Vector2f p);

  void draw(sf::RenderWindow* window);

  void finishCurve();

  void update(double t);
};

double curvature(double a, double b, double c);
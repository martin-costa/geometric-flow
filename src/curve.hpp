#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

#include <iostream>

#include "graphics-vectors.hpp"

#define PI 3.14159

// create a curve object
class Curve {
private:

  int pointCount;

  bool finished = false;

  double length = 0;

  std::vector<sf::Vertex> vertices;

  std::vector<sf::Vertex> normals;

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

  bool update(double t);
};

double curvature(double a, double b, double c);

Vector2 leftNormal(Vector2 forward, Vector2 back);

Vector2 RightNormal(Vector2 forward, Vector2 back);

Vector2 getNormal(Vector2 forward, Vector2 back);
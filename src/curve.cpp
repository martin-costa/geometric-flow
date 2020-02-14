#include "curve.hpp"

int Curve::I(int i) {
  if (i < 0) i += pointCount;
  if (i >= pointCount) i -= pointCount;
  return i;
}

void Curve::removeVertex(int i) {
  vertices.erase(vertices.begin() + I(i));
  vectors.erase(vectors.begin() + I(i));
  vectors_temp.erase(vectors_temp.begin() + I(i));
  pointCount--;
}

void Curve::addVertex(int i, Vector2 pos) {
  vertices.insert(vertices.begin() + I(i), sf::Vertex(sf::Vector2f(pos.x, pos.y)));
  vectors.insert(vectors.begin() + I(i), pos);
  vectors_temp.insert(vectors_temp.begin() + I(i), pos);
  pointCount++;
}

Curve::Curve() {
  this->vertices = std::vector<sf::Vertex>(0);

  this->vectors = std::vector<Vector2>(0);
  this->vectors_temp = std::vector<Vector2>(0);

  this->pointCount = 0;
}

double Curve::curveLength() {
  double length = 0;
  for (int i = 0; i < pointCount - 1; i++) {
    length += magnitude(vectors[I(i + 1)] - vectors[i]);
  }
  return length;
}

sf::Vector2f& Curve::operator[](int i) {
return vertices[I(i)].position;
}

void Curve::addPoint(double x, double y) {
  addPoint(sf::Vector2f(x, y));
}

void Curve::addPoint(sf::Vector2f p) {
  this->vertices.push_back(sf::Vertex(p));

  this->vectors.push_back(Vector2(p.x, p.y));
  this->vectors_temp.push_back(Vector2(p.x, p.y));

  this->pointCount++;
}

void Curve::draw(sf::RenderWindow* window) {
  //window->draw(normals.data(), normals.size(), sf::Lines);
  window->draw(vertices.data(), vertices.size(), sf::LinesStrip);
}

void Curve::finishCurve() {
  this->vertices.push_back(vertices[0]);
  this->finished = true;

  length = curveLength();
}

bool Curve::update(double t) {

  if (!finished) return false;

  if (pointCount < 3) {
    return true;
  }

  double kappaAverage = 0;

  for (int i = 0; i < pointCount; i++) {

    // ensure there aren't too many or too few points
    double a = magnitude(vectors[i] - vectors[I(i - 1)]);
    while (a < 3 || a > 6) {
      if (a < 3) removeVertex(i - 1);
      if (a > 6) addVertex(i, vectors[I(i - 1)] + (vectors[i] - vectors[I(i - 1)]) / 2.0);

      if (pointCount < 3) return true;
      a = magnitude(vectors[i] - vectors[I(i - 1)]);
    }

    double b = magnitude(vectors[i] - vectors[I(i + 1)]);
    while (b < 3 || b > 6) {
      if (b < 3) removeVertex(i + 1);
      if (b > 6) addVertex(i + 1, vectors[i] + (vectors[I(i + 1)] - vectors[i]) / 2.0);

      if (pointCount < 3) return true;
      b = magnitude(vectors[i] - vectors[I(i + 1)]);
    }

    double c = magnitude(vectors[I(i + 1)] - vectors[I(i - 1)]);

    // get the curvature
    double kappa = curvature(a, b, c);
    Vector2 normal = getNormal(vectors[I(i - 1)] - vectors[i], vectors[I(i + 1)] - vectors[i]);

    // move point in direction of curvature proportionally
    if (kappa > 0) {
      vectors_temp[i] = normal * t * 2 * kappa + vectors[i];
      kappaAverage += kappa;
    }
    else
      vectors_temp[i] = vectors[i];

    vertices[i].color = getColor(kappa);
  }

  for (int i = 0; i < pointCount; i++) {
    vectors[i] = vectors_temp[i];
    vertices[i].position = sf::Vector2f(vectors[i].x, vectors[i].y);
  }

  kappaAverage /= (double)pointCount;

  normals = std::vector<sf::Vertex>(pointCount * 2);

  // enlarges the curve
  if (curveLength() <= length) {
    for (int i = 0; i < pointCount; i++) {

      Vector2 back = vectors[I(i - 1)] - vectors[i];
      Vector2 forward = vectors[I(i + 1)] - vectors[i];

      Vector2 normal = leftNormal(forward, back);

      vectors_temp[i] = normal * t * 2 * kappaAverage + vectors[i];

      normals[i * 2 + 0] = sf::Vertex(sf::Vector2f(vectors[i].x, vectors[i].y), vertices[i].color);
      normals[i * 2 + 1] = sf::Vertex(sf::Vector2f(vectors[i].x + normal.x * 10, vectors[i].y + normal.y * 10), sf::Color(0, 0, 0));
    }

    for (int i = 0; i < pointCount; i++) {
      vectors[i] = vectors_temp[i];
      vertices[i].position = sf::Vector2f(vectors[i].x, vectors[i].y);
    }
  }

  vertices[pointCount] = vertices[0];

  return false;
}

// approximates the curvature at point (i) by computing the radius of the circle that goes through
// the points (i-1), (i) and (i+1) and returning the inverse of the radius of this approximate
// osculating circle
double curvature(double a, double b, double c) {
  double s = (a + b + c) / 2.0;
  double A = sqrt(s * (s - a) * (s - b) * (s - c));
  return  (4.0 * A) / (a * b * c);
}

Vector2 leftNormal(Vector2 forward, Vector2 back) {

  double theta = angle(back.rot(-angle(forward)));

  if (theta >= PI) return normalize(forward.rot(theta / 2 - PI));
  else return normalize(-forward.rot(theta / 2));
}

Vector2 RightNormal(Vector2 forward, Vector2 back);

Vector2 getNormal(Vector2 forward, Vector2 back) {

  double theta = angle(back.rot(-angle(forward)));

  if (theta >= PI) return normalize(forward.rot(theta / 2 - PI));
  else return normalize(forward.rot(theta / 2));
}

sf::Color Curve::getColor(double kappa) {
  return sf::Color(255, std::min((int)(1/kappa), 165), 0);
}
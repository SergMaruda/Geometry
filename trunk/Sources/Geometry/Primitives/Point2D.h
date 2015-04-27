#pragma once


class Point2D
  {
  public:
    Point2D();
    Point2D(double x, double y);

    double GetX() const;
    double GetY() const;
    double& operator[](size_t);
    const double& operator[](size_t) const;
    Point2D operator/(double);
    Point2D operator*(double);
    Point2D operator-(const Point2D&) const;
    Point2D operator+(const Point2D&) const;
    void Set(double x, double y);
    double Length();

  private:
    double m_point_2d[2];
  };


typedef Point2D TVector2D;
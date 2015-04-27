#pragma once


class Point2D
  {
  public:
    Point2D();
    double GetX() const;
    double GetY() const;
    double& operator[](size_t);
    const double& operator[](size_t) const;

  private:
    double m_point_2d[2];
  };


#pragma once
#include "Point2D.h"


class Line2D
  {
  public:
    Line2D();

    const Point2D& GetOrigin() const;
    const TVector2D&  GetDirection() const;

    void SetOrigin(const Point2D&);
    void SetDirection(const TVector2D&);

  private:
    Point2D m_origin;
    TVector2D m_direction;
  };
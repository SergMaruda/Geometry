
#pragma once

#include "UIObject.h"
#include "Point2D.h"


class UIPoint: public UIObject
  {
  public:
    UIPoint();
    UIPoint(const Point2D&);
    const Point2D& GetPoint() const;
    void SetPoint(const Point2D&);
    operator CPoint();

  private:
    Point2D m_point;
  };




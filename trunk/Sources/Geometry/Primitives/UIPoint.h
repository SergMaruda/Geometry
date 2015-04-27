
#pragma once

#include "UIObject.h"
#include "Point2D.h"


class UIPoint: public UIObject
  {
  public:
    UIPoint();
    const Point2D& GetPoint() const;
    void SetPoint(const Point2D&);

  private:
    Point2D m_point;
  };




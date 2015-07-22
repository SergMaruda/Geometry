
#pragma once

#include "UIObject.h"
#include "Point2D.h"


class UIPoint: public UIObject
  {
  public:
    UIPoint();
    UIPoint(const Point2D&);
    UIPoint(const UIPoint&);
    const Point2D& GetPoint() const;
    void SetPoint(const Point2D&);
    virtual UIPoint* Clone() const override;
    operator CPoint();

  private:
    Point2D m_point;
  };




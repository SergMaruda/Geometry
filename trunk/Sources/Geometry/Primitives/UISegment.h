#pragma once
#include "UIObject.h"
#include "Segment2D.h"

class UIPoint;

class UISegment: public UIObject
  {
  public:
  UISegment();
  UISegment(const UISegment&);
  ~UISegment();
  UIPoint* GetFirstPoint() const;
  UIPoint* GetSecondPoint() const;
  UISegment* Clone() const override;

  Segment2D GetSegment() const;
  };
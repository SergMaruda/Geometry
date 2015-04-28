#pragma once
#include "UIObject.h"
#include "Segment2D.h"

class UIPoint;

class UISegment: public UIObject
  {
  public:
  UISegment();
  ~UISegment();
  UIPoint* GetFirstPoint() const;
  UIPoint* GetSecondPoint() const;

  Segment2D GetSegment() const;
  };
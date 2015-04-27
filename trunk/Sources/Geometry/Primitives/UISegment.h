#pragma once
#include "UIObject.h"

class UIPoint;

class UISegment: public UIObject
  {
  public:
  UISegment();
  ~UISegment();
  UIPoint* GetFirstPoint();
  UIPoint* GetSecondPoint();
  };
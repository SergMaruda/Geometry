#include "UISegment.h"
#include "UIPoint.h"


UISegment::UISegment()
  {
  _SetNameWithIndex(L"Segment");

  AddChild(new UIPoint);
  AddChild(new UIPoint);
  }

UISegment::~UISegment()
  {
  }

UIPoint* UISegment::GetFirstPoint() const
  {
  return dynamic_cast<UIPoint*>(GetChild(0));
  }

Segment2D UISegment::GetSegment() const
  {
  return Segment2D(GetFirstPoint()->GetPoint(), GetSecondPoint()->GetPoint());
  }

UIPoint* UISegment::GetSecondPoint() const
  {
  return dynamic_cast<UIPoint*>(GetChild(1));
  }


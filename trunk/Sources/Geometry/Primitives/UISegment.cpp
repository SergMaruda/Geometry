#include "UISegment.h"
#include "UIPoint.h"


UISegment::UISegment()
  {
  _SetNameWithIndex(L"Segment");

  AddChild(new UIPoint);
  AddChild(new UIPoint);
  m_color = RGB(0, 128,0);
  }

UISegment::UISegment( const UISegment& i_other):
  UIObject(i_other)
  {
  }

UISegment::~UISegment()
  {
  }

UIPoint* UISegment::GetFirstPoint() const
  {
  return GetChild<UIPoint>(0);
  }

Segment2D UISegment::GetSegment() const
  {
  return Segment2D(GetFirstPoint()->GetPoint(), GetSecondPoint()->GetPoint());
  }

UIPoint* UISegment::GetSecondPoint() const
  {
  return GetChild<UIPoint>(1);
  }

UISegment* UISegment::Clone() const 
  {
  return new UISegment(*this);
  }

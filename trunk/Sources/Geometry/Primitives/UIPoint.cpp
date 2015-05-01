#include "UIPoint.h"
#include "..\Notifications\NotificationCenter.h"

//---------------------------------------------------------
UIPoint::UIPoint()
  {
  _SetNameWithIndex(L"Point");
  }

//---------------------------------------------------------
UIPoint::UIPoint(const Point2D& i_p):
  m_point(i_p)
  {
  _SetNameWithIndex(L"Point");
  }

//---------------------------------------------------------
const Point2D& UIPoint::GetPoint() const
  {
  return m_point;
  }

//---------------------------------------------------------
void UIPoint::SetPoint( const Point2D& i_point)
  {
  m_point = i_point;
  NotificationCenter::Notify(POINT_CHANGED, this);
  }

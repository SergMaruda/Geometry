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

UIPoint::UIPoint( const UIPoint& ip_point):
  UIObject(ip_point),
  m_point(ip_point.m_point)
  {
  }

//---------------------------------------------------------
const Point2D& UIPoint::GetPoint() const
  {
  return m_point;
  }

//---------------------------------------------------------
void UIPoint::SetPoint( const Point2D& i_point)
  {
  NotificationCenter::Notify(BEFORE_POINT_CHANGED, this);
  m_point = i_point;
  NotificationCenter::Notify(POINT_CHANGED, this);
  }

UIPoint* UIPoint::Clone() const 
  {
  return new UIPoint(*this);
  }

UIPoint::operator CPoint()
  {
  return CPoint(m_point[0],m_point[1]);
  }

#include "UIPoint.h"
#include "..\Notifications\NotificationCenter.h"

UIPoint::UIPoint()
  {
  CString num;
  num.Format(L"%d", mg_objects_count);
  m_label = CString(L"Point ") + num;
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
  NotificationCenter::Instance().Notify(POINT_CHANGED, this);
  }

#include "UISegment.h"
#include "UIPoint.h"


UISegment::UISegment()
  {
  CString num;
  num.Format(L"%d", mg_objects_count);
  m_label = CString(L"Segment ") + num;

  AddChild(new UIPoint);
  AddChild(new UIPoint);
  }

UISegment::~UISegment()
  {
  }

UIPoint* UISegment::GetFirstPoint()
  {
  return dynamic_cast<UIPoint*>(GetChild(0));
  }

UIPoint* UISegment::GetSecondPoint()
  {
  return dynamic_cast<UIPoint*>(GetChild(1));
  }


#include "UISegmentRender.h"
#include "..\Primitives\UISegment.h"
#include "..\Primitives\UIPoint.h"


UISegmentRender::UISegmentRender( UISegment* ip_segment):
 RenderTemplate<UISegment>(ip_segment)
  {
  }

void UISegmentRender::Render( CDC* ip_dc)
  {
  int x1 = mp_object->GetFirstPoint()->GetPoint()[0];
  int y1 = mp_object->GetFirstPoint()->GetPoint()[1];

  int x2 = mp_object->GetSecondPoint()->GetPoint()[0];
  int y2 = mp_object->GetSecondPoint()->GetPoint()[1];

  CPen pen;
  CBrush brush;
  brush.CreateSolidBrush(m_color);
  pen.CreatePen(PS_SOLID,m_thickness,m_color);

  auto p_old_pen = ip_dc->SelectObject(&pen);   
  auto p_old_brush = ip_dc->SelectObject(&brush);  

  ip_dc->MoveTo(x1, y1);
  ip_dc->LineTo(x2, y2);
  ip_dc->SelectObject(p_old_pen);
  ip_dc->SelectObject(p_old_brush);

  }

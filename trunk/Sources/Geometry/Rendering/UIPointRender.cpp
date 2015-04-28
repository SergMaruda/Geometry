#include "UIPointRender.h"
#include "..\Primitives\UIPoint.h"
#include "..\Primitives\Point2D.h"
#include "..\GeometryDoc.h"

//---------------------------------------------------------------------------------------------------
UIPointRender::UIPointRender( UIPoint* ip_point ):
  RenderTemplate<UIPoint>(ip_point)
  {
  }

//---------------------------------------------------------------------------------------------------
void UIPointRender::Render( CDC* ip_dc)
  {
  int x = mp_object->GetPoint()[0];
  int y = mp_object->GetPoint()[1];

  CPen pen;
  CBrush brush;
  brush.CreateSolidBrush(m_color);
  pen.CreatePen(PS_SOLID,1,m_color);

  auto p_old_pen = ip_dc->SelectObject(&pen);   
  auto p_old_brush = ip_dc->SelectObject(&brush);  
  int r = m_thickness/2+2;
  ip_dc->Ellipse(x-r,y+r,x+r,y-r);
  ip_dc->SelectObject(p_old_pen);
  ip_dc->SelectObject(p_old_brush);
  }



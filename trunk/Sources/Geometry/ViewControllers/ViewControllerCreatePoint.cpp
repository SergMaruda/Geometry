#include "ViewControllerCreatePoint.h"
#include "..\Primitives\UIPoint.h"
#include "..\GeometryDoc.h"
#include "..\GeometryView.h"

ViewControllerCreatePoint::ViewControllerCreatePoint(CGeometryView* ip_view):
  mp_view(ip_view),
  m_active(true)
  {
  }

//--------------------------------------------------------------------------------------
void ViewControllerCreatePoint::OnLButtonDown( UINT nFlags, CPoint point )
  {
  auto& root = mp_view->GetDocument()->GetRootObject();
  UIPoint* p_point = new UIPoint;
  double point_dbl[3] = {point.x, point.y, 0.};
  p_point->SetPoint(Point2D(point_dbl[0],point_dbl[1]));
  root.AddChild(p_point);
  }

//--------------------------------------------------------------------------------------
void ViewControllerCreatePoint::OnLButtonUp( UINT nFlags, CPoint point )
  {
  }

//--------------------------------------------------------------------------------------
void ViewControllerCreatePoint::OnLButtonDblClk( UINT nFlags, CPoint point )
  {
  m_active = false;
  }

//--------------------------------------------------------------------------------------
void ViewControllerCreatePoint::OnMouseMove( UINT nFlags, CPoint point )
  {
  }

//--------------------------------------------------------------------------------------
HCURSOR ViewControllerCreatePoint::GetCursor()
  {
  return LoadCursor(0, IDC_CROSS);
  }

bool ViewControllerCreatePoint::IsActive()
  {
  return m_active;
  }

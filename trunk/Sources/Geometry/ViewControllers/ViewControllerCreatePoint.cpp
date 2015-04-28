#include "ViewControllerCreatePoint.h"
#include "..\Primitives\UIPoint.h"
#include "..\GeometryDoc.h"
#include "..\GeometryView.h"

ViewControllerCreatePoint::ViewControllerCreatePoint():
  m_active(true)
  {
  }

//--------------------------------------------------------------------------------------
void ViewControllerCreatePoint::OnLButtonDown( UINT nFlags, CPoint point )
  {
  auto p_doc = CGeometryDoc::GetActive();
  auto& root = p_doc->GetRootObject();
  double point_dbl[3] = {point.x, point.y, 0.};
  UIPoint* p_point = new UIPoint(Point2D(point_dbl[0],point_dbl[1]));
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

#include "ViewControllerMovePoint.h"
#include "..\Primitives\Point2D.h"
#include "..\GeometryView.h"
#include "..\GeometryDoc.h"
#include "..\Primitives\UIPoint.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ViewControllerMovePoint::ViewControllerMovePoint(CGeometryView* ip_view):
  mp_view(ip_view)
  {
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ViewControllerMovePoint::OnLButtonDown( UINT nFlags, CPoint point )
  {
  m_start_point = point;
  auto p_picked = _GetPickedPoint(point);
  CGeometryDoc::GetActive()->DeselectAllObjects();
  CGeometryDoc::GetActive()->SelectObject(p_picked);
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ViewControllerMovePoint::OnLButtonUp( UINT nFlags, CPoint point )
  {
  
  }

void ViewControllerMovePoint::OnLButtonDblClk( UINT nFlags, CPoint point )
  {
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ViewControllerMovePoint::OnMouseMove( UINT nFlags, CPoint point )
  {
  auto p_doc = mp_view->GetDocument();
  auto& root_object = p_doc->GetRootObject();

  auto p_picked = _GetPickedPoint(point);

  bool lb_down = (nFlags&MK_LBUTTON);

  if(!lb_down)
    mp_picked = p_picked;

  if(mp_picked && lb_down)
    {
    CPoint p = point-m_start_point;
    Point2D diff(p.x, p.y);

    mp_picked->SetPoint(mp_picked->GetPoint()+diff);
    m_start_point = point;
    }

  }

//--------------------------------------------------------------------------------------
bool ViewControllerMovePoint::IsActive()
  {
  return m_active;
  }

//--------------------------------------------------------------------------------------
HCURSOR ViewControllerMovePoint::GetCursor()
  {
  return LoadCursor(0, mp_picked?IDC_HAND:IDC_ARROW);
  }

//------------------------------------------------------------------------------------------------------
UIPoint* ViewControllerMovePoint::_GetPickedPoint( const CPoint& i_pt)
  {
  auto p_doc = mp_view->GetDocument();
  auto& root_object = p_doc->GetRootObject();

  Point2D pt(i_pt.x, i_pt.y);

  UIPoint* p_picked_point(nullptr);

  for(size_t i = 0; i < root_object.GetNumChilds(); ++i)
    {
    auto p_point = dynamic_cast<UIPoint*>(root_object.GetChild(i));

    auto diff = (p_point->GetPoint() - pt).Length();

    if(diff <3)
      {
      p_picked_point  = p_point;
      break;
      }
    }
  return p_picked_point;
  }

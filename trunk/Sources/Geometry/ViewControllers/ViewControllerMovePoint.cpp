#include "ViewControllerMovePoint.h"
#include "..\Primitives\Point2D.h"
#include "..\GeometryView.h"
#include "..\GeometryDoc.h"
#include "..\Primitives\UIPoint.h"
#include "..\Primitives\UISegment.h"
#include "..\Primitives\Segment2D.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ViewControllerMovePoint::ViewControllerMovePoint(CGeometryView* ip_view):
  mp_view(ip_view)
  {
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ViewControllerMovePoint::OnLButtonDown( UINT nFlags, CPoint point )
  {
  auto p_doc = CGeometryDoc::GetActive();
  m_start_point = point;
  IUIObject* p_picked = _GetPickedPoint(point, &p_doc->GetRootObject());
  if(!p_picked)
    p_picked = _GetPickedSegment(point, &p_doc->GetRootObject());

  bool m_ctrl_pressed = (GetAsyncKeyState(VK_CONTROL) < 0);
  if(!m_ctrl_pressed)
    p_doc->DeselectAllObjects();

  p_doc->SelectObject(p_picked);
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

  IUIObject* p_picked = _GetPickedPoint(point, &p_doc->GetRootObject());
  if(!p_picked)
    p_picked = _GetPickedSegment(point, &p_doc->GetRootObject());

  bool lb_down = (nFlags&MK_LBUTTON);

  if(!lb_down)
    mp_picked = p_picked;

  auto p_picked_point = dynamic_cast<UIPoint*>(mp_picked);

  if(p_picked_point && lb_down)
    {
    CPoint p = point-m_start_point;
    Point2D diff(p.x, p.y);

    p_picked_point->SetPoint(p_picked_point->GetPoint()+diff);
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
UIPoint* ViewControllerMovePoint::_GetPickedPoint( const CPoint& i_pt, IUIObject* ip_root)
  {
  auto p_doc = mp_view->GetDocument();
  auto& root_object = p_doc->GetRootObject();

  Point2D pt(i_pt.x, i_pt.y);

  for(size_t i = 0; i < ip_root->GetNumChilds(); ++i)
    {
    auto p_child = ip_root->GetChild(i);
    auto p_point = dynamic_cast<UIPoint*>(p_child);
    if(p_point)
      {
      auto diff = (p_point->GetPoint() - pt).Length();

      if(diff <3)
        {
        return p_point;
        }
      }
    else
      {
      auto res = _GetPickedPoint(i_pt, p_child);
      if(res)
        return res;
      }
    }
  return nullptr;
  }

//------------------------------------------------------------------------------------------------------
UISegment* ViewControllerMovePoint::_GetPickedSegment( const CPoint& i_pt, IUIObject* ip_root)
  {
  auto p_doc = mp_view->GetDocument();
  auto& root_object = p_doc->GetRootObject();

  Point2D pt(i_pt.x, i_pt.y);

  for(size_t i = 0; i < ip_root->GetNumChilds(); ++i)
    {
    auto p_child = ip_root->GetChild(i);
    auto p_segment = dynamic_cast<UISegment*>(p_child);
    if(p_segment)
      {
      Segment2D segm(p_segment->GetFirstPoint()->GetPoint(),p_segment->GetSecondPoint()->GetPoint());

      auto diff = segm.DistanceToPoint(pt);

      if(diff <3)
        {
        return p_segment;
        }
      }
    else
      {
      auto res = _GetPickedSegment(i_pt, p_child);
      if(res)
        return res;
      }
    }
  return nullptr;
  }


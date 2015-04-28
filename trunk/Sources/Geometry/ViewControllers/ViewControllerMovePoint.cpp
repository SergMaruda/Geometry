#include "ViewControllerMovePoint.h"
#include "..\Primitives\Point2D.h"
#include "..\GeometryView.h"
#include "..\GeometryDoc.h"
#include "..\Primitives\UIPoint.h"
#include "..\Primitives\UISegment.h"
#include "..\Primitives\Segment2D.h"


//------------------------------------------------------------------------------------------------------
template<class TObjectType>
TObjectType* _GetPickedObjectTmpl( const CPoint& i_pt, IUIObject* ip_root, std::function<bool (TObjectType*, Point2D&)> i_length_pick_criteria)
  {
  auto p_doc = CGeometryDoc::GetActive();
  auto& root_object = p_doc->GetRootObject();

  Point2D pt(i_pt.x, i_pt.y);

  for(size_t i = 0; i < ip_root->GetNumChilds(); ++i)
    {
    auto p_child = ip_root->GetChild(i);
    auto p_object = dynamic_cast<TObjectType*>(p_child);
    if(p_object)
      {
      if(i_length_pick_criteria(p_object, pt))
        {
        return p_object;
        }
      }
    else
      {
      auto res = _GetPickedObjectTmpl(i_pt, p_child, i_length_pick_criteria);
      if(res)
        return res;
      }
    }
  return nullptr;
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ViewControllerMovePoint::ViewControllerMovePoint()
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
  auto p_doc = CGeometryDoc::GetActive();
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
  auto pick_segment = [](UIPoint* ip_point, Point2D& i_point)
    {
    auto diff = (ip_point->GetPoint() - i_point).Length();
    return diff < 3;
    };

  return _GetPickedObjectTmpl<UIPoint>(i_pt, ip_root, pick_segment);
  }

//------------------------------------------------------------------------------------------------------
UISegment* ViewControllerMovePoint::_GetPickedSegment( const CPoint& i_pt, IUIObject* ip_root)
  {
  auto pick_segment = [](UISegment* ip_segm, Point2D& i_point)
    {
    Segment2D segm(ip_segm->GetFirstPoint()->GetPoint(),ip_segm->GetSecondPoint()->GetPoint());
    auto diff = segm.DistanceToPoint(i_point);
    return diff < 3;
    };

  return _GetPickedObjectTmpl<UISegment>(i_pt, ip_root, pick_segment);
  }


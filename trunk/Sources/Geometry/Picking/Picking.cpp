#include "Picking.h"
#include "..\Primitives\IUIObject.h"
#include "..\Primitives\UISegment.h"
#include "..\Primitives\Segment2D.h"
#include <functional>
#include "..\Primitives\UIPoint.h"
#include "..\GeometryDoc.h"


//------------------------------------------------------------------------------------------------------
template<class TObjectType>
TObjectType* GetPickedObjectTmpl(const CPoint& i_pt, TIUIObjectPtr ip_root, std::function<bool (TObjectType*, Point2D&)> i_length_pick_criteria)
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
      auto res = GetPickedObjectTmpl(i_pt, p_child, i_length_pick_criteria);
      if(res)
        return res;
      }
    }
  return nullptr;
  }

//------------------------------------------------------------------------------------------------------
UIPoint* GetPickedPoint( const CPoint& i_pt, TIUIObjectPtr ip_root)
  {
  auto pick_segment = [](UIPoint* ip_point, Point2D& i_point) -> bool
    {
    auto diff = (ip_point->GetPoint() - i_point).Length();
    return diff < 3;
    };

  return GetPickedObjectTmpl<UIPoint>(i_pt, ip_root, pick_segment);
  }

//------------------------------------------------------------------------------------------------------
UISegment* GetPickedSegment( const CPoint& i_pt, TIUIObjectPtr ip_root)
  {
  auto pick_segment = [](UISegment* ip_segm, Point2D& i_point) -> bool
    {
    Segment2D segm(ip_segm->GetFirstPoint()->GetPoint(),ip_segm->GetSecondPoint()->GetPoint());
    auto diff = segm.DistanceToPoint(i_point);
    return diff < 3;
    };

  return GetPickedObjectTmpl<UISegment>(i_pt, ip_root, pick_segment);
  }

//------------------------------------------------------------------------------------------------------
TIUIObjectPtr GetPickedObject( const CPoint& i_pt, TIUIObjectPtr ip_root )
  {
  TIUIObjectPtr p_picked = GetPickedPoint(i_pt, ip_root);
  if(!p_picked)
    p_picked = GetPickedSegment(i_pt, ip_root);

  return p_picked;
  }

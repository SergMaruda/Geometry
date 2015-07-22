#include "ViewControllerMovePoint.h"
#include "..\Primitives\Point2D.h"
#include "..\GeometryView.h"
#include "..\GeometryDoc.h"
#include "..\Primitives\UIPoint.h"
#include "..\Primitives\UISegment.h"
#include "..\Primitives\Segment2D.h"
#include "..\UndoRedo\UndoRedoManager.h"


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ViewControllerMovePoint::ViewControllerMovePoint():
  m_point_set(false),
  m_active(true)
  {
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ViewControllerMovePoint::OnLButtonDown( UINT nFlags, CPoint point )
  {
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ViewControllerMovePoint::OnLButtonUp( UINT nFlags, CPoint point )
  {
  UndoRedoManager::GetInstance().BlockUndoRedo(false);
  m_active = false;
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ViewControllerMovePoint::OnLButtonDblClk( UINT nFlags, CPoint point )
  {
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ViewControllerMovePoint::OnMouseMove( UINT nFlags, CPoint point )
  {
  bool point_set = m_point_set;
  if(!m_point_set)
    {
    m_point_set = true;
    m_start_point = point;
    }

  auto p_doc = CGeometryDoc::GetActive();
  auto& root_object = p_doc->GetRootObject();

  auto p_picked = p_doc->GetPickedObject();
  
  bool lb_down = (nFlags&MK_LBUTTON);

  auto p_point = dynamic_cast<UIPoint*>(p_picked);

  if(p_point && lb_down)
    {
    CPoint p = point-m_start_point;
    Point2D diff(p.x, p.y);

    p_point->SetPoint(p_point->GetPoint()+diff);
    m_start_point = point;
    }

  if(point_set != m_point_set)
    {
    UndoRedoManager::GetInstance().BlockUndoRedo(true);
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
  return LoadCursor(0, IDC_HAND);
  }

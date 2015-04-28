#include "ViewControllerSelectObject.h"
#include "..\Primitives\Point2D.h"
#include "..\GeometryView.h"
#include "..\GeometryDoc.h"
#include "..\Primitives\UIPoint.h"
#include "..\Primitives\UISegment.h"
#include "..\Primitives\Segment2D.h"
#include "..\Picking\Picking.h"



//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ViewControllerSelectObject::ViewControllerSelectObject():
  m_active(true)
  {
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ViewControllerSelectObject::OnLButtonDown( UINT nFlags, CPoint point )
  {
  auto p_doc = CGeometryDoc::GetActive();

  if(p_doc->GetPickedObject())
    {
    bool m_ctrl_pressed = (GetAsyncKeyState(VK_CONTROL) < 0);
    if(!m_ctrl_pressed)
      p_doc->DeselectAllObjects();

    p_doc->SelectObject(p_doc->GetPickedObject());
    }
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ViewControllerSelectObject::OnLButtonUp( UINT nFlags, CPoint point )
  {
  }

void ViewControllerSelectObject::OnLButtonDblClk( UINT nFlags, CPoint point )
  {
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ViewControllerSelectObject::OnMouseMove( UINT nFlags, CPoint point )
  {
  auto p_doc = CGeometryDoc::GetActive();
  auto& root_object = p_doc->GetRootObject();
  auto p_picked = GetPickedObject(point, &p_doc->GetRootObject());
  p_doc->SetPickedObject(p_picked );
  }

//--------------------------------------------------------------------------------------
bool ViewControllerSelectObject::IsActive()
  {
  return m_active;
  }

//--------------------------------------------------------------------------------------
HCURSOR ViewControllerSelectObject::GetCursor()
  {
  auto p_doc = CGeometryDoc::GetActive();
  return LoadCursor(0, p_doc->GetPickedObject() ? IDC_HAND:IDC_ARROW);
  }

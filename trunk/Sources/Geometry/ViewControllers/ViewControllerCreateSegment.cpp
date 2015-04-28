#include "ViewControllerCreateSegment.h"
#include "..\Primitives\UISegment.h"
#include "..\Primitives\Point2D.h"
#include "..\Primitives\UIPoint.h"
#include "..\GeometryDoc.h"



ViewControllerCreateSegment::ViewControllerCreateSegment( CGeometryView* ip_view):
  mp_view(ip_view),
  m_active(true),
  mp_created_segment(nullptr)
  {
  }

void ViewControllerCreateSegment::OnLButtonDown( UINT nFlags, CPoint p)
  {
  if(!mp_created_segment)
    {
    mp_created_segment = new UISegment; 
    mp_created_segment->GetFirstPoint()->SetPoint(Point2D(p.x, p.y));
    mp_created_segment->GetSecondPoint()->SetPoint(Point2D(p.x, p.y));
    CGeometryDoc::GetActive()->GetRootObject().AddChild(mp_created_segment);
    }
  }

void ViewControllerCreateSegment::OnLButtonUp( UINT nFlags, CPoint point )
  {
  if(mp_created_segment)
    {
    auto p1 = mp_created_segment->GetFirstPoint();
    auto p2 = mp_created_segment->GetSecondPoint();
    if((p1->GetPoint() - p2->GetPoint()).Length() <1)
      {
      CGeometryDoc::GetActive()->GetRootObject().DeleteChild(mp_created_segment);
      }
    }

  mp_created_segment = nullptr;
  }

void ViewControllerCreateSegment::OnLButtonDblClk( UINT nFlags, CPoint point )
  {
  m_active = false;
  }

void ViewControllerCreateSegment::OnMouseMove( UINT nFlags, CPoint p )
  {
  if(mp_created_segment)
    {
    mp_created_segment->GetSecondPoint()->SetPoint(Point2D(p.x, p.y));
    }
  }

HCURSOR ViewControllerCreateSegment::GetCursor()
  {
  return LoadCursor(0, IDC_CROSS);
  }

bool ViewControllerCreateSegment::IsActive()
  {
  return m_active;
  }


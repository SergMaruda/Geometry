#pragma once

#pragma once
#include "IViewController.h"

class CGeometryView;
class UISegment;

class ViewControllerCreateSegment: public IViewController
  {
  public:
    ViewControllerCreateSegment(CGeometryView*);

    virtual void OnLButtonDown( UINT nFlags, CPoint point );
    virtual void OnLButtonUp( UINT nFlags, CPoint point );
    virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

    virtual void OnMouseMove( UINT nFlags, CPoint point );

    virtual HCURSOR GetCursor();
    virtual bool IsActive();

  private:
    CGeometryView* mp_view;
    bool m_active;
    UISegment* mp_created_segment;
  };
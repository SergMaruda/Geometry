#pragma once

#pragma once
#include "IViewController.h"
#include "ViewControllerMovePoint.h"
#include <memory>

class CGeometryView;
class UISegment;

class ViewControllerCreateSegment: public IViewController
  {
  public:
    ViewControllerCreateSegment();

    virtual void OnLButtonDown( UINT nFlags, CPoint point );
    virtual void OnLButtonUp( UINT nFlags, CPoint point );
    virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

    virtual void OnMouseMove( UINT nFlags, CPoint point );

    virtual HCURSOR GetCursor();
    virtual bool IsActive();

  private:
    bool m_active;
    UISegment* mp_created_segment;
    ViewControllerMovePoint m_ctrl;
  };
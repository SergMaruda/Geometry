#pragma once
#include "IViewController.h"

class CGeometryView;
class UIPoint;
struct IUIObject;
class  UISegment;
  
class ViewControllerMovePoint:public IViewController
  {
  public:
    ViewControllerMovePoint();
  virtual void OnLButtonDown( UINT nFlags, CPoint point );

  virtual void OnLButtonUp( UINT nFlags, CPoint point );
  virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
  virtual void OnMouseMove( UINT nFlags, CPoint point );
  virtual bool IsActive();

  virtual HCURSOR GetCursor();
  private:

    CPoint m_start_point;
    bool m_active;
    bool m_point_set;
  };
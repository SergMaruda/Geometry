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
    UIPoint* _GetPickedPoint(const CPoint&, IUIObject* ip_root);
    UISegment* _GetPickedSegment( const CPoint& i_pt, IUIObject* ip_root);

    CPoint m_start_point;
    IUIObject* mp_picked;
    bool m_active;
  };
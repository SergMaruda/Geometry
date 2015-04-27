#pragma once
#include "IViewController.h"

class CGeometryView;
class UIPoint;
struct IUIObject;

class ViewControllerMovePoint:public IViewController
  {
  public:
    ViewControllerMovePoint(CGeometryView*);
  virtual void OnLButtonDown( UINT nFlags, CPoint point );

  virtual void OnLButtonUp( UINT nFlags, CPoint point );
  virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
  virtual void OnMouseMove( UINT nFlags, CPoint point );
  virtual bool IsActive();

  virtual HCURSOR GetCursor();
  private:
    UIPoint* _GetPickedPoint(const CPoint&, IUIObject* ip_root);

    CGeometryView* mp_view;
    CPoint m_start_point;
    UIPoint* mp_picked;
    bool m_active;
  };
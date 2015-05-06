#pragma once
#include "IViewController.h"

class CGeometryView;
class UIPoint;

class ViewControllerCreatePoint: public IViewController
  {
  public:
  ViewControllerCreatePoint();

  virtual void OnLButtonDown( UINT nFlags, CPoint point );
  virtual void OnLButtonUp( UINT nFlags, CPoint point );
  virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

  virtual void OnMouseMove( UINT nFlags, CPoint point );

  virtual HCURSOR GetCursor();
  virtual bool IsActive();

  private:
    UIPoint* mp_created_point;
    bool m_active;
  };
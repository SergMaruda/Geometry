#pragma once
#include "IViewController.h"

class CGeometryView;


class ViewControllerCreatePoint: public IViewController
  {
  public:
  ViewControllerCreatePoint(CGeometryView*);

  virtual void OnLButtonDown( UINT nFlags, CPoint point );
  virtual void OnLButtonUp( UINT nFlags, CPoint point );
  virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

  virtual void OnMouseMove( UINT nFlags, CPoint point );

  virtual HCURSOR GetCursor();
  virtual bool IsActive();

  private:
    CGeometryView* mp_view;
    bool m_active;
  };
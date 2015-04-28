#pragma once
#include "IViewController.h"

class CGeometryView;
class UIPoint;
struct IUIObject;
class  UISegment;
  
class ViewControllerSelectObject:public IViewController
  {
  public:
    ViewControllerSelectObject();
  virtual void OnLButtonDown( UINT nFlags, CPoint point );

  virtual void OnLButtonUp( UINT nFlags, CPoint point );
  virtual void OnLButtonDblClk( UINT nFlags, CPoint point );
  virtual void OnMouseMove( UINT nFlags, CPoint point );
  virtual bool IsActive();

  virtual HCURSOR GetCursor();

  private:
    bool m_active;
  };
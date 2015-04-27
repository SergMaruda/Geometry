#pragma once


interface IViewController
  {
  virtual void OnLButtonDown(UINT nFlags, CPoint point) = 0;
  virtual void OnLButtonUp(UINT nFlags, CPoint point) = 0;
  virtual void OnMouseMove(UINT nFlags, CPoint point) = 0;
  virtual void OnLButtonDblClk( UINT nFlags, CPoint point ) = 0;
  virtual bool IsActive() = 0;
  virtual HCURSOR GetCursor() = 0;
  };
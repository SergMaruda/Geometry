#pragma once

interface IUIObject;

interface IRender
  {
  virtual void Render(CDC*) = 0;
  virtual void SetColor(COLORREF) = 0;
  virtual void SetThickness(int) = 0;
  virtual IUIObject* GetObject() = 0;
  virtual ~IRender() = 0 {};
  };
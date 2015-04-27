#pragma once
#include "IRender.h"


class Render: public IRender
  {
  public:
    Render();
    ~Render();
    virtual void SetColor( COLORREF );
    virtual void SetThickness(int);
  protected:
    COLORREF m_color;
    int m_thickness;
  };
#include "Render.h"



//---------------------------------------------------------------------------------------------------
void Render::SetColor( COLORREF c)
  {
  m_color = c;
  }

//---------------------------------------------------------------------------------------------------
void Render::SetThickness( int i_thickness)
  {
  m_thickness = i_thickness;
  }

//---------------------------------------------------------------------------------------------------
Render::Render():
  m_color(RGB(0,0,0)),
  m_thickness(1)
  {
  }

  //---------------------------------------------------------------------------------------------------
Render::~Render()
  {
  }


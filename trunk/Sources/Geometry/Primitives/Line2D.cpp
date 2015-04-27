#include "Line2D.h"

//---------------------------------------------------------------
Line2D::Line2D()
  {
  m_direction.Set(0,1);
  }

//---------------------------------------------------------------
const Point2D& Line2D::GetOrigin() const
  {
  return m_origin;
  }

//---------------------------------------------------------------
const TVector2D& Line2D::GetDirection() const
  {
  return m_direction;
  }

//---------------------------------------------------------------
void Line2D::SetOrigin(const Point2D& i_origin)
  {
  m_origin = i_origin;
  }

void Line2D::SetDirection( const TVector2D& i_direction)
  {
  m_direction = i_direction;
  }

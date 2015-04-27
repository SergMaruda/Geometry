#include "Point2D.h"


//--------------------------------------------------------------------
Point2D::Point2D()
  {
  m_point_2d[0] = 0;
  m_point_2d[1] = 0;
  }

//--------------------------------------------------------------------
double Point2D::GetX() const
  {
  return m_point_2d[0];
  }

//--------------------------------------------------------------------
double Point2D::GetY() const
  {
  return m_point_2d[1];
  }

//--------------------------------------------------------------------
double& Point2D::operator[]( size_t i)
  {
  return m_point_2d[i];
  }

//--------------------------------------------------------------------
const double& Point2D::operator[]( size_t i) const
  {
  return m_point_2d[i];
  }

#include "Point2D.h"
#include <math.h>


//--------------------------------------------------------------------
Point2D::Point2D()
  {
  m_point_2d[0] = 0;
  m_point_2d[1] = 0;
  }

Point2D::Point2D( double x, double y )
  {
  m_point_2d[0] = x;
  m_point_2d[1] = y;
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
Point2D Point2D::operator/( double i_val)
  {
  Point2D res(*this);
  res.m_point_2d[0] /= i_val;
  res.m_point_2d[1] /= i_val;
  return res;
  }

//--------------------------------------------------------------------
Point2D Point2D::operator*( double i_val)
  {
  Point2D res(*this);
  res.m_point_2d[0] *= i_val;
  res.m_point_2d[1] *= i_val;
  return res;  
  }

//--------------------------------------------------------------------
Point2D Point2D::operator-( const Point2D& i_other) const
  {
  Point2D res(*this);

  res.m_point_2d[0] -= i_other.m_point_2d[0];
  res.m_point_2d[1] -= i_other.m_point_2d[1];
  return res;
  }

//--------------------------------------------------------------------
Point2D Point2D::operator+( const Point2D& i_other) const
  {
  Point2D res(*this);

  res.m_point_2d[0] += i_other.m_point_2d[0];
  res.m_point_2d[1] += i_other.m_point_2d[1];
  return res;
  }

//--------------------------------------------------------------------
void Point2D::Set( double x, double y )
  {
  m_point_2d[0] = x;
  m_point_2d[1] = y;
  }

//--------------------------------------------------------------------
double Point2D::Length()
  {
  return sqrt(m_point_2d[0]*m_point_2d[0]+m_point_2d[1]*m_point_2d[1]);
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

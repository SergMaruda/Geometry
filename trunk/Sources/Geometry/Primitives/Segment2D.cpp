#include "Segment2D.h"
#include <math.h>
//---------------------------------------------------------------
Segment2D::Segment2D():
  m_p1(0,0),
  m_p2(0,1)
  {
  }

//---------------------------------------------------------------
Segment2D::Segment2D( const Point2D& i_p1,const Point2D& i_p2):
  m_p1(i_p1),
  m_p2(i_p2)
  {

  }

//---------------------------------------------------------------
const Point2D& Segment2D::GetP1() const
  {
  return m_p1;
  }

//---------------------------------------------------------------
const TVector2D& Segment2D::GetP2() const
  {
  return m_p2;
  }

//---------------------------------------------------------------
void Segment2D::SetP1(const Point2D& i_p1)
  {
   m_p1 = i_p1;
  }

//---------------------------------------------------------------
void Segment2D::SetP2( const TVector2D& i_p2)
  {
  m_p2 = i_p2;
  }

//---------------------------------------------------------------
double Segment2D::DistanceToPoint( const Point2D& i_p) const
  {
  Point2D w = i_p - m_p1;
  Point2D v = m_p2 - m_p1;

  double c1 = w*v;
  double c2 = v*v;
  if ( c1 <= 0)
    return i_p.Distance(m_p1);
  else if (c2<=c1)
    return i_p.Distance(m_p2);

  return i_p.Distance(m_p1+v*(c1/c2));
  }

//---------------------------------------------------------------
bool Segment2D::GetIntersectionBetweenLines( Point2D& o_point, const Segment2D& i_other) const
  {
  const double tolerance(0.01);

  const Point2D& A1 = m_p1;
  const Point2D& A2 = m_p2;
  const Point2D& B1 = i_other.m_p1;
  const Point2D& B2 = i_other.m_p2;

  double nominator = (B2[0]-B1[0])*(A1[1]-B1[1]) - (B2[1]-B1[1])*(A1[0]-B1[0]);
  double denominator = (B2[1]-B1[1])*(A2[0]-A1[0]) - (B2[0]-B1[0])*(A2[1]-A1[1]);

  if(fabs(denominator) < tolerance)
    return false;

  double p = nominator/denominator;
  o_point[0] = A1[0] + p * (A2[0]-A1[0]);
  o_point[1] = A1[1] + p * (A2[1]-A1[1]);
  return true;
  }

//---------------------------------------------------------------
bool Segment2D::GetIntersection( Point2D& o_point, const Segment2D& i_other) const
  {
  if(GetIntersectionBetweenLines(o_point, i_other))
    {
    double dist1 = i_other.DistanceToPoint(o_point);
    double dist2 = DistanceToPoint(o_point);
    return fabs(dist1) + fabs(dist2) < 2.0 * 0.01;
    }
  return false;
  }

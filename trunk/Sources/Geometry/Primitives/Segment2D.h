#pragma once
#include "Point2D.h"


class Segment2D
  {
  public:
    Segment2D();

    Segment2D(const Point2D&,const Point2D&);

    const Point2D& GetP1() const;
    const TVector2D&  GetP2() const;
    void SetP1(const Point2D&);
    void SetP2(const TVector2D&);
    double DistanceToPoint(const Point2D&) const;
    bool GetIntersectionBetweenLines(Point2D& o_point, const Segment2D&) const;
    bool GetIntersection(Point2D& , const Segment2D&) const;

  private:
    Point2D m_p1;
    Point2D m_p2;
  };
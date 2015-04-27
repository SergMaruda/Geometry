#pragma once
#include "..\Primitives\UISegment.h"
#include "RenderTemplate.h"


class UISegmentRender: public RenderTemplate<UISegment>
  {
  public:
    UISegmentRender(UISegment*);
    virtual void Render( CDC* );
  };
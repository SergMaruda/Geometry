#pragma once
#include "Render.h"
#include "RenderTemplate.h"
#include "..\Primitives\UIPoint.h"

class UIPointRender: public RenderTemplate<UIPoint>
  {
  public:
    UIPointRender(UIPoint*);
    virtual void Render( CDC* );
  };
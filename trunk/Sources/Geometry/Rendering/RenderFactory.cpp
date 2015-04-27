#include "RenderFactory.h"
#include "..\Primitives\UIPoint.h"
#include "UIPointRender.h"
#include "..\Primitives\UISegment.h"
#include "UISegmentRender.h"

//----------------------------------------------------------------------
RenderFactory& RenderFactory::Instance()
  {
  static RenderFactory factory;
  return factory;
  }

//----------------------------------------------------------------------
IRender* RenderFactory::CreateRender( IUIObject* ip_object)
  {
  auto p_point = dynamic_cast<UIPoint*>(ip_object);
  if(p_point)
    return new UIPointRender(p_point);

  auto p_segment = dynamic_cast<UISegment*>(ip_object);
  if(p_segment)
    return new UISegmentRender(p_segment);

  return nullptr;
  }

//----------------------------------------------------------------------
RenderFactory::RenderFactory()
  {
  }

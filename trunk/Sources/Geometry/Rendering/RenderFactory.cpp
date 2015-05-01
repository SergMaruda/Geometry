#include "RenderFactory.h"
#include "..\Primitives\IUIObject.h"

//----------------------------------------------------------------------
RenderFactory& RenderFactory::Instance()
  {
  static RenderFactory factory;
  return factory;
  }

//----------------------------------------------------------------------
IRender* RenderFactory::CreateRender( IUIObject* ip_object)
  {
  auto name = typeid(*ip_object).name();
  auto creator = m_creators.find(name) ;
  if(creator != m_creators.end())
    return creator->second(ip_object);

  return nullptr;
  }

//----------------------------------------------------------------------
RenderFactory::RenderFactory()
  {
  }

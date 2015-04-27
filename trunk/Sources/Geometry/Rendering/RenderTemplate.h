#pragma once
#include "Render.h"

template<class TObjectType>
class RenderTemplate: public Render
  {
  public:
    RenderTemplate(TObjectType*);
    virtual TObjectType* GetObject();

  protected:
    TObjectType* mp_object;
  };

template<class TObjectType>
RenderTemplate<TObjectType>::RenderTemplate( TObjectType* ip_object):
  mp_object(ip_object)
  {
  }

template<class TObjectType>
TObjectType* RenderTemplate<TObjectType>::GetObject()
  {
  return mp_object;
  }


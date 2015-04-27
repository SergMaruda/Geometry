#pragma once

interface IRender;
interface IUIObject;

class RenderFactory
  {
  public:
    static RenderFactory& Instance();
    IRender* CreateRender(IUIObject*);
  private:
    RenderFactory();
    RenderFactory(const RenderFactory&);
    RenderFactory& operator=(const RenderFactory&);
  };
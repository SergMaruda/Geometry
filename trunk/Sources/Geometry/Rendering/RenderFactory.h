#pragma once
#include <functional>
#include <map>
#include <string>

interface IRender;
interface IUIObject;

class RenderFactory
  {
  public:
    static RenderFactory& Instance();

    template<class TObject, class TRender> 
    void RegisterRender();

    template<class TObject, class TRender> 
    struct Registrator;

    IRender* CreateRender(IUIObject*);
  private:
    RenderFactory();
    RenderFactory(const RenderFactory&);
    RenderFactory& operator=(const RenderFactory&);
    typedef std::function<IRender* (IUIObject*)> TCreator;
    std::map<std::string, TCreator> m_creators; 
  };

template<class TObject, class TRender>
void RenderFactory::RegisterRender()
  {

  auto f = [](IUIObject* ip_obj) -> IRender*
    {
  return new TRender(dynamic_cast<TObject*>(ip_obj));
  };
  m_creators[typeid(TObject).name()] = f;
  }


template<class TObject, class TRender> 
struct RenderFactory::Registrator
  {
  Registrator()
    {
    Instance().RegisterRender<TObject, TRender>();
    }
  };
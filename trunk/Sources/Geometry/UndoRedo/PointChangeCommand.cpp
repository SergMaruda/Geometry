#include "ICommand.h"
#include "..\Primitives\UIPoint.h"
#include "UndoRedoManager.h"



class PointChangeCommand: public UR::ICommand
  {
  public:

  PointChangeCommand(UIPoint*ip_point, const Point2D& i_p):mp_point(ip_point), m_p(i_p)
    {

    }

  virtual bool Do()
    {
    mp_point->SetPoint(m_p);
    return true;
    }

  UIPoint* mp_point;
  Point2D m_p;
  };


struct Registrator
  {
  Registrator()
    {
    auto trigger = [](TIUIObjectPtr ip_object) -> UR::ICommand*
      {
      auto point = dynamic_cast<UIPoint*>(ip_object);
      return new PointChangeCommand(point, point->GetPoint());
      };

    UndoRedoManager::GetInstance().RegisterUndoRedoTrigger(BEFORE_POINT_CHANGED, trigger);
    }
  };

static Registrator r_reg;
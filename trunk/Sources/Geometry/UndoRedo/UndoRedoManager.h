#pragma once

#include "..\Notifications\Notifications.h"
#include "..\Notifications\Observer.h"

#include <vector>
#include <memory>
#include <functional>
#include <map>



namespace UR
  {
  struct ICommand;
  }

struct IUIObject;

class UndoRedoManager: public Observer
  {
  public:
    typedef std::function<UR::ICommand* (TIUIObjectPtr)> TTrigger;

    UndoRedoManager();
    static UndoRedoManager& GetInstance();

    void RegisterUndoRedoTrigger(ENotification, TTrigger);
    bool Undo();
    bool Redo();
    void OnNotification(TIUIObjectPtr, ENotification i_ntf);
    void BlockUndoRedo(bool);


  private:
    std::vector<std::unique_ptr<UR::ICommand>> m_undo_stack;
    std::vector<std::unique_ptr<UR::ICommand>> m_redo_stack;
    std::map<ENotification, TTrigger> m_triggers;
    bool m_undoing;
    bool m_redoing;
    bool m_is_blocked;
  };

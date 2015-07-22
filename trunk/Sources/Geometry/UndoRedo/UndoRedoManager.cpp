#include "UndoRedoManager.h"
#include "ICommand.h"

UndoRedoManager::UndoRedoManager():m_undoing(false), m_is_blocked(false)
  {
  }


UndoRedoManager& UndoRedoManager::GetInstance()
  {
  static UndoRedoManager g_man;
  return g_man;
  }

void UndoRedoManager::RegisterUndoRedoTrigger( ENotification i_ntf, TTrigger i_trigger)
  {
  m_triggers[i_ntf] = i_trigger;

  auto func = [&,i_ntf](TIUIObjectPtr ip_obj)
    {
    OnNotification(ip_obj, i_ntf);
    };

  Subscribe(i_ntf, func);
  }

bool UndoRedoManager::Undo()
  {
  m_undoing = true;
  if(m_undo_stack.empty() == false)
    {
    m_undo_stack.back()->Do();
    m_undo_stack.pop_back();
    }

  m_undoing = false;
  return true;
  }

bool UndoRedoManager::Redo()
  {
  m_redoing = true;
  if(m_redo_stack.empty() == false)
    {
    m_redo_stack.back()->Do();
    m_redo_stack.pop_back();
    }
  m_redoing = false;
  return false;
  }

void UndoRedoManager::OnNotification( TIUIObjectPtr ip_object, ENotification i_ntf)
  {
  if(m_is_blocked)
    return;

  m_is_blocked = true;

  if(m_undoing == false && m_redoing == false)
    {
    m_redo_stack.clear();
    m_undo_stack.push_back(std::unique_ptr<UR::ICommand>(m_triggers[i_ntf](ip_object)));
    }
  else if (m_undoing)
    {
    m_redo_stack.push_back(std::unique_ptr<UR::ICommand>(m_triggers[i_ntf](ip_object)));
    } 
  else if(m_redoing)
    {
    m_undo_stack.push_back(std::unique_ptr<UR::ICommand>(m_triggers[i_ntf](ip_object)));
    }
  else
    {
    ASSERT(false);
    }
  m_is_blocked = false;
  }

void UndoRedoManager::BlockUndoRedo( bool i_block)
  {
  m_is_blocked = i_block;
  }

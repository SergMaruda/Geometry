#include "Primitives\IUIObject.h"
#include "UndoRedo\ICommand.h"
#include "UndoRedo\UndoRedoManager.h"



struct AddObjectCommand: public UR::ICommand
  {
  TIUIObjectPtr mp_root;
  TIUIObjectPtr mp_object;
  AddObjectCommand(TIUIObjectPtr ip_root, TIUIObjectPtr ip_object): mp_root(ip_root), mp_object(ip_object)
    {
    }

  ~AddObjectCommand()
    {
    delete mp_object;
    }

  virtual bool Do()
    {
    mp_root->AddChild(mp_object);
    mp_object = nullptr;
    return true;
    }
  };


 struct RegistratorXXX
   {
   RegistratorXXX()
     {
     auto trigger = [](TIUIObjectPtr ip_object) -> UR::ICommand*
       {
       auto parent = ip_object->GetParent();
       auto new_obj = ip_object->Clone();
       ip_object->SetParent(nullptr);
       return new AddObjectCommand(parent, new_obj);
       };
 
     UndoRedoManager::GetInstance().RegisterUndoRedoTrigger(BEFORE_OBJECT_REMOVED, trigger);
     }
   };
 
static RegistratorXXX r_reg;
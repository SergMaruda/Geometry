 
#include "Primitives\IUIObject.h"
#include "UndoRedo\ICommand.h"
#include "UndoRedo\UndoRedoManager.h"
 
 
 struct RemoveObjectCommand: public UR::ICommand
   {
   TIUIObjectPtr mp_object;
   TIUIObjectPtr mp_parent;
 
   RemoveObjectCommand(TIUIObjectPtr ip_object): mp_object(ip_object)
     {
     mp_parent = mp_object->GetParent();
     }

   ~RemoveObjectCommand()
     {
     delete mp_object;
     }

   virtual bool Do()
     {
     bool res = mp_parent->RemoveChild(mp_object);
     if(res == false)
       mp_object = nullptr;
     return true;
     }
  
   };

 struct Registrator2
   {
   Registrator2()
     {
     auto trigger = [](TIUIObjectPtr ip_object) -> UR::ICommand*
       {
       return new RemoveObjectCommand(ip_object);
       };

     UndoRedoManager::GetInstance().RegisterUndoRedoTrigger(OBJECT_ADDED, trigger);
     }
   };

 static Registrator2 r_reg;
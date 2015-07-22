#pragma once
#include <memory>


interface IUIObject;

typedef IUIObject* TIUIObjectPtr;

interface IUIObject
  {
  virtual void SetLabel(const CString&) = 0;
  virtual void SetParent(TIUIObjectPtr) = 0;
  virtual void AddChild(TIUIObjectPtr) = 0;
  virtual TIUIObjectPtr RemoveChild(size_t) = 0; //release ownership
  virtual bool RemoveChild(TIUIObjectPtr) = 0;
  virtual void DeleteChild(size_t) = 0;//delete completely
  virtual void DeleteChild(TIUIObjectPtr) = 0;//delete completely
  virtual void DeleteAllChilds() = 0;
  virtual IUIObject* Clone() const = 0;

  virtual const CString& GetLabel() const = 0;
  virtual TIUIObjectPtr  GetParent() const = 0;
  virtual TIUIObjectPtr  GetChild(size_t) const = 0;
  virtual size_t GetChildIdx(TIUIObjectPtr) const = 0;
  virtual size_t         GetNumChilds() const = 0;
  virtual COLORREF       GetColor() const = 0;

  virtual ~IUIObject() = 0 {};
  };



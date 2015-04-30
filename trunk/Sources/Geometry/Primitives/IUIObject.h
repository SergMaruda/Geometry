#pragma once

interface IUIObject
  {
  virtual void SetLabel(const CString&) = 0;
  virtual void SetParent(IUIObject*) = 0;
  virtual void AddChild(IUIObject*) = 0;
  virtual IUIObject* RemoveChild(size_t) = 0; //release ownership
  virtual void DeleteChild(size_t) = 0;//delete completely
  virtual void DeleteChild(IUIObject*) = 0;//delete completely
  virtual void DeleteAllChilds() = 0;

  virtual const CString& GetLabel() const = 0;
  virtual IUIObject*     GetParent() const = 0;
  virtual IUIObject*     GetChild(size_t) const = 0;
  virtual size_t         GetNumChilds() const = 0;
  virtual COLORREF       GetColor() const = 0;

  virtual ~IUIObject() = 0 {};
  };

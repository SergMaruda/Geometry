#pragma once

#include "IUIObject.h"
#include <vector>

class UIObject: public IUIObject
  {
  public:
    UIObject();
    UIObject(const UIObject&);
    ~UIObject();

  virtual void SetLabel(const CString&) override;
  virtual void SetParent(IUIObject*) override;
  virtual void AddChild(IUIObject* ip_child) override;
  virtual IUIObject* RemoveChild(size_t) override;
  virtual void DeleteChild(size_t) override;
  virtual void DeleteChild(IUIObject*) override;

  virtual const CString& GetLabel() const override;
  virtual IUIObject*     GetParent() const override;
  virtual IUIObject*     GetChild(size_t) const override;
  virtual size_t         GetNumChilds() const override;
  virtual COLORREF       GetColor() const override;

  template<class TObjectType> 
  std::vector<TObjectType*> GetChildsByType()
    {
    std::vector<TObjectType*> res;
    for(size_t i = 0; i < m_childs.size(); ++i)
      {
      auto p_obj = dynamic_cast<TObjectType*>(m_childs[i]);
      if(p_obj)
        res.push_back(p_obj);
      }
    return res;
    }

  protected:
    CString m_label;
    static size_t mg_objects_count;
    COLORREF m_color;
    CString _SetNameWithIndex(const CString&);

  private:
    IUIObject* mp_parent;
    std::vector<IUIObject*> m_childs;
  };
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
  virtual void SetParent(TIUIObjectPtr) override;
  virtual void AddChild(TIUIObjectPtr ip_child) override;
  virtual TIUIObjectPtr RemoveChild(size_t) override;
  virtual void DeleteChild(size_t) override;
  virtual void DeleteChild(TIUIObjectPtr) override;
  virtual void DeleteAllChilds() override;

  virtual const CString& GetLabel() const override;
  virtual TIUIObjectPtr     GetParent() const override;
  virtual TIUIObjectPtr     GetChild(size_t) const override;
  virtual size_t         GetNumChilds() const override;
  virtual COLORREF       GetColor() const override;

  template<class TObjectType> 
  TObjectType* GetChild(size_t i_idx) const
    {
    return dynamic_cast<TObjectType*>(GetChild(i_idx));
    }

  template<class TObjectType> 
  std::vector<TObjectType*> GetChildsByType()
    {

    std::vector<TObjectType*> res;
    for(size_t i = 0; i < m_childs.size(); ++i)
      {
      auto p_child = m_childs[i];
      auto p_concrete_obj = dynamic_cast<TObjectType*>(p_child);
      if(p_concrete_obj )
        res.push_back(p_concrete_obj );

      auto p_obj_n = dynamic_cast<UIObject*>(p_child);
      if(p_obj_n)
        {
        auto res2 = p_obj_n->GetChildsByType<TObjectType>();
        res.insert(res.begin(), res2.begin(), res2.end());
        }

      }
    return res;
    }

  protected:
    CString m_label;
    static size_t mg_objects_count;
    COLORREF m_color;
    CString _SetNameWithIndex(const CString&);

  private:
    TIUIObjectPtr mp_parent;
    std::vector<TIUIObjectPtr> m_childs;
  };
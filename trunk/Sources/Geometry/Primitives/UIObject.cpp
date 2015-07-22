#include "UIObject.h"
#include "..\Notifications\NotificationCenter.h"
#include <algorithm>

size_t UIObject::mg_objects_count(0);

CString UIObject::_SetNameWithIndex( const CString& i_obj_tag)
  {
  CString name;
  name.Format(L"%d", mg_objects_count);
  m_label = i_obj_tag + " " + name;
  return name;
  }

//----------------------------------------------------------------------------------------------------------------
UIObject::UIObject():
  mp_parent(nullptr),
  m_color(RGB(0,0,0))
  {
  ++mg_objects_count;
  }


//----------------------------------------------------------------------------------------------------------------
UIObject::UIObject( const UIObject& i_other)
  {
  ++mg_objects_count;

  mp_parent = i_other.mp_parent;
  m_childs = i_other.m_childs;
  m_label = i_other.m_label;
  m_color = i_other.m_color;

  for(size_t i = 0; i < i_other.m_childs.size(); ++i)
    {
    m_childs.push_back(i_other.m_childs[i]->Clone());
    }
  }

//----------------------------------------------------------------------------------------------------------------
UIObject::~UIObject()
  {
  --mg_objects_count;
  DeleteAllChilds();
  }

//----------------------------------------------------------------------------------------------------------------
void UIObject::SetLabel( const CString& i_label)
  {
  if(m_label != i_label)
    {
    m_label = i_label;
    NotificationCenter::Notify(LABEL_CHANGED, this);
    }
  }

//----------------------------------------------------------------------------------------------------------------
void UIObject::SetParent( TIUIObjectPtr ip_parent)
  {
  if(mp_parent != ip_parent)
    {
    auto parent_old = mp_parent;

    if(parent_old)
      parent_old->RemoveChild(this);

    mp_parent = ip_parent;

    if(ip_parent)
      ip_parent->AddChild(this);
    }
  }

//----------------------------------------------------------------------------------------------------------------
void UIObject::AddChild(TIUIObjectPtr ip_child)
  {
  auto i = std::find(m_childs.begin(),m_childs.end(), ip_child);
  if(i==m_childs.end())
    {
    m_childs.push_back(ip_child);
    ip_child->SetParent(this);
    NotificationCenter::Notify(OBJECT_ADDED, ip_child);
    }
  }

//----------------------------------------------------------------------------------------------------------------
bool UIObject::RemoveChild(TIUIObjectPtr ip_obj) 
  {
  size_t idx = GetChildIdx(ip_obj);
  if(idx != -1)
    {
    RemoveChild(idx);
    return true;
    }
  return false;
  }

//----------------------------------------------------------------------------------------------------------------
TIUIObjectPtr UIObject::RemoveChild( size_t i)
  {
  auto child = m_childs[i];
  NotificationCenter::Notify(BEFORE_OBJECT_REMOVED, child);
  if(GetChildIdx(child) != -1)
    {
    m_childs.erase(m_childs.begin() + i);
    child->SetParent(nullptr);
    NotificationCenter::Notify(OBJECT_REMOVED, child);
    return child;
    }
  return TIUIObjectPtr();
  }

//----------------------------------------------------------------------------------------------------------------
void UIObject::DeleteChild( size_t i)
  {
  auto child = m_childs[i];
  NotificationCenter::Notify(BEFORE_CHILD_DELETE, child);
  if(i < m_childs.size() && child == m_childs[i])
    {
    RemoveChild(i);
    delete child;
    }
  }

//----------------------------------------------------------------------------------------------------------------
void UIObject::DeleteChild( TIUIObjectPtr ip_obj)
  {
  size_t idx = GetChildIdx(ip_obj);
  if(idx != -1)
    DeleteChild(idx);
  }

//----------------------------------------------------------------------------------------------------------------
void UIObject::DeleteAllChilds()
  {
  while (GetNumChilds())
    DeleteChild((size_t)0);
  }

//----------------------------------------------------------------------------------------------------------------
size_t UIObject::GetChildIdx(TIUIObjectPtr ip_obj) const
  {
  auto child = std::find(m_childs.begin(), m_childs.end(), ip_obj);
  if(child != m_childs.end())
    return child - m_childs.begin();
  return -1;
  }

//----------------------------------------------------------------------------------------------------------------
const CString& UIObject::GetLabel() const 
  {
  return m_label;
  }

//----------------------------------------------------------------------------------------------------------------
TIUIObjectPtr UIObject::GetParent() const 
  {
  return mp_parent;
  }

//----------------------------------------------------------------------------------------------------------------
TIUIObjectPtr UIObject::GetChild( size_t i) const 
  {
  return m_childs[i];
  }

//----------------------------------------------------------------------------------------------------------------
size_t UIObject::GetNumChilds() const 
  {
  return m_childs.size();
  }

//----------------------------------------------------------------------------------------------------------------
COLORREF UIObject::GetColor() const 
  {
  return m_color;
  }

IUIObject* UIObject::Clone() const 
  {
  return new UIObject(*this);
  }

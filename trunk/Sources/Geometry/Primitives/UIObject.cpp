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
  }

//----------------------------------------------------------------------------------------------------------------
UIObject::~UIObject()
  {
  --mg_objects_count;
  while (GetNumChilds())
    {
    DeleteChild((size_t)0);
    }
  }

//----------------------------------------------------------------------------------------------------------------
void UIObject::SetLabel( const CString& i_label)
  {
  if(m_label != i_label)
    {
    m_label = i_label;
    NotificationCenter::Instance().Notify(LABEL_CHANGED, this);
    }
  }

//----------------------------------------------------------------------------------------------------------------
void UIObject::SetParent( IUIObject* ip_parent)
  {
  if(mp_parent != ip_parent)
    {
    mp_parent = ip_parent;
    if(mp_parent)
      mp_parent->AddChild(this);
    }
  }

//----------------------------------------------------------------------------------------------------------------
void UIObject::AddChild(IUIObject* ip_child)
  {
  auto i = std::find(m_childs.begin(),m_childs.end(), ip_child);
  if(i==m_childs.end())
    {
    m_childs.push_back(ip_child);
    ip_child->SetParent(this);
    NotificationCenter::Instance().Notify(OBJECT_ADDED, ip_child);
    }
  }

//----------------------------------------------------------------------------------------------------------------
IUIObject* UIObject::RemoveChild( size_t i)
  {
  auto child = m_childs[i];
  child->SetParent(nullptr);
  m_childs.erase(m_childs.begin() + i);
  NotificationCenter::Instance().Notify(OBJECT_REMOVED, child);
  return child;
  }

//----------------------------------------------------------------------------------------------------------------
void UIObject::DeleteChild( size_t i)
  {
  auto p_child = RemoveChild(i);
  delete p_child;
  }

//----------------------------------------------------------------------------------------------------------------
void UIObject::DeleteChild( IUIObject* ip_obj)
  {
  auto child = std::find(m_childs.begin(), m_childs.end(), ip_obj);
  if(child != m_childs.end())
    {
    size_t idx = child - m_childs.begin();
    DeleteChild(idx);
    }
  }

//----------------------------------------------------------------------------------------------------------------
const CString& UIObject::GetLabel() const 
  {
  return m_label;
  }

//----------------------------------------------------------------------------------------------------------------
IUIObject* UIObject::GetParent() const 
  {
  return mp_parent;
  }

//----------------------------------------------------------------------------------------------------------------
IUIObject* UIObject::GetChild( size_t i) const 
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

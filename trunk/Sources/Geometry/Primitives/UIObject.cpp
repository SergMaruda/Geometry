#include "UIObject.h"
#include "..\Notifications\NotificationCenter.h"
#include <algorithm>

size_t UIObject::mg_objects_count(0);

//----------------------------------------------------------------------------------------------------------------
UIObject::UIObject():
  mp_parent(nullptr)
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
  for(size_t i = 0; i < m_childs.size(); ++i)
    {
    m_childs[i]->SetParent(nullptr);
    delete m_childs[i];
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
  return child;
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

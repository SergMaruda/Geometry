
// GeometryDoc.cpp : implementation of the CGeometryDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GeometryApp.h"
#endif

#include "GeometryDoc.h"

#include <propkey.h>
#include "Notifications\NotificationCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGeometryDoc

IMPLEMENT_DYNCREATE(CGeometryDoc, CDocument)

BEGIN_MESSAGE_MAP(CGeometryDoc, CDocument)
END_MESSAGE_MAP()


// CGeometryDoc construction/destruction

static CGeometryDoc* g_active_doc(nullptr);

CGeometryDoc::CGeometryDoc():
  mp_picked_object(nullptr)
  {
  m_root_object.reset(new UIObject);
  g_active_doc = this;
  m_root_object->SetLabel(L"Objects");
  }

CGeometryDoc::~CGeometryDoc()
  {
  g_active_doc = nullptr;
  }

CGeometryDoc* CGeometryDoc::GetActive()
  {
  return g_active_doc;
  }

BOOL CGeometryDoc::OnNewDocument()
  {
  if (!CDocument::OnNewDocument())
    return FALSE;

  // TODO: add reinitialization code here
  // (SDI documents will reuse this document)
  m_root_object->DeleteAllChilds();
  return TRUE;
  }

// CGeometryDoc serialization

void CGeometryDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    // TODO: add storing code here
  }
  else
  {
    // TODO: add loading code here
  }
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CGeometryDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
  // Modify this code to draw the document's data
  dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

  CString strText = _T("TODO: implement thumbnail drawing here");
  LOGFONT lf;

  CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
  pDefaultGUIFont->GetLogFont(&lf);
  lf.lfHeight = 36;

  CFont fontDraw;
  fontDraw.CreateFontIndirect(&lf);

  CFont* pOldFont = dc.SelectObject(&fontDraw);
  dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
  dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CGeometryDoc::InitializeSearchContent()
{
  CString strSearchContent;
  // Set search contents from document's data. 
  // The content parts should be separated by ";"

  // For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
  SetSearchContent(strSearchContent);
}

void CGeometryDoc::SetSearchContent(const CString& value)
{
  if (value.IsEmpty())
  {
    RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
  }
  else
  {
    CMFCFilterChunkValueImpl *pChunk = NULL;
    ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
    if (pChunk != NULL)
    {
      pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
      SetChunkValue(pChunk);
    }
  }
}

#endif // SHARED_HANDLERS

//------------------------------------------------------------------------------------------------------------------
UIObject& CGeometryDoc::GetRootObject()
  {
  return *m_root_object;
  }

//------------------------------------------------------------------------------------------------------------------
UIObject* CGeometryDoc::GetRootObjectPtr()
  {
  return m_root_object.get();
  }

//------------------------------------------------------------------------------------------------------------------
void CGeometryDoc::SelectObject( TIUIObjectPtr ip_obj)
  {
  m_selected_objects.insert(ip_obj);
  NotificationCenter::Notify(OBJECT_SELECTED, ip_obj);
  }

//------------------------------------------------------------------------------------------------------------------
void CGeometryDoc::DeselectObject(TIUIObjectPtr ip_obj)
  {
  m_selected_objects.erase(ip_obj);
  NotificationCenter::Notify(OBJECT_DSELECTED, ip_obj);
  }

//------------------------------------------------------------------------------------------------------------------
bool CGeometryDoc::IsObjectSelected(TIUIObjectPtr ip_oj)
  {
  return m_selected_objects.find(ip_oj) != m_selected_objects.end();
  }

//------------------------------------------------------------------------------------------------------------------
TIUIObjectPtr CGeometryDoc::GetFirstSelected()
  {
  if(m_selected_objects.empty())
    return nullptr;

  return *m_selected_objects.begin();
  }

//------------------------------------------------------------------------------------------------------------------
void CGeometryDoc::DeselectAllObjects()
  {
  while(!m_selected_objects.empty())
    {
    DeselectObject(*m_selected_objects.begin());
    }
  }

//------------------------------------------------------------------------------------------------------------------
void CGeometryDoc::DeleteSelected()
  {
  while(!m_selected_objects.empty())
    {
    auto first = *m_selected_objects.begin();
    m_selected_objects.erase(first);
    m_root_object->DeleteChild(first);
    }
  }


//------------------------------------------------------------------------------------------------------------------
void CGeometryDoc::SetPickedObject( TIUIObjectPtr ip_object)
  {
  mp_picked_object = ip_object;
  }

//------------------------------------------------------------------------------------------------------------------
TIUIObjectPtr CGeometryDoc::GetPickedObject() const
  {
  return mp_picked_object;
  }

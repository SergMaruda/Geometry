
// GeometryDoc.cpp : implementation of the CGeometryDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Geometry.h"
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
  g_active_doc = this;
  m_root_object.SetLabel(L"Objects");
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

// CGeometryDoc diagnostics

#ifdef _DEBUG
void CGeometryDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGeometryDoc::Dump(CDumpContext& dc) const
  {
	CDocument::Dump(dc);
  }


#endif //_DEBUG


// CGeometryDoc commands

UIObject& CGeometryDoc::GetRootObject()
  {
  return m_root_object;
  }

void CGeometryDoc::SelectObject( IUIObject* ip_obj)
  {
  m_selected_objects.insert(ip_obj);
  NotificationCenter::Instance().Notify(OBJECT_SELECTED, ip_obj);
  }

void CGeometryDoc::DeselectObject(IUIObject* ip_obj)
  {
  m_selected_objects.erase(ip_obj);
  NotificationCenter::Instance().Notify(OBJECT_DSELECTED, ip_obj);
  }

bool CGeometryDoc::IsObjectSelected(IUIObject* ip_oj)
  {
  return m_selected_objects.find(ip_oj) != m_selected_objects.end();
  }

IUIObject* CGeometryDoc::GetFirstSelected()
  {
  if(m_selected_objects.empty())
    return nullptr;

  return *m_selected_objects.begin();
  }

void CGeometryDoc::DeselectAllObjects()
  {
  m_selected_objects.clear();
  }

void CGeometryDoc::SetPickedObject( IUIObject* ip_object)
  {
  mp_picked_object = ip_object;
  }

IUIObject* CGeometryDoc::GetPickedObject() const
  {
  return mp_picked_object;
  }

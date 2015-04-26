
// GeometryView.cpp : implementation of the CGeometryView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Geometry.h"
#endif

#include "GeometryDoc.h"
#include "GeometryView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGeometryView

IMPLEMENT_DYNCREATE(CGeometryView, CView)

BEGIN_MESSAGE_MAP(CGeometryView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CGeometryView construction/destruction

CGeometryView::CGeometryView()
{
	// TODO: add construction code here

}

CGeometryView::~CGeometryView()
{
}

BOOL CGeometryView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGeometryView drawing

void CGeometryView::OnDraw(CDC* /*pDC*/)
{
	CGeometryDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CGeometryView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGeometryView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGeometryView diagnostics

#ifdef _DEBUG
void CGeometryView::AssertValid() const
{
	CView::AssertValid();
}

void CGeometryView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGeometryDoc* CGeometryView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGeometryDoc)));
	return (CGeometryDoc*)m_pDocument;
}
#endif //_DEBUG


// CGeometryView message handlers

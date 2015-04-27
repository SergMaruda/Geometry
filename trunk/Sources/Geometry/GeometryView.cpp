
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
  ON_WM_MOUSEMOVE()
  ON_WM_MBUTTONDOWN()
  ON_WM_MBUTTONUP()
  ON_WM_MOUSEWHEEL()
  ON_WM_MOUSEMOVE()
  ON_WM_CREATE()
 // ON_WM_VSCROLL() 
 // ON_WM_SIZE()
END_MESSAGE_MAP()

// CGeometryView construction/destruction

CGeometryView::CGeometryView()
  {
  m_world_to_view.MakeScale3D(0.01, 0.01, 1.);
  double translation[3] = {2000,-2000, 0};
  m_world_to_view.AddTranslation(translation);
  }

CGeometryView::~CGeometryView()
{
}

//------------------------------------------------------------------------------
BOOL CGeometryView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

//------------------------------------------------------------------------------
int CGeometryView::OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
  int res = CView::OnCreate(lpCreateStruct);
  return res;
  }


//-----------------------------------------------------------------------------
// CGeometryView drawing
void CGeometryView::OnDraw(CDC* pDC)
  {
	CGeometryDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;//

  pDC->SetMapMode(MM_HIMETRIC);
  pDC->SetGraphicsMode(GM_ADVANCED);

  //pDC->GetWorldTransform(&m);
  //bool res = pDC->SetWorldTransform(&m);
  double point[3] = {0,0,0};
  m_world_to_view.TransformPoint3D(point,point);
  pDC->MoveTo(point[0],point[0]);

  double point2[3] = {0, 20000, 0};
  m_world_to_view.TransformPoint3D(point2,point2);
  pDC->LineTo(point2[0],point2[0]);
 
  
  // 
 // double pt2[3] = {0,200,0};
 // m_view_to_world.TransformPoint3D(point,point);


//  pDC->LineTo(pt2[0],pt2[0]);
  pDC->TextOut(20000,-200000, L"2000mm");
  pDC->TextOut(0,-0, L"(0,0)mm");


  pDC->MoveTo(0,0);
  pDC->LineTo(200000, 0);

  pDC->MoveTo(0,0);
  pDC->LineTo(0, -200000);

  pDC->MoveTo(0,0);
  pDC->LineTo(-200000, 0);

  // TODO: add draw code for native data here
  }


//----------------------------------------------------------
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

//------------------------------------------------------------------------
BOOL CGeometryView::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /*= NULL*/ )
  {
  BOOL res = CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
  return res;
  }

CPoint start;
bool m_is_mode(false);
//------------------------------------------------------------------------
void CGeometryView::OnMButtonDown( UINT nFlags, CPoint point )
  {
  m_is_mode = true;
  start = point;
  }

void CGeometryView::OnMButtonUp( UINT nFlags, CPoint point )
  {
  m_is_mode = false;
  }


BOOL CGeometryView::OnMouseWheel( UINT fFlags, short zDelta, CPoint point )
  {
  double coeff(1.5);
  if(zDelta < 0)
    coeff = 1./coeff;

  TransformMatrix m;
  m.MakeScale3D(coeff, coeff, 1);
  m_world_to_view.PostMultiply(m);
  Invalidate();
  return TRUE;
  }

void CGeometryView::OnMouseMove( UINT nFlags, CPoint point )
  {
  if(m_is_mode)
    {
    CPoint new_point = point-start;
    double tr[3] = {new_point.x,-new_point.y,0};
    double res[3];

    TransformMatrix m_i(m_world_to_view);
    //m_i.Invert();
    m_i.TransformVector3D(res, tr);

    TransformMatrix m;
    m.AddTranslation(res);
    m_world_to_view.PreMultiply(m);
    Invalidate();
    start = point;
    }
  else
    {
    
    }
  }



#endif //_DEBUG


// CGeometryView message handlers

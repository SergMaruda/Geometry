
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
#include <functional>
#include "Primitives\UIPoint.h"
#include "ViewControllers\IViewController.h"
#include "ViewControllers\ViewControllerMovePoint.h"
#include "ViewControllers\ViewControllerCreatePoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGeometryView

IMPLEMENT_DYNCREATE(CGeometryView, CView)

BEGIN_MESSAGE_MAP(CGeometryView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
  ON_WM_RBUTTONDOWN()
  ON_WM_LBUTTONDOWN()
  ON_WM_MOUSEMOVE()
  ON_WM_MBUTTONDOWN()
  ON_WM_MBUTTONUP()
  ON_WM_MOUSEWHEEL()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_MOUSEMOVE()
  ON_WM_CREATE()
  ON_COMMAND(ID_EDIT_CREATEPOINT, OnCreatePoint)
  ON_UPDATE_COMMAND_UI(ID_EDIT_CREATEPOINT, OnUpdateCreatePoint)
 // ON_WM_VSCROLL() 
 // ON_WM_SIZE()
END_MESSAGE_MAP()

// CGeometryView construction/destruction

//--------------------------------------------------------------------------------------------------------
CGeometryView::CGeometryView()
  {
  m_connections.push_back(NotificationCenter::Instance().AddObserver(OBJECT_ADDED, std::bind1st(std::mem_fun(&CGeometryView::OnUpdate), this)));
  m_connections.push_back(NotificationCenter::Instance().AddObserver(OBJECT_REMOVED, std::bind1st(std::mem_fun(&CGeometryView::OnUpdate), this)));
  m_connections.push_back(NotificationCenter::Instance().AddObserver(POINT_CHANGED, std::bind1st(std::mem_fun(&CGeometryView::OnUpdate), this)));
  m_connections.push_back(NotificationCenter::Instance().AddObserver(OBJECT_SELECTED, std::bind1st(std::mem_fun(&CGeometryView::OnUpdate), this)));

  m_controllers.push(std::unique_ptr<IViewController>(new ViewControllerMovePoint(this)));
  }

//--------------------------------------------------------------------------------------------------------
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
void RenderPoint(const UIPoint* ip_point, CDC* pDC)
  {
  int x = ip_point->GetPoint()[0];
  int y = ip_point->GetPoint()[1];

  COLORREF pen_color(RGB(0,0,0));
  int rad(2);

  if(CGeometryDoc::GetActive()->IsObjectSelected((UIPoint*)ip_point))
    {
    pen_color = RGB(255,0,0);
    rad = 4;
    }

  CPen pen;
  CBrush brush;
  brush.CreateSolidBrush(pen_color);
  pen.CreatePen(PS_SOLID,1,pen_color);

  auto p_old_pen = pDC->SelectObject(&pen);   
  auto p_old_brush = pDC->SelectObject(&brush);   
  pDC->Ellipse(x-rad,y+rad,x+rad,y-rad);
  pDC->SelectObject(p_old_pen);
  pDC->SelectObject(p_old_brush);
  }

//-----------------------------------------------------------------------------
// CGeometryView drawing
void CGeometryView::OnDraw(CDC* pDC)
  {
	CGeometryDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;//

  CRect rc;
  GetClientRect(&rc);

  auto p_doc = GetDocument();
  auto& root_object = p_doc->GetRootObject();

  for(size_t i = 0; i < root_object.GetNumChilds(); ++i)
    {
    auto p_point = dynamic_cast<UIPoint*>(root_object.GetChild(i));
    if(p_point)
      RenderPoint(p_point, pDC);
    }

  // TODO: add draw code for native data here
  }


void CGeometryView::OnRButtonDown( UINT nFlags, CPoint point )
  {
  }

//----------------------------------------------------------
void CGeometryView::OnRButtonUp(UINT /* nFlags */, CPoint point)
  {
  ClientToScreen(&point);
	OnContextMenu(this, point);
  }

//----------------------------------------------------------
void CGeometryView::OnLButtonDown( UINT nFlags, CPoint point )
  {
  m_controllers.top()->OnLButtonDown(nFlags, point);
  }

//----------------------------------------------------------
void CGeometryView::OnLButtonUp( UINT nFlags, CPoint point )
  {

  }

//----------------------------------------------------------
void CGeometryView::OnLButtonDblClk( UINT nFlags, CPoint point )
  {
  auto top = m_controllers.top().get();
  top->OnLButtonDblClk(nFlags, point);

  if(!top->IsActive())
    m_controllers.pop();
  }

//----------------------------------------------------------
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

//------------------------------------------------------------------------------------------------------------
void CGeometryView::OnMButtonUp( UINT nFlags, CPoint point )
  {
  m_is_mode = false;
  }

//------------------------------------------------------------------------------------------------------------
BOOL CGeometryView::OnMouseWheel( UINT fFlags, short zDelta, CPoint point )
  {
  Invalidate();
  return TRUE;
  }

//------------------------------------------------------------------------------------------------------------
void CGeometryView::OnMouseMove( UINT nFlags, CPoint point )
  {
  SetCursor(m_controllers.top()->GetCursor());
  m_controllers.top()->OnMouseMove(nFlags, point);
  }

//------------------------------------------------------------------------------------------------------------
void CGeometryView::OnCreatePoint()
  {
  m_controllers.push(std::unique_ptr<IViewController>(new ViewControllerCreatePoint(this)));
  }

//------------------------------------------------------------------------------------------------------------
void CGeometryView::OnUpdateCreatePoint( CCmdUI* pCmdUI )
  {
  pCmdUI->Enable(TRUE);
  }

//------------------------------------------------------------------------------------------------------------
void CGeometryView::OnUpdate( IUIObject* )
  {
  Invalidate();
  }

#endif //_DEBUG


// CGeometryView message handlers

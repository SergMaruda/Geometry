
// GeometryView.cpp : implementation of the CGeometryView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GeometryApp.h"
#endif

#include "GeometryDoc.h"
#include "GeometryView.h"
#include <functional>
#include "ViewControllers\IViewController.h"
#include "ViewControllers\ViewControllerMovePoint.h"
#include "ViewControllers\ViewControllerCreatePoint.h"
#include "Rendering\IRender.h"
#include "Rendering\RenderFactory.h"
#include "ViewControllers\ViewControllerCreateSegment.h"
#include "Primitives\UISegment.h"
#include "Primitives\Segment2D.h"
#include "Primitives\UIPoint.h"
#include "ViewControllers\ViewControllerSelectObject.h"

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
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_MOUSEMOVE()
  ON_WM_ERASEBKGND()
  ON_WM_SETCURSOR()
  ON_COMMAND(ID_EDIT_CREATEPOINT, OnCreatePoint)
  ON_UPDATE_COMMAND_UI(ID_EDIT_CREATEPOINT, OnUpdateCreatePoint)

  ON_COMMAND(ID_CREATE_SEGMENT, OnCreateSegment)
  ON_UPDATE_COMMAND_UI(ID_CREATE_SEGMENT, OnUpdateCreateSegment)

  ON_COMMAND(ID_EDIT_FINDINTERSECTION, OnFindItersection)
  ON_UPDATE_COMMAND_UI(ID_EDIT_FINDINTERSECTION, OnUpdateFindItersection)

 // ON_WM_VSCROLL() 
 // ON_WM_SIZE()
END_MESSAGE_MAP()

// CGeometryView construction/destruction

//--------------------------------------------------------------------------------------------------------
CGeometryView::CGeometryView()
  {
  Subscribe(OBJECT_ADDED,     this, &CGeometryView::OnUpdate);
  Subscribe(OBJECT_ADDED,     this, &CGeometryView::OnObjectAdded);
  Subscribe(OBJECT_REMOVED,   this, &CGeometryView::OnUpdate);
  Subscribe(OBJECT_REMOVED,   this, &CGeometryView::OnObjectDeleted);
  Subscribe(POINT_CHANGED,    this, &CGeometryView::OnUpdate);
  Subscribe(OBJECT_SELECTED,  this, &CGeometryView::OnUpdate);
  Subscribe(OBJECT_DSELECTED, this,&CGeometryView::OnUpdate);

  SelectController<ViewControllerSelectObject>();
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
// CGeometryView drawing
void CGeometryView::OnDraw(CDC* pDC)
  {
  CGeometryDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if(!pDoc)
    return;


  //Double-buffering to avoid flickering
  CDC dcMem;
  CBitmap bitmap;
  CRect rect;
  GetClientRect(&rect);
  dcMem.CreateCompatibleDC( pDC );
  bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
  CBitmap* pOldBitmap = dcMem.SelectObject(&bitmap);

  CBrush brush;
  brush.CreateSolidBrush(RGB(255,255,255));
  dcMem.FillRect(&rect, &brush);

  for(size_t i = 0; i < m_renders.size(); ++i)
    {
    auto p_obj = m_renders[i]->GetObject();

    int thickness(4);
    COLORREF color = p_obj->GetColor();
    if(pDoc->IsObjectSelected(p_obj))
      {
      thickness = 8;
      color = RGB(255, 0, 0);
      }

    m_renders[i]->SetThickness(thickness);
    m_renders[i]->SetColor(color);
    m_renders[i]->Render(&dcMem);
    }

  pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
  dcMem.SelectObject(pOldBitmap);
  }

//-----------------------------------------------------------------------------
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
  auto p_doc = GetDocument();

  m_controllers.top()->OnLButtonDown(nFlags, point);
  DeselectInActiveController();

  auto p_point = dynamic_cast<UIPoint*>(p_doc->GetPickedObject());
  if(p_point && !IsControllerActive<ViewControllerMovePoint>())
    SelectController<ViewControllerMovePoint>();
  }

//----------------------------------------------------------
void CGeometryView::OnLButtonUp( UINT nFlags, CPoint point )
  {
  m_controllers.top()->OnLButtonUp(nFlags, point);
  DeselectInActiveController();
  }

//----------------------------------------------------------
void CGeometryView::OnLButtonDblClk( UINT nFlags, CPoint point )
  {
  m_controllers.top()->OnLButtonDblClk(nFlags, point);
  DeselectInActiveController();
  }

//----------------------------------------------------------
void CGeometryView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
  {
#ifndef SHARED_HANDLERS
  theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
  }

CGeometryDoc* CGeometryView::GetDocument() const // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGeometryDoc)));
  return (CGeometryDoc*)m_pDocument;
}

//------------------------------------------------------------------------------------------------------------
void CGeometryView::OnMouseMove( UINT nFlags, CPoint point )
  {
  m_controllers.top()->OnMouseMove(nFlags, point);
  DeselectInActiveController();
  }

//------------------------------------------------------------------------------------------------------------
void CGeometryView::OnCreatePoint()
  {
  SelectController<ViewControllerCreatePoint>();
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

//---------------------------------------------------------------------------------------------------------
void CGeometryView::OnObjectAdded( IUIObject* ip_object)
  {
  IRender* p_render = RenderFactory::Instance().CreateRender(ip_object);
  m_renders.push_back(std::unique_ptr<IRender>(p_render));
  }

//---------------------------------------------------------------------------------------------------------
void CGeometryView::OnObjectDeleted( IUIObject* ip_obj)
  {
  size_t i(0);
  for(; i < m_renders.size(); ++i)
    {
    if(m_renders[i]->GetObject() == ip_obj)
      break;
    }

  if(i < m_renders.size())
    m_renders.erase(m_renders.begin() + i);
  }

//---------------------------------------------------------------------------------------------------------
void CGeometryView::OnCreateSegment()
  {
  SelectController<ViewControllerCreateSegment>();
  }

//---------------------------------------------------------------------------------------------------------
void CGeometryView::OnUpdateCreateSegment( CCmdUI* pCmdUI )
  {
  pCmdUI->Enable(TRUE);
  }

//---------------------------------------------------------------------------------------------------------
void CGeometryView::OnFindItersection()
  {
  auto p_doc = CGeometryDoc::GetActive();
  auto segments = CGeometryDoc::GetActive()->GetRootObject().GetChildsByType<UISegment>();

  for(size_t i = 0; i < segments.size(); ++i)
    {
    if(!p_doc->IsObjectSelected(segments[i]))
      {
      segments.erase(segments.begin() + i);
      --i;
      }
    }

  if(segments.size() >= 2)
    {
    auto s1 = segments[0]->GetSegment();
    auto s2 = segments[1]->GetSegment();

    Point2D res;
    if(s2.GetIntersection(res, s1))
      {
      auto& root = p_doc->GetRootObject();
      UIPoint* p_point = new UIPoint(res);
      root.AddChild(p_point);
      }
    }
  }

//---------------------------------------------------------------------------------------------------------
void CGeometryView::OnUpdateFindItersection( CCmdUI* pCmdUI )
  {
  size_t num_selected_segments = CGeometryDoc::GetActive()->GetRootObject().GetChildsByType<UISegment>().size();
  pCmdUI->Enable(num_selected_segments >=2 ? TRUE:FALSE);
  }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
void CGeometryView::SelectController()
  {
  m_controllers.push(std::unique_ptr<IViewController>(new T()));
  }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template <class T>
bool CGeometryView::IsControllerActive()
  {
  auto p_contr = m_controllers.top().get();
  return dynamic_cast<T*>(p_contr) != nullptr;
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CGeometryView::DeselectInActiveController()
  {
  auto top = m_controllers.top().get();
  if(!top->IsActive())
    m_controllers.pop();
  }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CGeometryView::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
  {
  SetCursor(m_controllers.top()->GetCursor());
  return TRUE;
  }

BOOL CGeometryView::OnEraseBkgnd( CDC* pDC )
  {
  return TRUE;

  }

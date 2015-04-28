#include "mainfrm.h"
#include "ObjectsTreeView.h"
#include "Resource.h"
#include "Geometry.h"
#include "GeometryDoc.h"
#include "Primitives\UIPoint.h"
#include "Notifications\NotificationCenter.h"
#include "Primitives\UISegment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const UINT TREE_VIEW_ID = 4;

/////////////////////////////////////////////////////////////////////////////
// CObjectsTreeView

//------------------------------------------------------------------------
CObjectsTreeView::CObjectsTreeView()
  {
  m_connections.push_back(NotificationCenter::Instance().Subscribe(OBJECT_ADDED,   this, &CObjectsTreeView::OnObjectsChanged));
  m_connections.push_back(NotificationCenter::Instance().Subscribe(OBJECT_REMOVED, this, &CObjectsTreeView::OnObjectsChanged));
  m_connections.push_back(NotificationCenter::Instance().Subscribe(LABEL_CHANGED,  this, &CObjectsTreeView::OnObjectsChanged));
  }

//------------------------------------------------------------------------
CObjectsTreeView::~CObjectsTreeView()
  {
  }

//------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CObjectsTreeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
  ON_NOTIFY(TVN_SELCHANGED, TREE_VIEW_ID, &OnSelectionChanged)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

//------------------------------------------------------------------------
int CObjectsTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, TREE_VIEW_ID))
	  {
	  TRACE0("Failed to create file view\n");
		return -1;      // fail to create
  	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillObjectsTree();
	AdjustLayout();

	return 0;
}

//------------------------------------------------------------------------
void CObjectsTreeView::OnSize(UINT nType, int cx, int cy)
  {
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
  }

//------------------------------------------------------------------------
void CObjectsTreeView::FillObjectsTree()
  {
  auto p_doc = CGeometryDoc::GetActive();
  auto& root_obj = p_doc->GetRootObject();

  m_wndFileView.DeleteAllItems();

	HTREEITEM hRoot = m_wndFileView.InsertItem(root_obj.GetLabel(), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hSrc = m_wndFileView.InsertItem(_T("Points"), 0, 0, hRoot);
  HTREEITEM hSegments = m_wndFileView.InsertItem(_T("Segments"), 0, 0, hRoot);

  for(size_t i = 0; i < root_obj.GetNumChilds(); ++i)
    {
    auto p_child = root_obj.GetChild(i);
    auto p_point = dynamic_cast<UIPoint*>(p_child);
    if(p_point)
      {
      HTREEITEM item = m_wndFileView.InsertItem(p_point->GetLabel(), 1, 1, hSrc);
      m_wndFileView.SetItemData(item, (DWORD_PTR)p_point);
      }

    auto p_segm = dynamic_cast<UISegment*>(p_child);
    if(p_segm)
      {
      HTREEITEM item_segm = m_wndFileView.InsertItem(p_segm->GetLabel(), 2, 2, hSegments);
      m_wndFileView.SetItemData(item_segm, (DWORD_PTR)p_segm);
      p_point = p_segm->GetFirstPoint();
      if(p_point)
        {
        HTREEITEM item = m_wndFileView.InsertItem(p_point->GetLabel(), 1, 1, item_segm);
        m_wndFileView.SetItemData(item, (DWORD_PTR)p_point);
        }
      p_point = p_segm->GetSecondPoint();
      if(p_point)
        {
        HTREEITEM item = m_wndFileView.InsertItem(p_point->GetLabel(), 1, 1, item_segm);
        m_wndFileView.SetItemData(item, (DWORD_PTR)p_point);
        }
      m_wndFileView.Expand(item_segm, TVE_EXPAND);

      }
    }

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
	m_wndFileView.Expand(hSegments, TVE_EXPAND);
  }

void CObjectsTreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CObjectsTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CObjectsTreeView::OnProperties()
{
	AfxMessageBox(_T("Properties...."));

}

void CObjectsTreeView::OnFileOpen()
{
	// TODO: Add your command handler code here
}

void CObjectsTreeView::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void CObjectsTreeView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CObjectsTreeView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CObjectsTreeView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CObjectsTreeView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CObjectsTreeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CObjectsTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CObjectsTreeView::OnSelectionChanged( NMHDR *pNMHDR, LRESULT *pResult )
  {
  HTREEITEM item = m_wndFileView.GetSelectedItem();
  UIPoint* p_point =  (UIPoint*)m_wndFileView.GetItemData(item);

  CGeometryDoc::GetActive()->DeselectAllObjects();
  if(p_point)
    CGeometryDoc::GetActive()->SelectObject(p_point);
  }

void CObjectsTreeView::OnObjectsChanged( IUIObject* )
  {
  FillObjectsTree();
  }

void CObjectsTreeView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}



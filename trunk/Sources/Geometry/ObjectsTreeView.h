#pragma once

#include "ViewTree.h"
#include <vector>
#include "Notifications\Observer.h"

struct IUIObject;

class CObjectsTreeViewToolBar : public CMFCToolBar
  {
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	  {
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	  }
	virtual BOOL AllowShowOnList() const { return FALSE; }
  };

class CObjectsTreeView : public CDockablePane, public Observer
{
// Construction
public:
	CObjectsTreeView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// Attributes
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CObjectsTreeViewToolBar m_wndToolBar;

protected:
	void FillObjectsTree();

// Implementation
public:
	virtual ~CObjectsTreeView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnSelectionChanged(NMHDR *pNMHDR, LRESULT *pResult);


	DECLARE_MESSAGE_MAP()
private:
  void OnObjectsChanged(TIUIObjectPtr);
};


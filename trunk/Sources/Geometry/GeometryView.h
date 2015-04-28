// GeometryView.h : interface of the CGeometryView class
//

#pragma once
#include "Notifications\NotificationCenter.h"
#include <stack>


interface IViewController;
interface IRender;
class CGeometryDoc;

class CGeometryView : public CView
{
protected: // create from serialization only
  CGeometryView();

 DECLARE_DYNCREATE(CGeometryView)

// Attributes
public:
 CGeometryDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CGeometryView();

// Generated message map functions
protected:
  // for child windows, views, panes etc

  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnFilePrintPreview();
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);

  afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
  afx_msg void OnCreatePoint();
  afx_msg void OnUpdateCreatePoint(CCmdUI* pCmdUI);;
  afx_msg void OnCreateSegment();
  afx_msg void OnUpdateCreateSegment(CCmdUI* pCmdUI);
  afx_msg void OnFindItersection();
  afx_msg void OnUpdateFindItersection(CCmdUI* pCmdUI);

  void OnUpdate(IUIObject*);
  void OnObjectAdded(IUIObject*);
  void OnObjectDeleted(IUIObject*);

  template <class T>
  void SelectController();

  template <class T>
  bool IsControllerActive();
  
  void DeselectInActiveController();


	DECLARE_MESSAGE_MAP()

  TSubscriptions m_subscriptions;
  std::vector<std::unique_ptr<IRender>> m_renders;
  std::stack<std::unique_ptr<IViewController>> m_controllers;
};


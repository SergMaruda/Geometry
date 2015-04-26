
// GeometryView.h : interface of the CGeometryView class
//

#pragma once


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
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GeometryView.cpp
inline CGeometryDoc* CGeometryView::GetDocument() const
   { return reinterpret_cast<CGeometryDoc*>(m_pDocument); }
#endif


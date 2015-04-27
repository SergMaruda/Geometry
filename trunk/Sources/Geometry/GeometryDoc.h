
// GeometryDoc.h : interface of the CGeometryDoc class
//


#pragma once
#include "Primitives\UIObject.h"
#include <set>


class CGeometryDoc : public CDocument
{
protected: // create from serialization only
	CGeometryDoc();
	DECLARE_DYNCREATE(CGeometryDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
  static CGeometryDoc* GetActive();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CGeometryDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

  UIObject& GetRootObject();
  void SelectObject(IUIObject*);
  void DeselectObject(IUIObject*);
  bool IsObjectSelected( IUIObject*);
  IUIObject* GetFirstSelected();
  void DeselectAllObjects();

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

  private:
    UIObject m_root_object;
    std::set<IUIObject*> m_selected_objects;
};

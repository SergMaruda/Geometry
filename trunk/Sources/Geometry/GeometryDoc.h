
// GeometryDoc.h : interface of the CGeometryDoc class
//


#pragma once
#include "Primitives\UIObject.h"
#include <set>


class CGeometryDoc : public CDocument
{
protected: // create from serialization only
	CGeometryDoc();
  virtual ~CGeometryDoc();
	DECLARE_DYNCREATE(CGeometryDoc)

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

  UIObject& GetRootObject();
  void SelectObject(IUIObject*);
  void DeselectObject(IUIObject*);
  bool IsObjectSelected( IUIObject*);
  IUIObject* GetFirstSelected();
  void DeselectAllObjects();
  void SetPickedObject( IUIObject*);
  IUIObject* GetPickedObject() const;

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
    IUIObject* mp_picked_object;
    std::set<IUIObject*> m_selected_objects;
};

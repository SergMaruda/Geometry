
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "GeometryApp.h"
#include "GeometryDoc.h"
#include "Primitives\UIPoint.h"
#include "Primitives\Point2D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

struct CMFCPropertyGridCtrlMy: public CMFCPropertyGridCtrl
  {
  virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
    {
    CMFCPropertyGridCtrl::OnPropertyChanged(pProp);

    IUIObject* p_obj = CGeometryDoc::GetActive()->GetFirstSelected();
    auto p_point = dynamic_cast<UIPoint*>(p_obj);

    CString prop_name(pProp->GetName());
    std::vector<CMFCPropertyGridProperty*> props;

    if(prop_name == "Label")
      {
      p_point->SetLabel(pProp->GetValue());
      }

    if(prop_name == "Coordinates")
      {
      props.push_back(pProp->GetSubItem(0));
      props.push_back(pProp->GetSubItem(1));
      }
    else
      props.push_back(pProp);

    for(size_t i = 0; i < props.size(); ++i)
      {
      pProp = props[i];
      prop_name = pProp->GetName();

      if(prop_name == L"X" || prop_name == L"Y")
        {
        size_t idx = prop_name == L"X" ? 0 : 1;
        auto new_x = pProp->GetValue().dblVal;
        if(p_obj)
          {
          if(p_point)
            {
            auto p = p_point->GetPoint();
            p[idx] = new_x;
            p_point->SetPoint(p);
            }
          }
        }
      }


    }
  };

//-----------------------------------------------------------------------------------------------
CPropertiesWnd::CPropertiesWnd():
m_wndPropList(new CMFCPropertyGridCtrlMy)
{
m_connections.push_back(NotificationCenter::Instance().Subscribe(OBJECT_SELECTED, std::bind1st(std::mem_fun(&CPropertiesWnd::SelectionChanged), this)));
m_connections.push_back(NotificationCenter::Instance().Subscribe(POINT_CHANGED, std::bind1st(std::mem_fun(&CPropertiesWnd::SelectionChanged), this)));
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	
	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList->SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList->Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList->ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList->SetAlphabeticMode(!m_wndPropList->IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList->IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList->EnableHeaderCtrl(FALSE);
	m_wndPropList->EnableDescriptionArea();
	m_wndPropList->SetVSDotNetLook();
	m_wndPropList->MarkModifiedProperties();

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Coordinates"), 0, TRUE);
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("X"), (_variant_t) 0.0, _T("Specifies X Coordinate"));
	pSize->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty( _T("Y"), (_variant_t) 0.0, _T("Specifies Y Coordinate"));
	pSize->AddSubItem(pProp);
	m_wndPropList->AddProperty(pSize);

  CMFCPropertyGridProperty* pLabel = new CMFCPropertyGridProperty(_T("Label"), (_variant_t)"", _T("Object name"));
  m_wndPropList->AddProperty(pLabel);
	}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
  {
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList->SetFocus();
  }

//------------------------------------------------------------------------------------
void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList->SetFont(&m_fntPropList);
}

void CPropertiesWnd::SelectionChanged( IUIObject* )
  {
  auto prop = m_wndPropList->GetProperty(0);
  auto p_selected = CGeometryDoc::GetActive()->GetFirstSelected();
  auto p_point = dynamic_cast<UIPoint*>(p_selected);

  if(p_point)
    {
    auto p = p_point->GetPoint();
    auto sub_1prop  = prop->GetSubItem(0);
    sub_1prop->SetValue(p[0]);
    auto sub_2prop  = prop->GetSubItem(1);
    sub_2prop->SetValue(p[1]);
    CString str = p_point->GetLabel();
    m_wndPropList->GetProperty(1)->SetValue(str);
    }
  }

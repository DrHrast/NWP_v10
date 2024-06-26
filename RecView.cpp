// RecView.cpp : implementation of the RecView class
//

#include "stdafx.h"
#include "v10.h"

#include "Set.h"
#include "Doc.h"
#include "RecView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RecView

IMPLEMENT_DYNCREATE(RecView, CRecordView)

BEGIN_MESSAGE_MAP(RecView, CRecordView)
	//{{AFX_MSG_MAP(RecView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRecordView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RecView construction/destruction

RecView::RecView()
	: CRecordView(RecView::IDD)
{
}

RecView::~RecView()
{
}

void RecView::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RecView)
	DDX_FieldText(pDX, IDC_EDIT1, m_pSet->m_id, m_pSet);
	DDX_FieldText(pDX, IDC_EDIT2, m_pSet->m_name, m_pSet);
	DDX_FieldCheck(pDX, IDC_CHECK1, m_pSet->m_manager, m_pSet);
	//}}AFX_DATA_MAP
}

BOOL RecView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CRecordView::PreCreateWindow(cs);
}

void RecView::OnInitialUpdate()
{
	m_pSet = &GetDocument()->m_set;
	CRecordView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// RecView printing

BOOL RecView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void RecView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void RecView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void RecView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	const int pageWidth = pInfo->m_rectDraw.Width();
	const int lineHeight = pDC->GetTextExtent(_T("Petar")).cy;
	const float borderHeight = lineHeight + lineHeight / 1.5;

	Set rs;
	rs.Open();
	const float topMargin = pInfo->m_rectDraw.Height() - (pInfo->m_rectDraw.Height() * 0.850);
	const float leftMargin = pageWidth * 0.15;
	const float rightMargin = pageWidth - leftMargin;
	float yPos = topMargin;

	pDC->TextOut(leftMargin, yPos, _T("ID"));
	pDC->TextOut(pageWidth / 4, yPos, _T("Name"));
	pDC->TextOut((pageWidth / 2), yPos, _T("Manager"));

	yPos += borderHeight;
	pDC->MoveTo(leftMargin, yPos);
	pDC->LineTo(rightMargin, yPos);

	yPos += borderHeight;

	CPen pen(PS_DASH, 2, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	while(!rs.IsEOF())
	{
		CString id;
		CString manager;
		id.Format(_T("%d"), rs.m_id);
		pDC->TextOut(leftMargin, yPos, id);
		pDC->TextOut(pageWidth / 4, yPos, rs.m_name);
		if (rs.m_manager) {
			manager = _T("x");
			pDC->TextOut((pageWidth / 2), yPos, manager);
		}
		
		rs.MoveNext();
		if(!rs.IsEOF()) 
		{
			yPos += borderHeight;
			pDC->MoveTo(leftMargin, yPos);
			pDC->LineTo(rightMargin, yPos);
		
			yPos += lineHeight;
		}
		else 
		{
			pDC->SelectObject(pOldPen);
			yPos += borderHeight;
			pDC->MoveTo(leftMargin, yPos);
			pDC->LineTo(rightMargin, yPos);
			yPos += lineHeight * 0.8;
			pDC->MoveTo(leftMargin, yPos);
			pDC->LineTo(rightMargin, yPos);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// RecView diagnostics

#ifdef _DEBUG
void RecView::AssertValid() const
{
	CRecordView::AssertValid();
}

void RecView::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
}

Doc* RecView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Doc)));
	return (Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// RecView database support
CRecordset* RecView::OnGetRecordset()
{
	return m_pSet;
}


/////////////////////////////////////////////////////////////////////////////
// RecView message handlers


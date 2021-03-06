// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MyPaintView.cpp : implementation of the CMyPaintView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MyPaint.h"
#endif

#include "MyPaintDoc.h"
#include "MyPaintView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyPaintView

IMPLEMENT_DYNCREATE(CMyPaintView, CView)

BEGIN_MESSAGE_MAP(CMyPaintView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_PEN_THICKNESS, &CMyPaintView::OnPenThickness)
	ON_COMMAND(ID_PEN_COLOR, &CMyPaintView::OnColorChanged)
	ON_COMMAND(ID_FIGURE, &CMyPaintView::OnFigureChanged)
END_MESSAGE_MAP()

// CMyPaintView construction/destruction

CMyPaintView::CMyPaintView():penWidth(1), penColor(RGB(0, 0, 0))
{
	selectedShape = "None";
	growingStroke = new Commands::StrokeCommand();
	tempShape = nullptr;
}



Shapes::Shape * CMyPaintView::BuildShape(CString& shape, CPoint& point)
{
	Shapes::Shape* newShape = nullptr;
	if (selectedShape == "Rectangle")
	{
		CRect rect(previousMousePoint, point);
		newShape = new Shapes::Rectangle(rect, penWidth, penColor);
	}
	else if (selectedShape == "Ellipse")
	{
		CRect rect(previousMousePoint, point);
		newShape = new Shapes::Ellipse(rect, penWidth, penColor);
	}
	else
	{
		throw new CNotSupportedException;
	}

	return newShape;
}

CMyPaintView::~CMyPaintView()
{

}

BOOL CMyPaintView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMyPaintView drawing

void CMyPaintView::OnDraw(CDC* pDC)
{

	CMyPaintDoc* pDoc = (CMyPaintDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CMemDC memDC(*pDC, this);
	CDC* dc = &memDC.GetDC();
	CRect rect;
	GetClientRect(&rect);
	dc->FillSolidRect(rect, RGB(255, 255, 255));
	for (auto &x : pDoc->commandManager.GetCommands())
	{
		x->Execute(dc);
	}
	if (growingStroke)
		growingStroke->Execute(dc);
	if (tempShape)
		tempShape->Draw(dc);
}

void CMyPaintView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMyPaintView::OnLButtonUp(UINT nFrags, CPoint point)
{
	if (selectedShape == "None")
	{
		((CMyPaintDoc*)GetDocument())->commandManager.AddCommand(growingStroke);
		growingStroke = new Commands::StrokeCommand();
	}
	else if (selectedShape != "None" && tempShape != nullptr)
	{
		auto shapeCmd = new Commands::ShapeCommand(tempShape);
		((CMyPaintDoc*)GetDocument())->commandManager.AddCommand(shapeCmd);
		tempShape = nullptr;
	}
	CView::OnLButtonUp(nFrags, point);

}

void CMyPaintView::OnLButtonDown(UINT nFrags, CPoint point)
{
	previousMousePoint = point;
	CView::OnLButtonDown(nFrags, point);
}

void CMyPaintView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (selectedShape == "None" && (GetKeyState(VK_LBUTTON) & 0x100) != 0 && previousMousePoint != point)
	{
		growingStroke->addStroke(new Shapes::Line(previousMousePoint,
			point, penWidth, penColor));
		previousMousePoint = point;
	}
	else if (selectedShape != "None" && (GetKeyState(VK_LBUTTON) & 0x100) != 0)
	{
		if (tempShape)
			delete tempShape;
		tempShape = BuildShape(selectedShape, point);
		
	}
	Invalidate(true);
	CView::OnMouseMove(nFlags, point);
	
}

void CMyPaintView::OnContextMenu(CWnd*  pWnd , CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}




// CMyPaintView diagnostics

#ifdef _DEBUG
void CMyPaintView::AssertValid() const
{
	CView::AssertValid();
}

void CMyPaintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

/*CMyPaintDoc * CMyPaintView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyPaintDoc)));
	return (CMyPaintDoc*)m_pDocument;
}
*/
#endif // _DEBUG




// CMyPaintView message handlers


void CMyPaintView::OnPenThickness()
{
	CMFCRibbonBar* pRibbon = ((CMainFrame *)AfxGetMainWnd())->GetRibbonBar();
	ASSERT_VALID(pRibbon);
	CMFCRibbonSlider* slider = DYNAMIC_DOWNCAST(
		CMFCRibbonSlider, pRibbon->FindByID(ID_PEN_THICKNESS));
	penWidth =  slider->GetPos();
}


BOOL CMyPaintView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}




void CMyPaintView::OnColorChanged()
{
	CMFCRibbonBar* pRibbon = ((CMainFrame *)AfxGetMainWnd())->GetRibbonBar();
	ASSERT_VALID(pRibbon);
	CMFCRibbonColorButton* button = DYNAMIC_DOWNCAST(
		CMFCRibbonColorButton, pRibbon->FindByID(ID_PEN_COLOR));

	penColor = button->GetColor();
}




void CMyPaintView::OnFigureChanged()
{
	CMFCRibbonBar* pRibbon = ((CMainFrame *)AfxGetMainWnd())->GetRibbonBar();
	ASSERT_VALID(pRibbon);
	CMFCRibbonComboBox* listBox = DYNAMIC_DOWNCAST(
		CMFCRibbonComboBox, pRibbon->FindByID(ID_FIGURE));

	selectedShape = listBox->GetItem(listBox->GetCurSel());
}



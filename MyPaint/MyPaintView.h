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

// MyPaintView.h : interface of the CMyPaintView class
//

#pragma once
#include "Commands.h"


class CMyPaintView : public CView
{
protected: // create from serialization only
	CMyPaintView();
	DECLARE_DYNCREATE(CMyPaintView)

// Operations

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:
	UINT								 penWidth;
	CPoint								 previousMousePoint;
	COLORREF							 penColor;
	CString								 selectedShape;
	Shapes::Shape*						 tempShape;
	Commands::StrokeCommand*			 growingStroke;
	Shapes::Shape* BuildShape(CString& shape, CPoint& point);

	
// Implementation
public:
	virtual ~CMyPaintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif



// Generated message map functions
protected:
	afx_msg void OnRButtonUp(UINT nFlags,   CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags,   CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd,  CPoint point);
	afx_msg void OnMouseMove(UINT nFlags,   CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPenThickness();
	afx_msg void OnColorChanged();
	afx_msg void OnFigureChanged();
	DECLARE_MESSAGE_MAP()
public:
};

#ifndef _DEBUG  // debug version in ScribVw.cpp
inline CMyPaintDoc* CScribbleView::GetDocument()
{
	return (CMyPaintDoc*)m_pDocument;
}
#endif

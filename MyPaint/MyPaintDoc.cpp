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

// MyPaintDoc.cpp : implementation of the CMyPaintDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MyPaint.h"
#endif

#include "MyPaintDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMyPaintDoc

IMPLEMENT_DYNCREATE(CMyPaintDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyPaintDoc, CDocument)
	ON_COMMAND(ID_UNDO, &CMyPaintDoc::OnUndo)
	ON_COMMAND(ID_REDO, &CMyPaintDoc::OnRedo)
END_MESSAGE_MAP()


// CMyPaintDoc construction/destruction

CMyPaintDoc::CMyPaintDoc()
{
	// TODO: add one-time construction code here

}

CMyPaintDoc::~CMyPaintDoc()
{
}


BOOL CMyPaintDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	commandManager.AddCommand(new Commands::NewDocumentCommand());
	return TRUE;
}




// CMyPaintDoc serialization

void CMyPaintDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

BOOL CMyPaintDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CImage image;
	CDC* pDC = CDC::FromHandle(GetDC(GetActiveWindow()));
	CMemDC memDC(*pDC, AfxGetMainWnd());
	CDC* dc = &memDC.GetDC();
	CRect rect;
	GetClientRect(GetActiveWindow(), rect);
	dc->FillSolidRect(rect, RGB(255, 255, 255));

	for (auto &x : commandManager.GetCommands())
	{
		x->Execute(dc);
	}

	HBITMAP h = *dc->GetCurrentBitmap();
	image.Attach(h);
	image.Save(lpszPathName);
	AfxGetMainWnd()->Invalidate();
	return 1;
}

BOOL CMyPaintDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	OnNewDocument();
	commandManager.AddCommand(new Commands::OpenDocumentCommand(lpszPathName));
	return 1;
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMyPaintDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMyPaintDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMyPaintDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMyPaintDoc diagnostics

#ifdef _DEBUG
void CMyPaintDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyPaintDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG



void CMyPaintDoc::OnUndo()
{
	this->commandManager.Undo();
	AfxGetMainWnd()->Invalidate();

}


void CMyPaintDoc::OnRedo()
{
	this->commandManager.Redo();
	AfxGetMainWnd()->Invalidate();
}



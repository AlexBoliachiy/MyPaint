#include "stdafx.h"
#include "Shapes.h"
#include "MyPaintView.h"


using namespace Shapes;

/********************************/
/* Shape */
void Shapes::Shape::setCustomPenAndNullBrush(CDC* dc)
{
	CPen* pen = new CPen(PS_SOLID, width, color);
	originalPen = dc->SelectObject(pen);
	originalBrush = (CBrush*)dc->SelectStockObject(NULL_BRUSH);
}


void Shapes::Shape::setOriginalPenAndBrush(CDC* dc)
{
	delete (dc->SelectObject(originalPen));
	dc->SelectObject(originalBrush);
}


void Shapes::Shape::Draw(CDC* dc)
{
	setCustomPenAndNullBrush(dc);
	draw(dc);
	setOriginalPenAndBrush(dc);
}
Shapes::Shape::Shape(int width, COLORREF color)
{	
	this->width = width;
	this->color = color;
}
/********************************/
/********************************/

/********************************/
/* Line */
Shapes::Line::Line(CPoint start, CPoint end, int width, COLORREF color) : 
	start(start), end(end), Shape(width, color)
{

}


void Shapes::Line::draw(CDC* pdc)
{
	pdc->MoveTo(start);
	pdc->LineTo(end);
}
/********************************/
/********************************/

/********************************/
/* Rect */
Shapes::Rectangle::Rectangle(CRect& rect, int width, COLORREF color) : 
	Shape(width, color), rect(rect)
{

}


void Shapes::Rectangle::draw(CDC* pdc)
{
	pdc->Rectangle(rect);
}
/********************************/
/********************************/

void Shapes::Ellipse::draw(CDC * pdc)
{
	pdc->Ellipse(rect);
}

/********************************/
/* Rect */
Shapes::Ellipse::Ellipse(CRect& rect, int width, COLORREF color) :
	Rectangle(rect, width, color)
{

}
/********************************/
/********************************/
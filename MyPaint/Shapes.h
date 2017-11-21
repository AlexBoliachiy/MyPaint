#pragma once




namespace Shapes 
{

	class Shape
	{

	private:
		CPen* originalPen;
		CBrush* originalBrush;

	private:
		virtual void draw(CDC* pdc) = 0;

	protected:
		COLORREF color;
		int		 width;

	protected:
		void setCustomPenAndNullBrush(CDC* dc);
		void setOriginalPenAndBrush(CDC* dc);

	public:
		
		void Draw(CDC* dc);
		Shape(int width, COLORREF color);
		
	};



	class Line : public Shape
	{

	private:
		CPoint start;
		CPoint end;

	private:
		virtual void draw(CDC* pdc) override;

	public:
		Line(CPoint start, CPoint end, int width, COLORREF color);
		
	};


	class Rectangle: public Shape
	{

	protected:
		CRect rect;

	private:
		virtual void draw(CDC* pdc) override;


	public:
		Rectangle(CRect& rect, int width, COLORREF color);
		
	};



	class Ellipse : public Rectangle
	{

	private:
		virtual void draw(CDC* pdc) override;


	public:
		Ellipse(CRect& rect, int width, COLORREF color);

	};

}







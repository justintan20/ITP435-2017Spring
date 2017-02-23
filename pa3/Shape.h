#pragma once
#include <wx/dc.h>

// Abstract base class for all Shapes
class Shape
{
public:
	Shape(const wxPoint& start);
	// Tests whether the provided point intersects
	// with this shape
	bool Intersects(const wxPoint& point) const;
	// Update shape with new provided point
	virtual void Update(const wxPoint& newPoint);
	// Finalize the shape -- when the user has finished drawing the shape
	virtual void Finalize();
	// Returns the top left/bottom right points of the shape
	void GetBounds(wxPoint& topLeft, wxPoint& botRight) const;
	// Draw the shape
	virtual void Draw(wxDC& dc) const = 0;
    
    virtual void SetPen(const wxPen& pen);
    virtual wxPen GetPen() const;
    
    virtual void SetBrush(const wxBrush& brush);
    virtual wxBrush GetBrush() const;
    
    virtual void DrawSelection(wxDC& dc) const;
    
    void SetOffset(const wxPoint& offset);
    wxPoint GetOffset() const;
	virtual ~Shape() { }
protected:
	// Starting point of shape
	wxPoint mStartPoint;
	// Ending point of shape
	wxPoint mEndPoint;
	// Top left point of shape
	wxPoint mTopLeft;
	// Bottom right point of shape
	wxPoint mBotRight;
    
    wxPen mPen = *wxBLACK_PEN;
    wxBrush mBrush = *wxWHITE_BRUSH;
    
    wxPoint mOffset;
};

class RectShape : public Shape
{
public:
    RectShape(const wxPoint& start);
    void Draw(wxDC& dc) const override;
};

class EllipseShape: public Shape
{
public:
    EllipseShape(const wxPoint& start);
    void Draw(wxDC& dc) const override;
};

class LineShape: public Shape
{
public:
    LineShape(const wxPoint& start);
    void Draw(wxDC& dc) const override;
};

class PencilShape: public Shape
{
public:
    PencilShape(const wxPoint& start);
    void Draw(wxDC& dc) const override;
    void Update(const wxPoint& newPoint) override;
    void Finalize() override;
private:
    std::vector<wxPoint> mPoints;
};

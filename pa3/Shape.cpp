#include "Shape.h"

Shape::Shape(const wxPoint& start)
	:mStartPoint(start)
	,mEndPoint(start)
	,mTopLeft(start)
	,mBotRight(start)
{
    mOffset.x = 0;
    mOffset.y = 0;
}

// Tests whether the provided point intersects
// with this shape
bool Shape::Intersects(const wxPoint& point) const
{
	wxPoint topleft;
	wxPoint botright;
	GetBounds(topleft, botright);
	if (point.x >= topleft.x && point.x <= botright.x &&
		point.y >= topleft.y && point.y <= botright.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Update shape with new provided point
void Shape::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;

	// For most shapes, we only have two points - start and end
	// So we can figure out the top left/bottom right bounds
	// based on this.
	mTopLeft.x = std::min(mStartPoint.x, mEndPoint.x);
	mTopLeft.y = std::min(mStartPoint.y, mEndPoint.y);
	mBotRight.x = std::max(mStartPoint.x, mEndPoint.x);
	mBotRight.y = std::max(mStartPoint.y, mEndPoint.y);
}

void Shape::Finalize()
{
	// Default finalize doesn't do anything
}

void Shape::GetBounds(wxPoint& topLeft, wxPoint& botRight) const
{
	topLeft.x = mTopLeft.x + mOffset.x;
    topLeft.y = mTopLeft.y + mOffset.y;
    botRight.x = mBotRight.x + mOffset.x;
    botRight.y = mBotRight.y + mOffset.y;
}

void Shape::SetPen(const wxPen& pen)
{
    mPen = pen;
}

wxPen Shape::GetPen() const
{
    return mPen;
}

void Shape::SetBrush(const wxBrush& brush)
{
    mBrush = brush;
}

wxBrush Shape::GetBrush() const
{
    return mBrush;
}

void Shape::DrawSelection(wxDC& dc) const
{
    dc.SetPen(*wxBLACK_DASHED_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    wxPoint topLeft;
    wxPoint botRight;
    topLeft.x = mTopLeft.x + mOffset.x - 10;
    topLeft.y = mTopLeft.y + mOffset.y - 10;
    botRight.x = mBotRight.x + mOffset.x + 10;
    botRight.y = mBotRight.y + mOffset.y + 10;
    wxRect rect(topLeft,botRight);
    dc.DrawRectangle(rect);
}

void Shape::SetOffset(const wxPoint& offset)
{
    mOffset = offset;
}

wxPoint Shape::GetOffset() const
{
    return mOffset;
}

RectShape::RectShape(const wxPoint& start): Shape(start)
{
    
}

void RectShape::Draw(wxDC& dc) const
{
    dc.SetPen(mPen);
    dc.SetBrush(mBrush);
    wxRect rect(mTopLeft + mOffset, mBotRight + mOffset);
    dc.DrawRectangle(rect);
}

EllipseShape::EllipseShape(const wxPoint& start): Shape(start)
{
    
}

void EllipseShape::Draw(wxDC& dc) const
{
    dc.SetPen(mPen);
    dc.SetBrush(mBrush);
    wxRect rect(mTopLeft + mOffset, mBotRight + mOffset);
    dc.DrawEllipse(rect);
}

LineShape::LineShape(const wxPoint& start): Shape(start)
{
    
}

void LineShape::Draw(wxDC& dc) const
{
    dc.SetPen(mPen);
    dc.SetBrush(mBrush);
    dc.DrawLine(mStartPoint + mOffset, mEndPoint + mOffset);
}

PencilShape::PencilShape(const wxPoint& start): Shape(start)
{
    mPoints.push_back(start);
}

void PencilShape::Draw(wxDC& dc) const
{
    dc.SetPen(mPen);
    dc.SetBrush(mBrush);
    if(mPoints.size() == 1)
    {
        dc.DrawPoint(mPoints[0]);
    }
    else
    {
        dc.DrawLines(static_cast<int>(mPoints.size()), mPoints.data(), mOffset.x, mOffset.y);
    }
}

void PencilShape::Update(const wxPoint& newPoint)
{
    Shape::Update(newPoint);
    mPoints.push_back(newPoint);
}

void PencilShape::Finalize()
{
    mTopLeft = mPoints[0];
    mBotRight = mPoints[0];
    for(int i = 1; i < mPoints.size(); i++)
    {
        if(mPoints[i].x < mTopLeft.x)
        {
            mTopLeft.x = mPoints[i].x;
        }
        if(mPoints[i].y < mTopLeft.y)
        {
            mTopLeft.y = mPoints[i].y;
        }
        if(mPoints[i].x > mBotRight.x)
        {
            mBotRight.x = mPoints[i].x;
        }
        if(mPoints[i].y > mBotRight.y)
        {
            mBotRight.y = mPoints[i].y;
        }
    }
}

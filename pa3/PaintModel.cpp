#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>

PaintModel::PaintModel()
{
	
}

// Draws any shapes in the model to the provided DC (draw context)
void PaintModel::DrawShapes(wxDC& dc, bool showSelection)
{
	// TODO
    if(mStoredBitmap.IsOk())
    {
        dc.DrawBitmap(mStoredBitmap, wxPoint());
    }
    for(int i = 0; i < mShapes.size(); i++)
    {
        mShapes[i]->Draw(dc);
    }
    if(mSelectedShape != nullptr)
    {
        mSelectedShape->DrawSelection(dc);
    }
}

// Clear the current paint model and start fresh
void PaintModel::New()
{
	// TODO
    mCommand.reset();
    mShapes.clear();
    while (mUndo.empty() == false)
    {
        mUndo.pop();
    }
    while (mRedo.empty() == false)
    {
        mRedo.pop();
    }
    mPen = *wxBLACK_PEN;
    mBrush = *wxWHITE_BRUSH;
    mSelectedShape.reset();
    mStoredBitmap = wxBitmap();
}

// Add a shape to the paint model
void PaintModel::AddShape(std::shared_ptr<Shape> shape)
{
	mShapes.emplace_back(shape);
}

// Remove a shape from the paint model
void PaintModel::RemoveShape(std::shared_ptr<Shape> shape)
{
	auto iter = std::find(mShapes.begin(), mShapes.end(), shape);
	if (iter != mShapes.end())
	{
		mShapes.erase(iter);
	}
}

bool PaintModel::HasActiveCommand() const
{
    if(mCommand == nullptr)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void PaintModel::CreateCommand(CommandType type, const wxPoint& start)
{
    mCommand = CommandFactory::Create(shared_from_this(), type, start);
    while(mRedo.empty() == false)
    {
        mRedo.pop();
    }
}

void PaintModel::UpdateCommand(const wxPoint& point)
{
    mCommand->Update(point);
    while(mRedo.empty() == false)
    {
        mRedo.pop();
    }
}

void PaintModel::FinalizeCommand()
{
    mCommand->Finalize(shared_from_this());
    mUndo.push(mCommand);
    mCommand.reset();
}

bool PaintModel::CanUndo() const
{
    if(mUndo.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool PaintModel::CanRedo() const
{
    if(mRedo.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void PaintModel::Undo()
{
    mCommand = mUndo.top();
    mCommand->Undo(shared_from_this());
    mUndo.pop();
    mRedo.push(mCommand);
    if(mSelectedShape == mCommand->GetShape())
    {
        mSelectedShape.reset();
    }
    mCommand.reset();
}

void PaintModel::Redo()
{
    mCommand = mRedo.top();
    mCommand->Redo(shared_from_this());
    mRedo.pop();
    mUndo.push(mCommand);
    mCommand.reset();
}

void PaintModel::SetPenColour(const wxColour& colour)
{
    mPen.SetColour(colour);
}

wxColour PaintModel::GetPenColour() const
{
    return mPen.GetColour();
}

void PaintModel::SetPenWidth(int width)
{
    mPen.SetWidth(width);
}

int PaintModel::GetPenWidth() const
{
    return mPen.GetWidth();
}

void PaintModel::SetBrushColour(const wxColour& colour)
{
    mBrush.SetColour(colour);
}

wxColour PaintModel::GetBrushColour() const
{
    return mBrush.GetColour();
}

wxPen PaintModel::GetPen() const
{
    return mPen;
}

wxBrush PaintModel::GetBrush() const
{
    return mBrush;
}

void PaintModel::SelectShape(const wxPoint& point)
{
    mSelectedShape.reset();
    for(int i = static_cast<int>(mShapes.size() - 1); i > -1; i--)
    {
        wxPoint topLeft;
        wxPoint botRight;
        mShapes[i]->GetBounds(topLeft, botRight);
        if(point.x > topLeft.x && point.y > topLeft.y && point.x < botRight.x && point.y < botRight.y)
        {
            mSelectedShape = mShapes[i];
            break;
        }
    }
}

std::shared_ptr<Shape> PaintModel::GetSelectedShape()
{
    return mSelectedShape;
}

void PaintModel::Export(const wxString& fileName, const wxSize& size)
{
    wxBitmap bitmap;
    // Create the bitmap of the specified wxSize
    bitmap.Create(size);
    // Create a memory DC to draw to the bitmap
    wxMemoryDC dc(bitmap);
    // Clear the background color
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    // Draw all the shapes (make sure not the selection!)
    DrawShapes(dc);
    wxBitmapType type;
    wxString extension = fileName.AfterLast('.');
    if(extension.Cmp("jpeg") == 0)
    {
        type = wxBITMAP_TYPE_JPEG;
    }
    else if(extension.Cmp("bmp") == 0)
    {
        type = wxBITMAP_TYPE_BMP;
    }
    else if(extension.Cmp("png") == 0)
    {
        type = wxBITMAP_TYPE_PNG;
    }
    // Write the bitmap with the specified file name and wxBitmapType
    bitmap.SaveFile(fileName, type);
}

void PaintModel::Load(const wxString& fileName, const wxBitmapType& type){
    New();
    mStoredBitmap.LoadFile(fileName, type);
    wxMemoryDC dc(mStoredBitmap);
    DrawShapes(dc);
}

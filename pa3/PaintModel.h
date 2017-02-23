#pragma once
#include <memory>
#include <vector>
#include "Shape.h"
#include "Command.h"
#include <wx/bitmap.h>
#include <stack>

class PaintModel : public std::enable_shared_from_this<PaintModel>
{
public:
	PaintModel();
	
	// Draws any shapes in the model to the provided DC (draw context)
	void DrawShapes(wxDC& dc, bool showSelection = true);

	// Clear the current paint model and start fresh
	void New();

	// Add a shape to the paint model
	void AddShape(std::shared_ptr<Shape> shape);
	// Remove a shape from the paint model
	void RemoveShape(std::shared_ptr<Shape> shape);
    
    bool HasActiveCommand() const;
    
    void CreateCommand(CommandType type, const wxPoint& start);
    
    void UpdateCommand(const wxPoint& point);
    
    void FinalizeCommand();
    
    bool CanUndo() const;
    bool CanRedo() const;
    
    void Undo();
    void Redo();
    
    void SetPenColour(const wxColour& colour);
    wxColour GetPenColour() const;
    void SetPenWidth(int width);
    int GetPenWidth() const;
    
    void SetBrushColour(const wxColour& colour);
    wxColour GetBrushColour() const;
    
    wxPen GetPen() const;
    wxBrush GetBrush() const;
    
    void SelectShape(const wxPoint& point);
    
    std::shared_ptr<Shape> GetSelectedShape();
    
    void Export(const wxString& fileName, const wxSize& size);
    void Load(const wxString& fileName, const wxBitmapType& type);
private:
	// Vector of all the shapes in the model
	std::vector<std::shared_ptr<Shape>> mShapes;
    std::shared_ptr<Command> mCommand;
    std::stack<std::shared_ptr<Command>> mUndo;
    std::stack<std::shared_ptr<Command>> mRedo;
    wxPen mPen = *wxBLACK_PEN;
    wxBrush mBrush = *wxWHITE_BRUSH;
    std::shared_ptr<Shape> mSelectedShape;
    wxBitmap mStoredBitmap;
};

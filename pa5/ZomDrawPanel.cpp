//
//  ZomDrawPanel.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include "Machine.h"
#include "ZomWorld.h"

BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel)
	EVT_PAINT(ZomDrawPanel::PaintEvent)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void ZomDrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void ZomDrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void ZomDrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw the grid
	DrawGrid(dc);
}

void ZomDrawPanel::DrawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);
    for(int i = 1; i < 20; i++)
    {
        dc.DrawLine(10, 10+30*i, 610, 10+30*i);
        dc.DrawLine(10+30*i, 10, 10+30*i, 610);
    }
    dc.SetBrush(*wxRED_BRUSH);
    for(int i = 0; i < ZomWorld::get().GetNumZombies(); i++)
    {
        std::shared_ptr<MachineState> currState = ZomWorld::get().GetZombieState(i);
        int x = currState->mX + 10;
        int y = currState->mY + 10;
        wxPoint pt1;
        wxPoint pt2;
        wxPoint pt3;
        switch (currState->mFacing)
        {
            case (MachineState::UP) :
                pt1 = wxPoint(x, y+30);
                pt2 = wxPoint(x+30, y+30);
                pt3 = wxPoint(x+15, y);
                break;
            case (MachineState::RIGHT) :
                pt1 = wxPoint(x, y);
                pt2 = wxPoint(x, y+30);
                pt3 = wxPoint(x+30, y+15);
                break;
            case (MachineState::DOWN) :
                pt1 = wxPoint(x, y);
                pt2 = wxPoint(x+15, y+30);
                pt3 = wxPoint(x+30, y);
                break;
            case (MachineState::LEFT) :
                pt1 = wxPoint(x+30, y);
                pt2 = wxPoint(x, y+15);
                pt3 = wxPoint(x+30, y+30);
                break;
        }
        wxPoint points[3] = {pt1, pt2, pt3};
        dc.DrawPolygon(3, points);
    }
}

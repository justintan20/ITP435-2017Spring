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
#include <string>

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
	//draw grid
    dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);
    for(int i = 1; i < 20; i++)
    {
        dc.DrawLine(10, 10+30*i, 610, 10+30*i);
        dc.DrawLine(10+30*i, 10, 10+30*i, 610);
    }
    //draw zombies
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
    //draw humans
    dc.SetBrush(*wxGREEN_BRUSH);
    for(int i = 0; i < ZomWorld::get().GetNumHumans(); i++)
    {
        std::shared_ptr<MachineState> currState = ZomWorld::get().GetHumanState(i);
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
    //draw info text
    dc.SetTextForeground(*wxRED);
    dc.DrawText("Zombies Program:", 620, 10);
    std::string zomFilename =ZomWorld::get().GetZombieFileName();
    if(zomFilename.size() != 0)
    {
        std::string actualFile =zomFilename.substr(zomFilename.find_last_of("/") + 1);
        dc.DrawText(actualFile, 620, 25);
    }
    else
    {
        dc.DrawText("Not loaded.", 620, 25);
    }
    dc.SetTextForeground(*wxGREEN);
    dc.DrawText("Humans Program:", 620, 100);
    std::string humFilename =ZomWorld::get().GetHumanFileName();
    if(humFilename.size() != 0)
    {
        std::string actualFile =humFilename.substr(humFilename.find_last_of("/") + 1);
        dc.DrawText(actualFile, 620, 115);
    }
    else
    {
        dc.DrawText("Not loaded.", 620, 115);
    }
    dc.SetTextForeground(*wxBLACK);
    dc.DrawText("Month: " + std::to_string(ZomWorld::get().GetMonth()), 620, 200);
}

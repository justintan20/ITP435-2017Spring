//
//  ZomFrame.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"
#include "ZomWorld.h"

enum
{
	ID_SImSTART=1000,
	ID_TURN_TIMER,
    ID_LOAD_ZOMBIE,
    ID_LOAD_SURVIVOR
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame)
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SImSTART, ZomFrame::OnSimStart)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
    EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::OnLoadZom)
wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
, mIsActive(false)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(wxID_EXIT);
	
	// Simulation menu
	mSimMenu = new wxMenu;
	mSimMenu->Append(ID_SImSTART, "Start/stop\tSpace", "Start or stop the simulation");
    mSimMenu->Append(ID_LOAD_ZOMBIE, "Load Zombie");
	
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(mSimMenu, "&Simulation");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new ZomDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	mTurnTimer = new wxTimer(this, ID_TURN_TIMER);
    ZomWorld::get().NewWorld();
	// TEMP CODE: Initialize zombie test machine
//	zombieMachine.LoadMachine(std::string(""));
//	zombieMachine.BindState(zombieTestState);
	// END TEMP CODE
}

ZomFrame::~ZomFrame()
{
	delete mTurnTimer;
}

void ZomFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::OnNew(wxCommandEvent& event)
{
	// TODO: Add code for File>New
}

void ZomFrame::OnSimStart(wxCommandEvent& event)
{
	if (!mIsActive)
	{
		// Add timer to run once per second
		mTurnTimer->Start(1000);
		mIsActive = true;
	}
	else
	{
		mTurnTimer->Stop();
		mIsActive = false;
	}
}

void ZomFrame::OnTurnTimer(wxTimerEvent& event)
{
	// TEMP CODE: Take turn for zombie machine
    for(int i = 0; i < ZomWorld::get().GetNumZombies(); i++)
    {
        ZomWorld::get().GetZombie(i)->TakeTurn(*ZomWorld::get().GetZombieState(i));
    }
//	zombieMachine.TakeTurn(zombieTestState);
    mPanel->PaintNow();
	// END TEMP CODE
}

void ZomFrame::OnLoadZom(wxCommandEvent& event)
{
    wxFileDialog dialog(this, _("Load Zombie"),"./zom","","ZOM Files|*.zom");
    if(dialog.ShowModal() == wxID_OK)
    {
        std::string fileName = dialog.GetPath().ToStdString();
        Machine<ZombieTraits> zombie;
        zombie.LoadMachine(fileName);
        ZomWorld::get().AddZombie(std::make_shared<Machine<ZombieTraits>>(zombie));

    }
}

void ZomFrame::OnLoadSurvivor(wxCommandEvent& event)
{
    wxFileDialog dialog(this, _("Load Human"),"./zom","","ZOM Files|*.zom");
    if(dialog.ShowModal() == wxID_OK)
    {
        std::string fileName = dialog.GetPath().ToStdString();
        Machine<HumanTraits> human;
        human.LoadMachine(fileName);
        ZomWorld::get().AddHuman(std::make_shared<Machine<HumanTraits>>(human));
    }
}

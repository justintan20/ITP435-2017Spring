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
    ID_LOAD_SURVIVOR,
    ID_RESET,
    ID_RANDOMIZE
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame)
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SImSTART, ZomFrame::OnSimStart)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
    EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::OnLoadZom)
    EVT_MENU(ID_LOAD_SURVIVOR, ZomFrame::OnLoadSurvivor)
    EVT_MENU(ID_RESET, ZomFrame::OnReset)
    EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
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
    mSimMenu->Append(ID_LOAD_SURVIVOR, "Load Human");
    mSimMenu->Append(ID_RESET, "Reset");
    mSimMenu->Append(ID_RANDOMIZE, "Randomize");
    //disable these options in the beginning
    mSimMenu->Enable(ID_SImSTART, false);
    mSimMenu->Enable(ID_RESET, false);
    mSimMenu->Enable(ID_RANDOMIZE, false);
	
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
    //create the world
    ZomWorld::get().NewWorld();
    ZomWorld::get().SetMonth(0);
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
    //reset, delete loaded files and disable randomize menu
    OnReset(event);
    ZomWorld::get().SetHumanFilename("");
    ZomWorld::get().SetZombieFilename("");
    mSimMenu->Enable(ID_RANDOMIZE, false);
    mPanel->PaintNow();
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
    //disable randomize menu
    mSimMenu->Enable(ID_RANDOMIZE, false);
}

void ZomFrame::OnTurnTimer(wxTimerEvent& event)
{
    //update month
    int currMonth = ZomWorld::get().GetMonth();
    ZomWorld::get().SetMonth(currMonth + 1);
    //check if there are zombies
    if(ZomWorld::get().GetNumZombies() != 0)
    {
        //take a turn for each zombie
        for(int i = 0; i < ZomWorld::get().GetNumZombies(); i++)
        {
            try
            {
                ZomWorld::get().GetZombie(i)->TakeTurn(*ZomWorld::get().GetZombieState(i));
            }
            catch(std::exception e)
            {
                wxMessageBox(e.what());
            }
        }
    }
    //check if there are humans
    if(ZomWorld::get().GetNumHumans() != 0)
    {
        //take a turn for each human
        for(int i = 0; i < ZomWorld::get().GetNumHumans(); i++)
        {
            ZomWorld::get().GetHuman(i)->TakeTurn(*ZomWorld::get().GetHumanState(i));
        }
    }
    mPanel->PaintNow();
    //if no humans left
    if(ZomWorld::get().GetNumHumans() == 0)
    {
        //zombies win
        mTurnTimer->Stop();
        wxMessageBox("Zombies win!");
        mSimMenu->Enable(ID_SImSTART, false);
    }
    //if no zombies left
    else if(ZomWorld::get().GetNumZombies() == 0)
    {
        //humans win
        mTurnTimer->Stop();
        wxMessageBox("Humans win!");
        mSimMenu->Enable(ID_SImSTART, false);
    }
    mSimMenu->Enable(ID_RANDOMIZE, false);
}

void ZomFrame::OnLoadZom(wxCommandEvent& event)
{
    //initialize dialog
    wxFileDialog dialog(this, _("Load Zombie"),"./zom","","ZOM Files|*.zom");
    //get filename
    if(dialog.ShowModal() == wxID_OK)
    {
        ZomWorld::get().SetZombieFilename(dialog.GetPath().ToStdString());
        //if both zombies and humans are loaded, enable randomize option
        if((ZomWorld::get().GetZombieFileName().size() > 0)&&(ZomWorld::get().GetHumanFileName().size() > 0))
        {
            mSimMenu->Enable(ID_RANDOMIZE, true);
        }
    }
    mPanel->PaintNow();
}

//similar implementation to OnLoadZom
void ZomFrame::OnLoadSurvivor(wxCommandEvent& event)
{
    wxFileDialog dialog(this, _("Load Human"),"./zom","","ZOM Files|*.zom");
    if(dialog.ShowModal() == wxID_OK)
    {
        ZomWorld::get().SetHumanFilename(dialog.GetPath().ToStdString());
        if((ZomWorld::get().GetZombieFileName().size() > 0)&&(ZomWorld::get().GetHumanFileName().size() > 0))
        {
            mSimMenu->Enable(ID_RANDOMIZE, true);
        }
    }
    mPanel->PaintNow();
}

void ZomFrame::OnReset(wxCommandEvent& event)
{
    //set month to 0
    ZomWorld::get().SetMonth(0);
    //clear all zombies and humans
    ZomWorld::get().ClearAll();
    //restart timer
    mTurnTimer->Start();
    mTurnTimer->Stop();
    //disable these options
    mSimMenu->Enable(ID_RESET, false);
    mSimMenu->Enable(ID_SImSTART, false);
    mSimMenu->Enable(ID_RANDOMIZE, true);
    mPanel->PaintNow();
}

void ZomFrame::OnRandomize(wxCommandEvent& event)
{
    //create 20 zombies
    for(int i = 0; i < 20; i++)
    {
        Machine<ZombieTraits> zombie;
        try
        {
            zombie.LoadMachine(ZomWorld::get().GetZombieFileName());
            ZomWorld::get().AddZombie(std::make_shared<Machine<ZombieTraits>>(zombie));
        }
        catch (std::exception e)
        {
            wxMessageBox(e.what());
        }
    }
    //create 10 humans
    for(int i = 0; i < 10; i++)
    {
        Machine<HumanTraits> human;
        try
        {
        human.LoadMachine(ZomWorld::get().GetHumanFileName());
        ZomWorld::get().AddHuman(std::make_shared<Machine<HumanTraits>>(human));
        }
        catch(std::exception e)
        {
            wxMessageBox(e.what());
        }
    }
    //enable these options
    mSimMenu->Enable(ID_SImSTART, true);
    mSimMenu->Enable(ID_RESET, true);
    mSimMenu->Enable(ID_RANDOMIZE, false);
    mPanel->PaintNow();
}

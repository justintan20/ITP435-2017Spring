//
//  DNAFrame.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNAFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/busyinfo.h>
#include "DNADrawPanel.h"
#include "Exceptions.h"
#include "DNAAlignDlg.h"
#include "DNANeedWun.h"
#include "Timer.h"
#include <iostream>

enum
{
	ID_AMINO_HIST=1000,
    ID_ALIGNMENT=2000
};

wxBEGIN_EVENT_TABLE(DNAFrame, wxFrame)
	EVT_MENU(wxID_EXIT, DNAFrame::OnExit)
	EVT_MENU(wxID_NEW, DNAFrame::OnNew)
	EVT_MENU(ID_AMINO_HIST, DNAFrame::OnAminoHist)
    EVT_MENU(ID_ALIGNMENT, DNAFrame::OnAlignment)
wxEND_EVENT_TABLE()

DNAFrame::DNAFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(ID_AMINO_HIST, "Amino Acid Histogram...",
					 "Create a histogram from a FASTA file.");
    menuFile->Append(ID_ALIGNMENT, "Pairwise Alignment...");
	menuFile->Append(wxID_EXIT);
	
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new DNADrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}

void DNAFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void DNAFrame::OnNew(wxCommandEvent& event)
{
	// Doesn't do anything...
}

void DNAFrame::OnAminoHist(wxCommandEvent& event)
{
	// Implement (File>Amino Acid Histogram...)
    wxFileDialog dialog(this, _("Amino Acid Histogram"),"./data","","FASTA(*.fasta)|*.fasta");
    if(dialog.ShowModal() == wxID_OK)
    {
        std::string fileName = dialog.GetPath().ToStdString();
        //translate with regards to exception
        try
        {
            mTranslator = std::make_shared<DNATranslate>();
            mData = std::make_shared<DNAData>(fileName);
            mTranslator->SetHeader(mData->GetHeader());
            mTranslator->Translate(mData->GetSequence());
            mPanel->GetTranslated()->SetData(mTranslator);
            mPanel->PaintNow();
            
        }
        catch(FileLoadExcept)
        {
            wxMessageBox("FASTA file is invalid", "Error", wxOK | wxICON_ERROR);
        }
    }
}

void DNAFrame::OnAlignment(wxCommandEvent& event)
{
    //open dialog
    std::shared_ptr<DNAAlignDlg> dialog = std::make_shared<DNAAlignDlg>();
    if(dialog->ShowModal() == wxID_OK)
    {
        //do alignment for two given files
        std::string fileName1 = dialog->GetInputAPath();
        std::string fileName2 = dialog->GetInputBPath();
        std::shared_ptr<DNANeedWun> alignment = std::make_shared<DNANeedWun>(fileName1,fileName2);
        //keep track of time
        Timer time;
        time.Start();
        alignment->Run();
        double seconds = time.GetElapsed();
        std::cout << "Time used: " << seconds << std::endl;
        //output results to file
        alignment->WriteResults(dialog->GetOutputPath());
    }
}

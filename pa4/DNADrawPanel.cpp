//
//  DNADrawPanel.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNADrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <array>
#include <sstream>
#include <iomanip>
#include <string>

BEGIN_EVENT_TABLE(DNADrawPanel, wxPanel)
	EVT_PAINT(DNADrawPanel::PaintEvent)
END_EVENT_TABLE()

DNADrawPanel::DNADrawPanel(wxFrame* parent)
: wxPanel(parent)
{
    mTranslated = std::make_shared<DNATranslate>();
}

void DNADrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void DNADrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void DNADrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	// Draw histogram, if one has been generated
    if(mTranslated->GetTotal() != 0){
        //set initial colots
        dc.SetPen(*wxBLACK_PEN);
        dc.SetTextBackground(*wxBLACK);
        dc.DrawText(mTranslated->GetHeader(), 0, 0);
        //draw histogram for each amino acid
        DrawAcid(dc, "Alanine", mTranslated->GetA(), *wxRED, *wxRED_BRUSH, 50);
        DrawAcid(dc, "Cysteine", mTranslated->GetC(), *wxBLUE, *wxBLUE_BRUSH, 80);
        DrawAcid(dc, "Aspartic Acid", mTranslated->GetD(), *wxGREEN, *wxGREEN_BRUSH, 110);
        DrawAcid(dc, "Glutamic Acid", mTranslated->GetE(), *wxRED, *wxRED_BRUSH, 140);
        DrawAcid(dc, "Phenylalanine", mTranslated->GetF(), *wxBLUE, *wxBLUE_BRUSH, 170);
        DrawAcid(dc, "Glycine", mTranslated->GetG(), *wxGREEN, *wxGREEN_BRUSH, 200);
        DrawAcid(dc, "Histidine", mTranslated->GetH(), *wxRED, *wxRED_BRUSH, 230);
        DrawAcid(dc, "Isoleucine", mTranslated->GetI(), *wxBLUE, *wxBLUE_BRUSH, 260);
        DrawAcid(dc, "Lysine", mTranslated->GetK(), *wxGREEN, *wxGREEN_BRUSH, 290);
        DrawAcid(dc, "Leucine", mTranslated->GetL(), *wxRED, *wxRED_BRUSH, 320);
        DrawAcid(dc, "Methionine", mTranslated->GetM(), *wxBLUE, *wxBLUE_BRUSH, 350);
        DrawAcid(dc, "Asparagine", mTranslated->GetN(), *wxGREEN, *wxGREEN_BRUSH, 380);
        DrawAcid(dc, "Proline", mTranslated->GetP(), *wxRED, *wxRED_BRUSH, 410);
        DrawAcid(dc, "Glutamine", mTranslated->GetQ(), *wxBLUE, *wxBLUE_BRUSH, 440);
        DrawAcid(dc, "Arginine", mTranslated->GetR(), *wxGREEN, *wxGREEN_BRUSH, 470);
        DrawAcid(dc, "Serine", mTranslated->GetS(), *wxRED, *wxRED_BRUSH, 500);
        DrawAcid(dc, "Threonine", mTranslated->GetT(), *wxBLUE, *wxBLUE_BRUSH, 530);
        DrawAcid(dc, "Valine", mTranslated->GetV(), *wxGREEN, *wxGREEN_BRUSH, 560);
        DrawAcid(dc, "Tryptophan", mTranslated->GetW(), *wxRED, *wxRED_BRUSH, 590);
        DrawAcid(dc, "Tyrosine", mTranslated->GetY(), *wxBLUE, *wxBLUE_BRUSH, 620);
        
    }
}

void DNADrawPanel::DrawAcid(wxDC& dc, const std::string& name, int num, const wxColour& color, const wxBrush& brush, int y) const
{
    //set given colors
    dc.SetTextForeground(color);
    dc.SetBrush(brush);
    //calculate percentage
    double percent = static_cast<double>(num) / static_cast<double>(mTranslated->GetTotal()) * 100;
    std::stringstream output;
    int precision = 3;
    if(percent >= 10)
    {
        precision = 4;
    }
    //output info
    output << name << " " << std::setprecision(3) << percent << "% (" << num << ")";
    dc.DrawText(output.str(), 0, y);
    //draw histogram
    dc.DrawRectangle(200, y, percent*45, 30);
    
}

std::shared_ptr<DNATranslate> DNADrawPanel::GetTranslated() const
{
    return mTranslated;
}

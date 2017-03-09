//
//  DNADrawPanel.h
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <string>
#include <memory>
#include "DNATranslate.h"

class DNADrawPanel : public wxPanel
{
public:
	DNADrawPanel(wxFrame* parent);
	void PaintNow();
    std::shared_ptr<DNATranslate> GetTranslated() const;

protected:
	void PaintEvent(wxPaintEvent & evt);
	void Render(wxDC& dc);
    //helper for Render
    void DrawAcid(wxDC& dc, const std::string& name, int num, const wxColour& color, const wxBrush& brush, int y) const;
 
	DECLARE_EVENT_TABLE()
	
public:
	// Variables here
    std::shared_ptr<DNATranslate> mTranslated;
};


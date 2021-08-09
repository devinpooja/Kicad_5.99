/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2010-2014 Jean-Pierre Charras  jp.charras at wanadoo.fr
 * Copyright (C) 1992-2018 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gerbview.h>
#include <gerbview_frame.h>
#include <core/arraydim.h>

#include "panel_gerbview_settings.h"


/// List of page sizes
static const wxChar* gerberPageSizeList[] =
{
    wxT( "GERBER" ),    // index 0: full size page selection
    wxT( "A4" ),
    wxT( "A3" ),
    wxT( "A2" ),
    wxT( "A" ),
    wxT( "B" ),
    wxT( "C" ),
};


PANEL_GERBVIEW_SETTINGS::PANEL_GERBVIEW_SETTINGS( GERBVIEW_FRAME *aFrame, wxWindow* aWindow ) :
        PANEL_GERBVIEW_SETTINGS_BASE( aWindow, wxID_ANY ),
        m_Parent( aFrame )
{
}


bool PANEL_GERBVIEW_SETTINGS::TransferDataToWindow( )
{
    m_ShowPageLimitsOpt->SetValue( m_Parent->GetDisplayOptions().m_DisplayPageLimits );

    for( unsigned i = 0;  i < arrayDim( gerberPageSizeList );  ++i )
    {
        if( gerberPageSizeList[i] == m_Parent->GetPageSettings().GetType() )
        {
            m_PageSize->SetSelection( i );
            break;
        }
    }

    return true;
}


bool PANEL_GERBVIEW_SETTINGS::TransferDataFromWindow()
{
    auto opts = m_Parent->GetDisplayOptions();
    opts.m_DisplayPageLimits = m_ShowPageLimitsOpt->GetValue();

    PAGE_INFO pageInfo( gerberPageSizeList[ m_PageSize->GetSelection() ] );
    m_Parent->SetPageSettings( pageInfo );

    m_Parent->UpdateDisplayOptions( opts );

    return true;
}


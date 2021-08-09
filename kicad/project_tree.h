/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2010-2014 Jean-Pierre Charras
 * Copyright (C) 2004-2014 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef PROJECT_TREE_H
#define PROJECT_TREE_H


#include <wx/treectrl.h>

#include "tree_file_type.h"

class PROJECT_TREE_PANE;

/** PROJECT_TREE
 * This is the class to show (as a tree) the files in the project directory
 */
class PROJECT_TREE : public wxTreeCtrl
{
    DECLARE_DYNAMIC_CLASS( PROJECT_TREE )

private:
    PROJECT_TREE_PANE* m_projectTreePane;
    wxImageList*       m_imageList;

public:
    PROJECT_TREE_PANE* GetProjectTreePane() const { return m_projectTreePane; }

    PROJECT_TREE( PROJECT_TREE_PANE* parent );
    ~PROJECT_TREE();

private:
    /* overridden sort function */
    int OnCompareItems( const wxTreeItemId& item1, const wxTreeItemId& item2 ) override;
};

#endif  // PROJECT_TREE_H

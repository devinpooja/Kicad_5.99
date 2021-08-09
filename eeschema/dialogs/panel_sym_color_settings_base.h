///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PANEL_SYM_COLOR_SETTINGS_BASE
///////////////////////////////////////////////////////////////////////////////
class PANEL_SYM_COLOR_SETTINGS_BASE : public wxPanel
{
	DECLARE_EVENT_TABLE()
	private:

		// Private event handlers
		void _wxFB_OnUseEeschemaThemeChanged( wxCommandEvent& event ){ OnUseEeschemaThemeChanged( event ); }


	protected:
		wxCheckBox* m_useEeschemaTheme;
		wxStaticText* m_txtTheme;
		wxChoice* m_themeSelection;

		// Virtual event handlers, overide them in your derived class
		virtual void OnUseEeschemaThemeChanged( wxCommandEvent& event ) { event.Skip(); }


	public:

		PANEL_SYM_COLOR_SETTINGS_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~PANEL_SYM_COLOR_SETTINGS_BASE();

};


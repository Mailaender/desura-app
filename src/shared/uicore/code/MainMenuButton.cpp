/*
Desura is the leading indie game distribution platform
Copyright (C) 2011 Mark Chandler (Desura Net Pty Ltd)

$LicenseInfo:firstyear=2014&license=lgpl$
Copyright (C) 2014, Linden Research, Inc.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation;
version 2.1 of the License only.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, see <http://www.gnu.org/licenses/>
or write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
$/LicenseInfo$
*/

#include "Common.h"
#include "MainMenuButton.h"
#include <wx/msgdlg.h>
#include <branding/branding.h>



MainMenuButton::MainMenuButton(wxWindow* parent, bool offline) : gcImageButton(parent, wxID_ANY, wxDefaultPosition, wxSize(30, 66))//, gcCustomMenu()
{
	Bind(wxEVT_KILL_FOCUS, &MainMenuButton::onBlur, this);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainMenuButton::onMouseClick, this);

	SetBackgroundColour( wxColour(128,128,0) );
	SetSize(wxSize( 30,66 ));

	setDefaultImage("#menu_button");
	setHoverImage("#menu_button_hover");
	
	m_mainMenu = nullptr;
	createMenu(offline);

	m_bIgnoreNextClick = false;
}

MainMenuButton::~MainMenuButton()
{
	if (HasCapture())
		ReleaseMouse();

	safe_delete(m_mainMenu);
}

void MainMenuButton::onActiveToggle(bool state)
{
	if (state)
	{
		setDefaultImage("#menu_button");
	}
	else
	{
		setDefaultImage("#menu_button_nonactive");
	}
}



void MainMenuButton::onBlur(wxFocusEvent& event)
{
#ifdef WIN32
	EndMenu();
#endif
}

void MainMenuButton::onMouseClick(wxCommandEvent& event)
{
	if (m_bIgnoreNextClick == false)
	{
		PopupMenu(m_mainMenu, 0, GetRect().GetHeight());
	}
	else
	{
		m_bIgnoreNextClick = false;	
	}
}

void MainMenuButton::createMenu(bool offline)
{
	if (m_mainMenu)
		return;

	m_mainMenu = new gcMenu();

	gcMenu* m_toolMenu = new gcMenu();
	gcMenu* m_historyMenu = new gcMenu();

	m_toolMenu->Append(DESURA_wxActivateGame, Managers::GetString(L"#MENU_ACTIVATEGAME"), Managers::GetString(L"#MENU_ACTIVATEGAME_TOOLTIP"));
#ifdef WIN32
	m_toolMenu->Append(DESURA_wxModWizard, Managers::GetString(L"#MENU_WIZZARD"), Managers::GetString(L"#MENU_WIZZARD_TOOLTIP"));
#endif
	m_toolMenu->Append(DESURA_wxCustomInstallPath, Managers::GetString(L"#MENU_INSTALLPATH"), Managers::GetString(L"#MENU_INSTALLPATH_TOOLTIP"));
	m_toolMenu->Append(DESURA_wxListKeys, Managers::GetString(L"#MENU_LISTKEYS"), Managers::GetString(L"#MENU_LISTKEYS_TOOLTIP"));

	m_historyMenu->Append(DESURA_wxGifts, Managers::GetString(L"#MENU_GIFTS"), Managers::GetString(L"#MENU_GIFTS_TOOLTIP"));
	m_historyMenu->Append(DESURA_wxPurchase, Managers::GetString(L"#MENU_PURCHASE"), Managers::GetString(L"#MENU_PURCHASE_TOOLTIP"));
	m_historyMenu->Append(DESURA_wxCart, Managers::GetString(L"#MENU_CART"), Managers::GetString(L"#MENU_CART_TOOLTIP"));

#ifdef DESURA_OFFICIAL_BUILD
	m_historyMenu->Append(DESURA_wxChangeLog, Managers::GetString(L"#MENU_CHANGELOG"), Managers::GetString(L"#MENU_CHANGELOG_TOOLTIP"));
#endif

	m_mainMenu->Append(DESURA_wxOffline, Managers::GetString(L"#MENU_OFFLINE"), Managers::GetString(L"#MENU_OFFLINE_TOOLTIP"));
	m_mainMenu->Append(DESURA_wxLogOut, Managers::GetString(L"#MENU_LOGOUT"), Managers::GetString(L"#MENU_LOGOUT_TOOLTIP"));
	m_mainMenu->AppendSeparator();

	m_mainMenu->Append(DESURA_wxAbout, Managers::GetString(L"#MENU_ABOUT"));
	m_mainMenu->Append(DESURA_wxHelp, Managers::GetString(L"#MENU_HELP"));
	m_mainMenu->AppendSeparator();

	m_mainMenu->Append(DESURA_wxLog, Managers::GetString(L"#MENU_CONSOLE"));
	m_mainMenu->Append(DESURA_wxSettings, Managers::GetString(L"#MENU_SETTINGS"), Managers::GetString(L"#MENU_SETTINGS_TOOLTIP"));
	m_mainMenu->Append(-1, Managers::GetString(L"#MENU_HISTORY"), m_historyMenu);
	m_mainMenu->Append(-1, Managers::GetString(L"#MENU_TOOLS"), m_toolMenu);
	m_mainMenu->AppendSeparator();

#if defined(DEBUG) && defined(WITH_GTEST)
	m_mainMenu->Append(DESURA_wxUnitTest, Managers::GetString(L"#MENU_UNITTEST"));
	m_mainMenu->AppendSeparator();
#endif

	m_mainMenu->Append(DESURA_wxExit, Managers::GetString(L"#MENU_EXIT"), Managers::GetString(L"#MENU_EXIT_TOOLTIP"));

	if (offline)
	{
		m_toolMenu->Enable(DESURA_wxActivateGame, false);
#ifdef WIN32
		m_toolMenu->Enable(DESURA_wxModWizard, false);
#endif
		m_toolMenu->Enable(DESURA_wxCustomInstallPath, false);
		m_toolMenu->Enable(DESURA_wxListKeys, false);

		m_historyMenu->Enable(DESURA_wxGifts, false);
		m_historyMenu->Enable(DESURA_wxPurchase, false);
		m_historyMenu->Enable(DESURA_wxCart, false);
#ifdef DESURA_OFFICIAL_BUILD
		m_historyMenu->Enable(DESURA_wxChangeLog, false);
#endif

		m_mainMenu->Enable(DESURA_wxHelp, false);
		m_mainMenu->Enable(DESURA_wxLogOut, false);
		m_mainMenu->Enable(DESURA_wxSettings, false);

		m_mainMenu->SetLabel(DESURA_wxOffline, Managers::GetString(L"#MENU_ONLINE"));
	}

	m_mainMenu->layoutMenu();
}

#ifdef WIN32
WXLRESULT MainMenuButton::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
	if (WM_MENUSELECT == message)
	{
		if (HIWORD(wParam) == 0xFFFF && lParam == 0)
		{
			wxPoint pos = wxGetMousePosition()-GetScreenPosition();
			wxRect rect = GetRect();
			m_bIgnoreNextClick = rect.Contains(pos);
		}
	}

	return gcImageButton::MSWWindowProc(message, wParam, lParam);
}
#endif

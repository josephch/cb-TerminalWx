/***************************************************************
 * Name:      cbTerminalView
 * Purpose:   This class implements the panel that is added to
 *            Code::Blocks Message notebook or C::B layout.
 * Author:    Jerome ANTOINE
 * Created:   2007-10-08
 * Copyright: Jerome ANTOINE
 * Copyright: Christo Joseph
 * License:   GPL
 **************************************************************/

#include <wx/bitmap.h>
#include <wx/bmpbuttn.h>
#include <wx/display.h>
#include <wx/statline.h>
#ifndef CB_PRECOMP
    #include <wx/combobox.h>
    #include <wx/menu.h>
    #include <wx/sizer.h>
    #include <wx/splitter.h>
    #include <wx/statbox.h>
    #include <wx/stattext.h>
    #include <wx/settings.h>
    #include <wx/toolbar.h>
#endif

#include "cbTerminal.hpp"
#include "cbTerminalView.hpp"
#include <wx/nativewin.h>
#include <vte/vte.h>

cbTerminalView::cbTerminalView(cbTerminal& terminalPlugin) :
    wxPanel(Manager::Get()->GetAppWindow()),
    m_cbTerminalPlugin(terminalPlugin)
{
    VteTerminal* vteTerminal = VTE_TERMINAL(vte_terminal_new());
    if (!vteTerminal)
    {
        fprintf(stderr, "Could not create VTE terminal\n");
        return;
    }
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    char* bash_args[] = { "/bin/bash", NULL }; //NOLINT
#if 0 //FIXME for async, codeblocks hangs on startup.
    vte_terminal_spawn_async(m_vte, VTE_PTY_DEFAULT, NULL, bash_args, NULL,
                             (GSpawnFlags)0, NULL, NULL, NULL, -1, NULL, NULL, NULL);
#else
     vte_terminal_spawn_sync(vteTerminal, VTE_PTY_DEFAULT, NULL, bash_args, NULL,  //NOLINT
                             (GSpawnFlags)0, NULL, NULL, NULL, NULL, NULL );
#endif
    wxNativeWindow* nativeWindow = new wxNativeWindow(this, wxID_ANY, GTK_WIDGET(vteTerminal));
    sizer->Add(nativeWindow, 1, wxEXPAND);
    SetSizer(sizer);
}

cbTerminalView::~cbTerminalView() = default;

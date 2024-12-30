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

#include "TerminalWx/src/GTerm/pseudo_unix98.hpp"
#include "TerminalWx/src/terminalwx.h"
#include <atomic>
#include <thread>
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

struct cbTerminalWx : public TerminalWx
{
    cbTerminalWx(wxWindow* parent, wxWindowID id) : TerminalWx(parent, id)
    {
        struct timeval tv;
    //	struct sigaction child_action;
        /* spawn shell in pseudo terminal */
        if(!p.init() || !p.spawn("/bin/sh")) {
                fprintf(stderr, 
                    "Error: Could not open pty or could not spawn shell.\n");
        }
        master_fd = p.get_master();

        /* we want non-blocking reads from pty output */
        fcntl(master_fd, F_SETFL, O_NONBLOCK);
        m_pollThread = std::thread(&cbTerminalWx::pollData, this);
    }
    ~cbTerminalWx()
    {
        m_quit = true;
        m_pollThread.join();
    }
    void pollData()
    {
        fd_set rset;
        unsigned char buf[1000];
        while (!m_quit)
        {
            int retval = 0,xfd, high, i, ret, need_timeout = 1;
            FD_ZERO(&rset);
            FD_SET(master_fd, &rset);     high = master_fd;
            ret = select(high+1, &rset, NULL, NULL, NULL);
            if (FD_ISSET(master_fd, &rset)) {
                i = read(master_fd, buf, 1000);
                if (i>0){
                        DisplayChars(wxString(buf, i));
                }
            }
        }
    }
    void OnUserInput(wxString str) override
    {
        wxScopedCharBuffer buf = str.ToAscii();
        write(master_fd, buf.data(), buf.length());
    }
    int master_fd;
    PseudoTerminal p;
    std::thread m_pollThread;
    std::atomic_bool m_quit{false};
};

cbTerminalView::cbTerminalView(cbTerminal& terminalPlugin) :
    wxPanel(Manager::Get()->GetAppWindow()),
    m_cbTerminalPlugin(terminalPlugin)
{
    cbTerminalWx* Term1 = new cbTerminalWx(this, wxID_ANY);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    char* bash_args[] = { "/bin/bash", NULL }; //NOLINT
    sizer->Add(Term1, 1, wxEXPAND);
    SetSizer(sizer);
	
}
#if 0
void cbTerminalView::OnCopy(wxCommandEvent& event) {
    if (Terminal[portnum]->HasSelection()) {

        wxString Selection = Terminal[portnum]->GetSelection();
        if (wxTheClipboard->Open())
        {
            wxTheClipboard->SetData( new wxTextDataObject(Selection) );
            wxTheClipboard->Flush();
            wxTheClipboard->Close();
        }
    }

    event.Skip();
}

void cbTerminalView::OnSelectAll(wxCommandEvent& WXUNUSED(event)) { Terminal[portnum]->SelectAll(); }

#endif

cbTerminalView::~cbTerminalView() = default;

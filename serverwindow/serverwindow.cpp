#include <functional>
#include <thread>
#include <queue>

#include "serverwindow.h"
#include "serverwindow.events.cpp"

ServerWindow::ServerWindow(wxString title, std::string& hostname, int port)
	: wxFrame(NULL, wxID_ANY, title), server{ }
{
	// GUI Building
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_Details, "Server &Details...\tCtrl+D", "Server Connection Details");

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");

	SetMenuBar(menuBar);

	// Log panel
	wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);
	logContainer = new wxBoxSizer(wxVERTICAL);

	logScroller = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	logScroller->SetSizer(logContainer);

	logContainer->Fit(logScroller);
	contentSizer->Add(logScroller, 1, wxEXPAND | wxALL, 5);

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetSizer(contentSizer);
	this->Centre(wxBOTH);

	// Window event bindings
	Bind(wxEVT_CLOSE_WINDOW, &ServerWindow::OnClose, this);
	Bind(wxEVT_MENU, &ServerWindow::OnDetails, this, ID_Details);

	// Start server
	if (!StartServerThread(hostname, port)) {
		wxLogFatalError("Failed to start server");
	}
}

bool ServerWindow::StartServerThread(std::string& hostname, int port) {
	// create a thread for the server to run on
	if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
		return false; // failed to create thread
	}

	// We have a thread, lets make sure we have a valid Server instance for it to use
	server = std::make_unique<Server>();
	if (!server->BindAndListen(hostname, port)) {
		return false; // unable to bind or listen (could be port is already bound)
	}

	// All preconditions satisfied, SMASH THAT GO BUTTON!
	if (GetThread()->Run() != wxTHREAD_NO_ERROR) {
		return false; // button smashed too hard, thread failed to start
	}

	return true;
}
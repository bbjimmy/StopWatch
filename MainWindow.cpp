#include "MainWindow.h"
#include <Application.h>
#include <Button.h>
#include <StringView.h>
#include <View.h>
#include <String.h>
#include <be/kernel/OS.h>
#include <unistd.h>

bool clockStart = false;
bool clockReset = false;
int totalCycles = 0;
int totalSeconds = 0;
int totalMinutes = 0;
int totalHours = 0;
BString displayClock;
BString allZeros = "00:00:00.0";
BStringView *clockStringView;

enum
{
	M_BUTTON_START = 'btn1',
	M_BUTTON_STOP = 'btn2',
	M_BUTTON_RESET = 'btn3',
	M_UPDATE_CLOCK = 'uclk'
};


MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,324,200), "StopWatch", B_TITLED_WINDOW,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE |
				B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	BView *mainView = new BView(Bounds(), "mainView", B_FOLLOW_ALL, B_WILL_DRAW);
	
	clockStringView = new BStringView(BRect(10,10,254,55), "clockStringView", NULL);
	
	BButton *startButton = new BButton(BRect(9,65,87,90), "startButton", "Start",
								   	   new BMessage(M_BUTTON_START));
								  
	BButton *stopButton = new BButton(BRect(93,65,171,90), "stopButton", "Stop",
								   	  new BMessage(M_BUTTON_STOP));
								   	  
	BButton *resetButton = new BButton(BRect(177,65,255,90), "resetButton", "Reset",
								   	   new BMessage(M_BUTTON_RESET));
								   	   
	BFont font(be_plain_font);
    font.SetSize(40.0);
    font.SetFamilyAndStyle("DejaVu Sans Mono", "Book");
	
	
	AddChild(mainView);
	mainView->AddChild(clockStringView);
	mainView->AddChild(startButton);
	mainView->AddChild(stopButton);
	mainView->AddChild(resetButton);
	
	mainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	clockStringView->SetFont(&font);
	clockStringView->SetAlignment(B_ALIGN_CENTER);
	clockStringView->SetText(allZeros.String());
	clockStringView->SetViewColor(0,0,0);
	clockStringView->SetHighColor(255,255,255);
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	
						
	switch (msg->what)
	{
		case M_BUTTON_START:
		{
			clockStart = true;
			clockReset = false;
			
			TickTock();
			break;	
		}
		case M_BUTTON_STOP:
		{
			clockStart = false;
			clockReset = false;
			
			break;
		}
		case M_BUTTON_RESET:
		{
			clockStart = false;
			clockReset = true;
			
			UpdateClock();
			totalCycles = 0;
			totalSeconds = 0;
			totalMinutes = 0;
			totalHours = 0;
			break;
		}
		case M_UPDATE_CLOCK:
		{
			clockStringView->SetText(displayClock.String());
			if(clockReset) { clockStringView->SetText(allZeros.String()); }
			else { TickTock(); }
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


int
MainWindow::UpdateClock(void)
{
	PostMessage(M_UPDATE_CLOCK);
	usleep(9750);
	return(0);	
}


int
MainWindow::TickTock(void)
{
	while(clockStart) {
		int H1, H2, M1, M2, S1, S2, TS;
		H1 = ((totalHours % 60) / 10);
		H2 = ((totalHours % 60) % 10);
		M1 = ((totalMinutes % 60) / 10);
		M2 = ((totalMinutes % 60) % 10);
		S1 = ((totalSeconds % 60) / 10);
		S2 = ((totalSeconds % 60) % 10);
		TS = ((totalCycles / 10) % 10);
		displayClock = "";
		displayClock << H1 << H2 << ":" << M1 << M2 << ":" << S1 << S2 << "." << TS;
		totalCycles += 1;
		totalSeconds = totalCycles / 100;
		totalMinutes = totalSeconds / 60;
		totalHours = totalMinutes / 60;
		if(totalHours == 24) {
			totalCycles = 0;
			totalSeconds = 0;
			totalMinutes = 0;
			totalHours = 0;
		}
		UpdateClock();
		break;
	}
	return(0);
}

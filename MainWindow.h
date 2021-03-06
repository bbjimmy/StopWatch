#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>

class MainWindow : public BWindow
{
public:
	MainWindow(void);
	void MessageReceived(BMessage *msg);
	bool QuitRequested(void);

private:
	int TickTock(void);
	int UpdateClock(void);
};

#endif
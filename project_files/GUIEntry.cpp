#include "GUIEntry.h"
#include "MainFrame.h"

using namespace std;

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480

// entry function, just like main() in regular c++
bool GUIEntry::OnInit() {
    // required to make sure rand has a new seed every time the app starts to not have the same generated numbers when app restarts
    srand(time(NULL));
    // we dont have to deallocate these pointers as wxwidgets handles them for you
    // the main view of the app, everything that is shown will be within this frame
    MainFrame* mainFrame = new MainFrame();
    mainFrame->SetClientSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    mainFrame->Center(); // the window will be in the middle of user's screen
    mainFrame->Show();
    
    return true;
}

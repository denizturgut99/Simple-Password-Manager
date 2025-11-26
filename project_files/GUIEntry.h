#include <wx/wx.h>
/*
* this class doesn't require anything other than the OnInit() function which is provided by wxwidgets
* OnInit is basically main() in regular c++
* the entry point of the app
*/
class GUIEntry : public wxApp {
	public:
		bool OnInit();
};


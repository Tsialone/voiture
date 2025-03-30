#include <wx/wx.h>
#include "MainFrame.hpp"
#include <iostream>
#include <string>
#include "Voiture.hpp"
using namespace std;
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    try
    {
        UtilDb::init("database/voiture.sqlite3");
        Voiture * kia = new Voiture(1, "Kia", 500.0, 70, 20, 80.0, 0.0042);

        MainFrame *frame = new MainFrame("Gestion de Voitures"  , kia);
        frame->Show(true);
        UtilDb::closeCon();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return true;
}
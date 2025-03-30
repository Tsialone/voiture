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
        // Voiture *kia = new Voiture(2, "Kia", 500.0, 10, 20, 80.0, 1);
        Voiture *kia = new Voiture();
        vector<Voiture> all_car = kia->getAll();
        vector<Voiture *> all_ptr_car;
        for (Voiture v : all_car)
        {
            Voiture *ptr = new Voiture(v.getId(), v.getModel(), v.getVitesseMaximal(), v.getCapaciteAccelere(), v.getCapaciteFreinage(), v.getCarburantMaximal(), v.getConsommation());
            all_ptr_car.push_back(ptr);
        }

        
        
        MainFrame *frame = new MainFrame("Gestion de Voitures", all_ptr_car.front() , all_ptr_car);
        frame->setAllPtrCar(all_ptr_car);
        frame->Show(true);
        // UtilDb::closeCon();
        // cout << list.capacity() << endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return true;
}
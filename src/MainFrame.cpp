#include "MainFrame.hpp"
#include "Voiture.hpp"
#include "DashBoard.hpp"
#include <sstream>
#include "Fonction.hpp"
#include <iomanip>
#include <ctime>
#include <tuple>
#include <chrono>
#include "Evenement.hpp"
#include <set>
#include <wx/wx.h>
using namespace std;

MainFrame::MainFrame(const wxString &title, Voiture *voiture)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1900, 900))
{
    this->setVoiture(voiture);
    this->SetAutoLayout(false);
    wxPanel *toto = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(460, 600));
    toto->SetBackgroundColour(*wxYELLOW);
    toto->Enable(false);
    dashboard = new DashBoard(this, voiture);
    dashboard->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyDown, this);
    dashboard->Bind(wxEVT_KEY_UP, &MainFrame::OnKeyUp, this);
}

void MainFrame::setVoiture(Voiture *voiture)
{
    this->voiture = voiture;
}
Voiture *MainFrame::getVoiture()
{
    return this->voiture;
}

void MainFrame::OnKeyDown(wxKeyEvent &event)
{
    
    wxTimer *timer = dashboard->getTimer();
    int keyCode = event.GetKeyCode();
    double intensite = 0.0;
    double current = 0;
    Evenement *last_event = nullptr;
    if (voiture->getEventTuplet().capacity() > 0)
    {
        last_event = &voiture->getEventTuplet().back();
    }
    double   last = -1;
    if (last_event != nullptr) {
        last = last_event->getGamma();
    }

   

    // cout << "nombre d'evenement " << voiture->getEventTuplet().capacity() << endl;

    // full
    if (keyCode == 'F' && wxGetKeyState(WXK_UP))
    {
        // cout << "acceleration full" << endl;
        intensite = 1;
        current = voiture->getCapaciteAccelere() * intensite;
        // cout << "current "  << current << " last " << last << endl;
        if (current != last) {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning() )
        {
            voiture->setGamma(voiture->getCapaciteAccelere() * intensite);
            voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
            Evenement last_event = voiture->getEventTuplet().back();
            timer->Start(100);
        }
    }
    else if (keyCode == '1' && wxGetKeyState(WXK_UP))
    {
        // cout << "acceleration 1" << endl;
        intensite = 10.0 / 100.0;
        current = voiture->getCapaciteAccelere() * intensite;
        if (current != last) {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning())
        {
            voiture->setGamma(voiture->getCapaciteAccelere() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());
            voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
            Evenement last_event = voiture->getEventTuplet().back();
            timer->Start(100);
        }
    }
    else if (keyCode == '2' && wxGetKeyState(WXK_UP))
    {
        // cout << "acceleration 2" << endl;
        intensite = 20.0 / 100.0;
        current = voiture->getCapaciteAccelere() * intensite;
        if (current != last) {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning())
        {
            voiture->setGamma(voiture->getCapaciteAccelere() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());
            voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
            Evenement last_event = voiture->getEventTuplet().back();
            timer->Start(100);
        }
    }
    if (keyCode == 'F' && wxGetKeyState(WXK_DOWN))
    {
        // cout << "freinage full" << endl;
        intensite = 1;
        current = voiture->getCapaciteFreinage() * intensite;
        if (current != last) {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning())
        {
            voiture->setGamma(-voiture->getCapaciteFreinage() * intensite);
            voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
            Evenement last_event = voiture->getEventTuplet().back();
            timer->Start(100);
        }
    }
    else if (keyCode == '1' && wxGetKeyState(WXK_DOWN))
    {
        // cout << "freinage 1" << endl;
        intensite = 10.0 / 100.0;
        current = voiture->getCapaciteFreinage() * intensite;
        if (current != last) {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning())
        {
            voiture->setGamma(-voiture->getCapaciteFreinage() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());
            voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
            Evenement last_event = voiture->getEventTuplet().back();
            timer->Start(100);
        }
    }
    else if (keyCode == '2' && wxGetKeyState(WXK_DOWN))
    {
        // cout << "freinage 2" << endl;
        intensite = 20.0 / 100.0;
        current = voiture->getCapaciteFreinage() * intensite;
        if (current != last) {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning())
        {
            voiture->setGamma(-voiture->getCapaciteFreinage() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());
            voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
            Evenement last_event = voiture->getEventTuplet().back();
            timer->Start(100);
        }
    }

    event.Skip();
}
void MainFrame::OnKeyUp(wxKeyEvent &event)
{
    int keyCode = event.GetKeyCode();
    wxTimer *timer = dashboard->getTimer();
    timer->Stop();
    event.Skip();
}

// void MainFrame::OnKeyEvent(wxKeyEvent &event)
// {

//     auto now = std::chrono::system_clock::now();
//     std::time_t now_time = std::chrono::system_clock::to_time_t(now);

//     std::tm local_time = *std::localtime(&now_time);
//     // std::cout << "Heure actuelle: "
//     //           << std::setfill('0') << std::setw(2) << local_time.tm_hour << ":"
//     //           << std::setfill('0') << std::setw(2) << local_time.tm_min << ":"
//     //           << std::setfill('0') << std::setw(2) << local_time.tm_sec
//     //           << std::endl;

//     int keyCode = event.GetKeyCode();
//     const wxEventType eventType = event.GetEventType();
//     double current = 0;
//     double last = -1;
//     double intesite = 0.0;
//     wxTimer *timer = dashboard->getTimer();

//     voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
//     Evenement last_event = voiture->getEventTuplet().back();

//     // if (keyCode == '1' && wxGetKeyState(WXK_DOWN))
//     // {
//         // intesite = 10.0/100.0;
//         // voiture->setGamma(-1 * voiture->getCapaciteFreinage() * intesite);
//         // current = voiture->getGamma();
//         // last = last_event.getGamma();
//         // if (current != last)
//         // {
//         //     voiture->setVitesseInitial(voiture->getVitesse());
//         //     dashboard->t = 0;
//         //     timer->Start(100);
//         // }
//         // cout << "Freinage activé (Down+1) " <<  voiture->getGamma()  << endl;
//     // }
//     //    else  if (keyCode == '2' && wxGetKeyState(WXK_DOWN))
//     //     {
//     //         intesite = 20.0/100.0;
//     //         voiture->setGamma(-1 * voiture->getCapaciteFreinage() * intesite);
//     //         current = voiture->getGamma();
//     //         last = last_event.getGamma();
//     //         if (current != last)
//     //         {
//     //             voiture->setVitesseInitial(voiture->getVitesse());
//     //             dashboard->t = 0;
//     //             timer->Start(100);
//     //         }
//     //         cout << "Freinage activé (Down+2) " <<    voiture->getGamma()  << endl;
//     //     }
//     //     else  if (keyCode == 'F' && wxGetKeyState(WXK_DOWN))
//     //     {
//     //         intesite = 1;
//     //         voiture->setGamma(-1 * voiture->getCapaciteFreinage() * intesite);
//     //         current = voiture->getGamma();
//     //         last = last_event.getGamma();
//     //         if (current != last)
//     //         {
//     //             voiture->setVitesseInitial(voiture->getVitesse());
//     //             dashboard->t = 0;
//     //             timer->Start(100);
//     //         }
//     //         cout << "Freinage activé (Down+2 )" <<    voiture->getGamma()  << endl;
//     //     }
//     //     else if (keyCode == '1' && wxGetKeyState(WXK_UP)) {
//     //         intesite = 10.0/100.0;
//     //         voiture->setGamma(voiture->getCapaciteFreinage()  * intesite);
//     //         current = voiture->getGamma();
//     //         last = last_event.getGamma();
//     //         if (current != last)
//     //         {
//     //             voiture->setVitesseInitial(voiture->getVitesse());
//     //             dashboard->t = 0;
//     //             timer->Start(100);
//     //         }
//     //         cout << "Freinage activé (Up+1) " <<    voiture->getGamma()  << endl;

//     //     }
//     //     else if (keyCode == '2' && wxGetKeyState(WXK_UP)) {
//     //         intesite = 20.0/100.0;
//     //         voiture->setGamma(voiture->getCapaciteFreinage()  * intesite);
//     //         current = voiture->getGamma();
//     //         last = last_event.getGamma();
//     //         if (current != last)
//     //         {
//     //             voiture->setVitesseInitial(voiture->getVitesse());
//     //             dashboard->t = 0;
//     //             timer->Start(100);
//     //         }
//     //         cout << "Freinage activé (Up+2) " <<   voiture->getGamma() << endl;

//     //     }
//     // else if (keyCode == 'F' && wxGetKeyState(WXK_UP)) {
//     //     intesite = 1;
//     //     voiture->setGamma(voiture->getCapaciteFreinage()  * intesite);
//     //     current = voiture->getGamma();
//     //     last = last_event.getGamma();
//     //     if (current != last)
//     //     {
//     //         voiture->setVitesseInitial(voiture->getVitesse());
//     //         dashboard->t = 0;
//     //         timer->Start(100);
//     //     }
//     //     cout << "Freinage activé (Up+f) " <<  voiture->getGamma() <<endl;

//     // }

//     dashboard->Refresh();
//     this->Refresh();
//     event.Skip(); // Important !
// }

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
#define COMBO_ID 1902

MainFrame::MainFrame(const wxString &title, Voiture *voiture, vector<Voiture *> all_ptr_car)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1900, 900)), all_ptr_car(all_ptr_car)
{
    this->setVoiture(voiture);
    this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
    this->SetAutoLayout(false);
    wxPanel *toto = new wxPanel(this, wxID_ANY, wxPoint(13, 47), wxSize(430, 640));
    toto->SetBackgroundColour(*wxYELLOW);
    toto->Enable(false);

    vector<Voiture *> ptrs_car = this->getAllPtrCar();
    for (Voiture *v : ptrs_car)
    {
        options[v->getModel()] = v;
    }
    cout << "taille " << ptrs_car.capacity() << endl;

    wxArrayString choices;
    for (const auto &entry : options)
    {
        choices.Add(entry.first); // Ajouter uniquement le nom visible
    }

    wxFont font(20, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD);

    wxStaticText *nom_voiture = new wxStaticText(this, wxID_ANY,
                                                 "Voiture: ",
                                                 wxPoint(1120, 5),
                                                 wxSize(300, 60));

    nom_voiture->SetFont(font);

    dashboard = new DashBoard(this, voiture);
    dashboard->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyDown, this);
    dashboard->Bind(wxEVT_KEY_UP, &MainFrame::OnKeyUp, this);

    v_combo = new wxComboBox(this, COMBO_ID, "", wxPoint(1250, 10), wxSize(200, -1), choices, wxCB_READONLY);
    // dashboard->SetFocus();
    dashboard->SetFocus();
    v_combo->Bind(wxEVT_COMBOBOX, &MainFrame::OnComboBoxSelect, this);
    v_combo->Bind(wxEVT_KEY_DOWN, &MainFrame::OnComboKeyDown, this);
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    // 🔹 Met ici le code à exécuter avant la fermeture
    std::cout << "Fermeture de la fenêtre principale..." << std::endl;

    // Si tu veux libérer des ressources ou sauvegarder des données
    UtilDb::closeCon();  

    // 🔹 Appelle event.Skip() pour permettre la fermeture
    event.Skip();
}
void MainFrame::OnComboKeyDown(wxKeyEvent &event)
{
    dashboard->SetFocus();
    if (event.GetKeyCode() == WXK_UP || event.GetKeyCode() == WXK_DOWN)
    {
        dashboard->SetFocus(); // 🔹 Rediriger le focus sur le Dashboard
    }
    else
    {
        event.Skip(); // 🔹 Laisser les autres touches fonctionner normalement
    }
}
vector<Voiture *> MainFrame::getAllPtrCar()
{
    return this->all_ptr_car;
}

void MainFrame::setAllPtrCar(vector<Voiture *> all_ptr_car)
{
    this->all_ptr_car = all_ptr_car;
};
void MainFrame::OnComboBoxSelect(wxCommandEvent &event)
{

    wxString selectedName = v_combo->GetStringSelection();
    Voiture *value = options[selectedName];
    voiture = value;
    dashboard->setVoitureDash(value);
    dashboard->UpdateSpeedDisplay();
    cout << "selectionne " << value->getModel() << endl;
    // dashboard->SetFocus();
    // event.Skip();
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
    m_currentlyPressedKeys.insert(keyCode); 
    double intensite = 0.0;
    double current = 0;
    Evenement *last_event = nullptr;
    if (voiture->getEventTuplet().capacity() > 0)
    {
        last_event = &voiture->getEventTuplet().back();

        // last_event->save();
    }
    double last = -1;
    if (last_event != nullptr)
    {
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
        if (current != last)
        {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning())
        {
            voiture->setGamma(voiture->getCapaciteAccelere() * intensite);
            voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
            // Evenement last_event = voiture->getEventTuplet().back();
            timer->Start(100);
        }
    }
    else if (keyCode == '1' && wxGetKeyState(WXK_UP))
    {
        // cout << "acceleration 1" << endl;
        intensite = 10.0 / 100.0;
        current = voiture->getCapaciteAccelere() * intensite;
        if (current != last)
        {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning())
        {
            voiture->setGamma(voiture->getCapaciteAccelere() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());
            // voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
            // Evenement last_event = voiture->getEventTuplet().back();
            timer->Start(100);
        }
    }
    else if (keyCode == '2' && wxGetKeyState(WXK_UP))
    {
        // cout << "acceleration 2" << endl;
        intensite = 20.0 / 100.0;
        current = voiture->getCapaciteAccelere() * intensite;
        if (current != last)
        {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning())
        {
            voiture->setGamma(voiture->getCapaciteAccelere() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());
            // voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
            // Evenement last_event = voiture->getEventTuplet().back();
            timer->Start(100);
        }
    }
    if (keyCode == 'F' && wxGetKeyState(WXK_DOWN))
    {
        // cout << "freinage full" << endl;
        intensite = 1;
        current = voiture->getCapaciteFreinage() * intensite;
        if (current != last)
        {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning())
        {
            voiture->setGamma(-voiture->getCapaciteFreinage() * intensite);
            // voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
            // Evenement last_event = voiture->getEventTuplet().back();
            timer->Start(100);
        }
    }
    else if (keyCode == '1' && wxGetKeyState(WXK_DOWN))
    {
        // cout << "freinage 1" << endl;
        intensite = 10.0 / 100.0;
        current = voiture->getCapaciteFreinage() * intensite;
        if (current != last)
        {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning())
        {
            voiture->setGamma(-voiture->getCapaciteFreinage() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());
            // voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "xxx"));
            // Evenement last_event = voiture->getEventTuplet().back();
            timer->Start(100);
        }
    }
    else if (keyCode == '2' && wxGetKeyState(WXK_DOWN))
    {
        // cout << "freinage 2" << endl;
        intensite = 20.0 / 100.0;
        current = voiture->getCapaciteFreinage() * intensite;
        if (current != last)
        {
            dashboard->t = 0;
            voiture->setVitesseInitial(voiture->getVitesse());
        }
        if (!timer->IsRunning())
        {
            voiture->setGamma(-voiture->getCapaciteFreinage() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());

            // voiture->addEvents(Evenement(1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), "09:10:01"));
            // Evenement last_event = voiture->getEventTuplet().back();

            timer->Start(100);
        }
    }

    event.Skip();
}

void MainFrame::OnKeyUp(wxKeyEvent &event) {
    

    int keyCode = event.GetKeyCode();
    m_currentlyPressedKeys.erase(keyCode); 

    if (m_currentlyPressedKeys.empty()) {
        cout << "enregistrement event (une seule fois)" << endl;
        
        wxTimer* timer = dashboard->getTimer();
        Evenement* temp_event = new Evenement();
        int last_id = 0;
        
        for (Evenement ev : (*temp_event).getAll()) {
            if (ev.getId() > last_id) last_id = ev.getId();
        }
        
        voiture->addEvents(Evenement(last_id + 1, voiture->getId(), 
                                   voiture->getVitesseInital(), 
                                   voiture->getGamma(), "09:10:01"));
        
        voiture->getEventTuplet().back().save();
    }
    // dashboard->SetFocus();
    dashboard->getTimer()->Stop();
    event.Skip();
    // event.Skip(); // Décommentez si nécessaire
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

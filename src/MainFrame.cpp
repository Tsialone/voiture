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
    // v_combo->Enable(false);
    dashboard->SetFocus();
    v_combo->Bind(wxEVT_COMBOBOX, &MainFrame::OnComboBoxSelect, this);
    v_combo->Bind(wxEVT_KEY_DOWN, &MainFrame::OnComboKeyDown, this);

    wxButton *button = new wxButton(this, wxID_ANY, "Replay", wxPoint(50, 50), wxSize(150, 50));
    button->Bind(wxEVT_BUTTON, &MainFrame::replay, this);
}

void MainFrame::replay(wxCommandEvent &event)
{

    Evenement *temp_ev = new Evenement();
    dashboard-> m_allEv.clear();

    for (Evenement ev : temp_ev->getAll())
    {
        if (ev.getIdVoiture() == voiture->getId())
        {
            // voiture  =  
            dashboard->m_allEv.push_back(ev);
        }
    }
    if (dashboard->m_allEv.size() >= 2)
    {
        // voiture->consommation_t = 0;
        dashboard->m_currentIndex = 0.0;
        voiture->consommation_t = 0.0;
        voiture->carburant_consomme = 0.0;
        dashboard->replay_t = 0.0;


        wxTimer *dash_timer = dashboard->replay_timer;
        dash_timer->Start(100);
        //     for (int i = 0; i < all_ev.capacity() - 1; i++)
        //     {

        //         if (all_ev[i].getIdVoiture() == voiture->getId())
        //         {
        //             std::string time1 = all_ev[i].getTempDebut();
        //             std::string time2 = all_ev[i + 1].getTempDebut();
        //             int seconds1 = parseTime(time1);
        //             int seconds2 = parseTime(time2);

        //             int temp_t = seconds2 - seconds1;
        //             double temp_gamma = all_ev[i].getGamma();
        //             double temp_v0 = all_ev[i].getVitesseInitial();

        //             dash_timer->Start(100);
        //             wxMilliSleep(temp_t * 1000);
        //             wxYield();

        //             cout << "difference: " << temp_t << " index: " << i << endl;
        //             break;
        //         }
        //     }
    }
    // wxMessageBox("Bouton cliqué !", "Information", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnClose(wxCloseEvent &event)
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
    Voiture *temp = new Voiture();
    temp = temp->getById(value->getId());
    // Voiture xoxo ((*temp).getId(), (*temp).getModel(), (*temp).getVitesseMaximal(), (*temp).getCapaciteAccelere(), (*temp).getCapaciteFreinage(), (*temp).getCarburantMaximal(), (*temp).getConsommation());
    
    
    
    
    voiture = temp;
    voiture->conso_fixe = voiture->getConsommation();

    cout << "consomme misafidy " <<  voiture->conso_fixe  << endl;
     dashboard->setVoitureDash(voiture);
    dashboard->angleAiguille = -180 * ((voiture->getVitesse() / voiture->getVitesseMaximal()));
    voiture->setCarburantActuel(voiture->getCarburantMaximal() - voiture->getConsommation() * voiture->consommation_t);

    int fuelPercent = static_cast<int>(
        voiture->getCarburantActuel() /
        voiture->getCarburantMaximal() * 100);
    dashboard->fuelGauge->SetValue(fuelPercent);

    dashboard->fuelGauge->Update();
    dashboard->fuelGauge->Refresh();

    dashboard->UpdateSpeedDisplay();
    // cout << "selectionne " << value->getModel() << endl;
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
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *local_time = std::localtime(&now_c);
    int hour = local_time->tm_hour;
    int minute = local_time->tm_min;
    int second = local_time->tm_sec;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hour << ":"
        << std::setw(2) << std::setfill('0') << minute << ":"
        << std::setw(2) << std::setfill('0') << second;
    std::string my_time = oss.str();

    int last_id = 0;
    Evenement *temp_event = new Evenement();
    for (Evenement ev : (*temp_event).getAll())
    {
        if (ev.getId() > last_id)
            last_id = ev.getId();
    }

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
    double last = 0;
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
        // current = voiture->getCapaciteAccelere() * intensite;
        // if (current != last)
        // {
        //     // cout << " ato foana " << endl;
        //     // voiture->setVitesseInitial(voiture->getVitesse());
        //     // cout << " changement de vitesse initial " <<   voiture->getVitesseInital()  << " current " << current << " last " << last <<endl;
        // }
        if (!timer->IsRunning())
        {   
            voiture->setConsommation(voiture->conso_fixe * intensite );
            voiture->setGamma(voiture->getCapaciteAccelere() * intensite);
            voiture->addEvents(Evenement(last_id + 1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), my_time));
            Evenement last_event = voiture->getEventTuplet().back();
            last_event.save();

            timer->Start(100);
        }
    }
    else if (keyCode == '1' && wxGetKeyState(WXK_UP))
    {
        // cout << "acceleration 1" << endl;
        intensite = 10.0 / 100.0;

        // current = voiture->getCapaciteAccelere() * intensite;
        // if (current != last)
        // {
        //     dashboard->t = 0;
        //     voiture->setVitesseInitial(voiture->getVitesse());
        // }
        if (!timer->IsRunning())
        {
            voiture->setConsommation(voiture->conso_fixe * intensite );
            cout << "ato zah no miova " << voiture->conso_fixe << endl; 
            voiture->setGamma(voiture->getCapaciteAccelere() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());
            voiture->addEvents(Evenement(last_id + 1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), my_time));
            Evenement last_event = voiture->getEventTuplet().back();
            last_event.save();
            timer->Start(100);
        }
    }
    else if (keyCode == '2' && wxGetKeyState(WXK_UP))
    {
        // cout << "acceleration 2" << endl;
        intensite = 20.0 / 100.0;
        
        // current = voiture->getCapaciteAccelere() * intensite;
        // if (current != last)
        // {
            //     dashboard->t = 0;
            //     voiture->setVitesseInitial(voiture->getVitesse());
            // }
            if (!timer->IsRunning())
            {
            voiture->setConsommation(voiture->conso_fixe * intensite );
            voiture->setGamma(voiture->getCapaciteAccelere() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());
            voiture->addEvents(Evenement(last_id + 1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), my_time));
            Evenement last_event = voiture->getEventTuplet().back();
            last_event.save();
            timer->Start(100);
        }
    }
    if (keyCode == 'F' && wxGetKeyState(WXK_DOWN))
    {
        // cout << "freinage full" << endl;
        intensite = 1;
        // current = voiture->getCapaciteFreinage() * intensite;
        // if (current != last)
        // {
        //     dashboard->t = 0;
        //     voiture->setVitesseInitial(voiture->getVitesse());
        // }
        if (!timer->IsRunning())
        {
            voiture->setGamma(-voiture->getCapaciteFreinage() * intensite);
            voiture->addEvents(Evenement(last_id + 1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), my_time));
            Evenement last_event = voiture->getEventTuplet().back();
            last_event.save();
            timer->Start(100);
        }
    }
    else if (keyCode == '1' && wxGetKeyState(WXK_DOWN))
    {
        // cout << "freinage 1" << endl;
        intensite = 10.0 / 100.0;
        // voiture->setConsommation(voiture->conso_fixe * intensite);

        // current = voiture->getCapaciteFreinage() * intensite;
        // if (current != last)
        // {
        //     dashboard->t = 0;
        //     voiture->setVitesseInitial(voiture->getVitesse());
        // }
        if (!timer->IsRunning())
        {
            voiture->setGamma(-voiture->getCapaciteFreinage() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());
            voiture->addEvents(Evenement(last_id + 1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), my_time));
            Evenement last_event = voiture->getEventTuplet().back();
            last_event.save();
            timer->Start(100);
        }
    }
    else if (keyCode == '2' && wxGetKeyState(WXK_DOWN))
    {
        // cout << "freinage 2" << endl;
        intensite = 20.0 / 100.0;
        // voiture->setConsommation(voiture->conso_fixe * intensite);

        // current = voiture->getCapaciteFreinage() * intensite;
        // if (current != last)
        // {
        //     dashboard->t = 0;
        //     voiture->setVitesseInitial(voiture->getVitesse());
        // }
        if (!timer->IsRunning())
        {
            voiture->setGamma(-voiture->getCapaciteFreinage() * intensite);
            // voiture->setVitesseInitial(voiture->getVitesse());
            voiture->addEvents(Evenement(last_id + 1, voiture->getId(), voiture->getVitesseInital(), voiture->getGamma(), my_time));
            Evenement last_event = voiture->getEventTuplet().back();
            last_event.save();

            timer->Start(100);
        }
    }

    event.Skip();
}

void MainFrame::OnKeyUp(wxKeyEvent &event)
{
    int keyCode = event.GetKeyCode();
    vector<char> command = {'F', '1', '2', '3'};
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *local_time = std::localtime(&now_c);
    int hour = local_time->tm_hour;
    int minute = local_time->tm_min;
    int second = local_time->tm_sec;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hour << ":"
        << std::setw(2) << std::setfill('0') << minute << ":"
        << std::setw(2) << std::setfill('0') << second;
    std::string my_time = oss.str();

    bool contains = find(command.begin(), command.end(), keyCode) != command.end();
    // m_currentlyPressedKeys.erase(keyCode);
    if (contains && (wxGetKeyState(WXK_UP) || wxGetKeyState(WXK_DOWN)))
    {

        cout << " sa vitesse initial  " << voiture->getVitesseInital() << endl;
        int last_id = 0;
        Evenement *temp_event = new Evenement();
        for (Evenement ev : (*temp_event).getAll())
        {
            if (ev.getId() > last_id)
                last_id = ev.getId();
        }

        double current = voiture->getGamma();
        voiture->setGamma(0);
        voiture->addEvents(Evenement(last_id + 1, voiture->getId(), voiture->getVitesse(), voiture->getGamma(), my_time));
        Evenement last_event = voiture->getEventTuplet().back();
        last_event.save();

        Evenement *last_ev = nullptr;
        if (voiture->getEventTuplet().capacity() > 0)
        {
            last_ev = &voiture->getEventTuplet().back();
            // last_event->save();
        }
        double last = 0;
        if (last_ev != nullptr)
        {
            last = last_ev->getGamma();
        }

        if (current != last)
        {
            // dashboard->  parcouru_t =0;
            voiture->setVitesseInitial(voiture->getVitesse());
            // voiture->position_initial = voiture->position;
            cout << "gamma: " << voiture->getGamma()  << " v0  " << voiture->getVitesseInital() <<   " x0: " << voiture->position_initial <<endl;
            dashboard->t = 0;
        }
    }

    dashboard->SetFocus();
    dashboard->getTimer()->Stop();
    event.Skip();
}

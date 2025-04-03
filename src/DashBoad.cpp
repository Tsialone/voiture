#include "DashBoard.hpp"
#include <iomanip>
#include "Voiture.hpp"
#include "MainFrame.hpp"
#include <wx/dcbuffer.h>

#define TIMER_MAIN 1000 // Identifiant unique pour timer
#define TIMER_CONSO 1001
#define TIMER_REPLAY 1002

wxBEGIN_EVENT_TABLE(DashBoard, wxPanel)
    EVT_TIMER(TIMER_MAIN, DashBoard::OnTimer)
        EVT_TIMER(TIMER_CONSO, DashBoard::consommationTimer)
            EVT_TIMER(TIMER_REPLAY, DashBoard::replayTimer)

                EVT_PAINT(DashBoard::OnPaint)
                    wxEND_EVENT_TABLE()

                        DashBoard::DashBoard(wxWindow *parent, Voiture *voiture)
    : wxPanel(parent, wxID_ANY, wxPoint(450, 50), wxSize(1000, 800),
              wxBORDER_SIMPLE | wxNO_FULL_REPAINT_ON_RESIZE),
      angleAiguille(0)
{
    SetAutoLayout(false);
    SetBackgroundColour(*wxLIGHT_GREY);
    voiture_dash = voiture;

    wxFont font(30, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << voiture_dash->getVitesse();
    std::string vitesse_label = oss.str();
    speed_display = new wxStaticText(this, wxID_ANY,
                                     vitesse_label + "km/h",
                                     wxPoint(420, 740),
                                     wxSize(300, 50));

    parcouru_display = new wxStaticText(this, wxID_ANY,
                                        +"0.00km",
                                        wxPoint(420, 640),
                                        wxSize(300, 50));

    parcouru_display->SetForegroundColour(*wxBLACK);
    speed_display->SetFont(font);
    parcouru_display->SetFont(font);

    timer = new wxTimer(this, TIMER_MAIN);
    consommation_timer = new wxTimer(this, TIMER_CONSO);
    replay_timer = new wxTimer(this, TIMER_REPLAY);

    SetBackgroundStyle(wxBG_STYLE_PAINT);
    CreateFuelGauge();
}
int parseTime(const std::string &timeStr)
{
    std::tm tm = {}; // Initialisation à zéro
    std::istringstream ss(timeStr);
    ss >> std::get_time(&tm, "%H:%M:%S");                  // Parser HH:MM:SS
    return tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec; // Convertir en secondes
}
void DashBoard::replayTimer(wxTimerEvent &event)
{
    double s = static_cast<double>(replay_timer->GetInterval() / 1000.0);
    replay_t += s;
    // t +=s;

    std::string time1 = m_allEv[m_currentIndex].getTempDebut();
    std::string time2 = m_allEv[m_currentIndex + 1].getTempDebut();
    int seconds1 = parseTime(time1);
    int seconds2 = parseTime(time2);

    int temp_t = seconds2 - seconds1;
    double temp_gamma = m_allEv[m_currentIndex].getGamma();
    double temp_v0 = m_allEv[m_currentIndex].getVitesseInitial();

    double pourcentage_consommation = fabs(temp_gamma * 100 / voiture_dash->getCapaciteAccelere());
    if (temp_v0 < 0)
    {
        pourcentage_consommation = fabs(temp_gamma * 100 / voiture_dash->getCapaciteFreinage());
    }
    cout << "consommme pourcentage " << pourcentage_consommation << endl;

    voiture_dash->setVitesseInitial(temp_v0);
    voiture_dash->setGamma(temp_gamma);

    double vitesse = voiture_dash->getGamma() * replay_t + voiture_dash->getVitesseInital();

    // cout << " vitesse initial  " << voiture_dash->getVitesseInital() << endl;
    // cout << " vitesse miova " << vitesse << endl;
    // cout << " gamma " << voiture_dash->getGamma() << "t: " << t << endl;

    if (vitesse > voiture_dash->getVitesseMaximal())
    {
        vitesse = voiture_dash->getVitesseMaximal();
    }
    else if (vitesse < 0)
    {
        vitesse = 0;
    }
    double reste_carburant = voiture_dash->getCarburantMaximal() - voiture_dash->carburant_consomme;

    if (reste_carburant > 0)
    {
        voiture_dash->setVitesse(vitesse);
        UpdateSpeedDisplay();
    }
    angleAiguille = -180 * ((voiture_dash->getVitesse() / voiture_dash->getVitesseMaximal()));
    t_enjehina = temp_t;

    voiture_dash->setConsommation(voiture_dash->conso_fixe * (pourcentage_consommation / 100));

    if (voiture_dash->getVitesse() > 0 && voiture_dash->getGamma() > 0 && reste_carburant > 0)
    {

        voiture_dash->consommation_t += s;
        cout << "consommation: " << voiture_dash->getConsommation() << endl;
        // cout << "->hummm " << voiture_dash->getGamma() << endl;

        // voiture_dash->setCarburantActuel(voiture_dash->getCarburantMaximal()- voiture_dash->getConsommation() * voiture_dash->consommation_t  );

        voiture_dash->carburant_consomme += voiture_dash->getConsommation() * voiture_dash->consommation_t;
        cout << "voiture dash consommation " << voiture_dash->consommation_t << endl;

        cout << voiture_dash->carburant_consomme << "/" << voiture_dash->getCarburantMaximal() << endl;

        int fuelPercent = static_cast<int>(
            ((voiture_dash->getCarburantMaximal() - voiture_dash->carburant_consomme) /
             voiture_dash->getCarburantMaximal()) *
            100);
        fuelGauge->SetValue(fuelPercent);
        // cout << "Carburant: " << voiture_dash->getCarburantActuel() << " | Consommation " << consommation_t << endl;
        fuelGauge->Refresh();
        // fuelGauge->Update();
    }

    if (replay_t >= temp_t)
    {
        cout << "tafa enjika  " << replay_t << " " << voiture_dash->getVitesseInital() << endl;
        replay_t = 0;
        // replay_timer->Stop();
        m_currentIndex++;
    }
    if (m_currentIndex >= m_allEv.size() - 1)
    {
        replay_timer->Stop();
    }

    cout << "difference: " << temp_t << " t: " << replay_t << " index: " << m_currentIndex << endl;

    //     break;
}

wxStaticText *DashBoard::getSpeedDisplay()
{

    return this->speed_display;
}

void DashBoard::setSpeedDisplay(wxStaticText *speed_display)
{
    this->speed_display = speed_display;
}

void DashBoard::UpdateSpeedDisplay()
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << voiture_dash->getVitesse();
    speed_display->SetLabel(oss.str() + "km/h");
    Refresh();
}

void DashBoard::UpdateParcouruDisplay()
{
    std::ostringstream oss;
    // double distance_parcouru = (voiture_dash->getGamma() * t*t / 2)
    oss << std::fixed << std::setprecision(2) << voiture_dash->position;
    parcouru_display->SetLabel(oss.str() + "km");
    Refresh();
}

void DashBoard::setTimer(wxTimer *timer)
{
    this->timer = timer;
}

wxTimer *DashBoard::getTimer()
{
    return this->timer;
}
void DashBoard::setVoitureDash(Voiture *voiture_dash)
{
    this->voiture_dash = voiture_dash;
}

void DashBoard::OnTimer(wxTimerEvent &event)
{
    double s = static_cast<double>(timer->GetInterval() / 1000.0);
    t += s;
    // cout << " gamma: " << voiture_dash->getGamma() << endl;

    double vitesse = voiture_dash->getGamma() * t + voiture_dash->getVitesseInital();
    // double position = (t*t * voiture_dash->getGamma() / 2 ) + voiture_dash->position_initial + voiture_dash->getVitesseInital() * t;

    cout << " vitesse initial  " << voiture_dash->getVitesseInital() << endl;
    cout << " vitesse miova " << vitesse << endl;
    cout << " gamma " << voiture_dash->getGamma() << "t: " << t << endl;

    if (vitesse > voiture_dash->getVitesseMaximal())
    {
        vitesse = voiture_dash->getVitesseMaximal();
    }
    else if (vitesse < 0)
    {
        vitesse = 0;
    }
    double reste_carburant = voiture_dash->getCarburantMaximal() - voiture_dash->carburant_consomme;
    if (reste_carburant > 0 || voiture_dash->getGamma() < 0)
    {
        voiture_dash->setVitesse(vitesse);
        if (!consommation_timer->IsRunning())
        {
            consommation_timer->Start(100);
        }
        // voiture_dash->position = position;
        UpdateSpeedDisplay();
        angleAiguille = -180 * ((voiture_dash->getVitesse() / voiture_dash->getVitesseMaximal()));
        // UpdateParcouruDisplay();
    }
    // cout << "gamma: " << voiture_dash->getGamma() << endl;
    if (voiture_dash->getVitesse() > 0 && voiture_dash->getGamma() > 0 && reste_carburant > 0)
    {

        voiture_dash->consommation_t += s;
        cout << "consommation: " << voiture_dash->getConsommation() << endl;
        // cout << "->hummm " << voiture_dash->getGamma() << endl;

        // voiture_dash->setCarburantActuel(voiture_dash->getCarburantMaximal()- voiture_dash->getConsommation() * voiture_dash->consommation_t  );

        voiture_dash->carburant_consomme += voiture_dash->getConsommation() * voiture_dash->consommation_t;
        reste_carburant = voiture_dash->getCarburantMaximal() - voiture_dash->carburant_consomme;
        cout << voiture_dash->carburant_consomme << "/" << voiture_dash->getCarburantMaximal() << endl;

        int fuelPercent = static_cast<int>(
            ((voiture_dash->getCarburantMaximal() - voiture_dash->carburant_consomme) /
             voiture_dash->getCarburantMaximal()) *
            100);
        if (reste_carburant > 0)
        {
            fuelGauge->SetValue(fuelPercent);
        }
        // cout << "Carburant: " << voiture_dash->getCarburantActuel() << " | Consommation " << consommation_t << endl;
        fuelGauge->Refresh();
        // fuelGauge->Update();
    }

    event.Skip();
}

void DashBoard::consommationTimer(wxTimerEvent &event)
{
    // cout << "Mise à jour consommation\n";
    if (voiture_dash->getVitesse() > 0)
    {
        double s = static_cast<double>(consommation_timer->GetInterval() / 1000.0);  
        double v = voiture_dash->getVitesse()/3600;  
        double a = voiture_dash->getGamma()/3600;   
        voiture_dash->position += (v * s) + (0.5 * a * s * s); 
        UpdateParcouruDisplay();  // Mise à jour de l'affichage
    }

    // if (voiture_dash->getVitesse() > 0 && voiture_dash->getGamma() > 0)
    // {
    //     voiture_dash->setCarburantActuel(voiture_dash->getCarburantMaximal() - voiture_dash->getConsommation() * consommation_t);

    //     int fuelPercent = static_cast<int>(voiture_dash->getCarburantActuel() /
    //                                        voiture_dash->getCarburantMaximal() * 100);
    //     fuelGauge->SetValue(fuelPercent);
    //     // cout << "Carburant: " << voiture_dash->getCarburantActuel() << " | Consommation " << consommation_t << endl;
    //     fuelGauge->Refresh();
    //     fuelGauge->Update();
    // }
    // else
    // {
    //     // voiture_dash->setCarburantMaximal(voiture_dash->getCarburantActuel());
    //     consommation_timer->Stop();
    //     // consommation_t = 0;
    // }

    // event.Skip();
}

void DashBoard::CreateFuelGauge()
{
    voiture_dash->setCarburantActuel(voiture_dash->getCarburantMaximal());
    fuelGauge = new wxGauge(this, wxID_ANY, 100, wxPoint(100, 450), wxSize(20, 300),
                            wxGA_VERTICAL); // Ajustement de la position et de la hauteur
    fuelGauge->Refresh();                   // Met à jour l'affichage immédiatement
}

void DashBoard::OnPaint(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    DrawSpeedIndicator(dc);
}

void DashBoard::DrawSpeedIndicator(wxDC &dc)
{
    wxPoint center(480, 750);
    int radius = 300;
    int vitesseMax = voiture_dash->getVitesseMaximal();

    dc.SetPen(wxPen(*wxBLACK, 3));
    dc.DrawArc(center.x + radius, center.y,
               center.x - radius, center.y,
               center.x, center.y);

    dc.SetFont(wxFont(14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    dc.SetTextForeground(*wxBLACK);

    int steps = 10;
    for (int i = 0; i <= steps; i++)
    {
        double angle = -180.0 * (i / (double)steps);
        double rad = wxDegToRad(angle);

        wxPoint inner(center.x - (radius - 25) * cos(rad),
                      center.y + (radius - 25) * sin(rad));
        wxPoint outer(center.x - radius * cos(rad),
                      center.y + radius * sin(rad));
        dc.DrawLine(inner, outer);

        int textRadius = radius + 25;
        int valeur = i * (vitesseMax / steps);
        wxString text = wxString::Format("%d", valeur);
        wxSize textSize = dc.GetTextExtent(text);

        wxPoint textPos;

        if (i == 0)
        {
            textPos = wxPoint(center.x - radius - textSize.x - 10,
                              center.y - textSize.y / 2);
        }
        else if (i == steps)
        {
            textPos = wxPoint(center.x + radius + 10,
                              center.y - textSize.y / 2);
        }
        else if (i == steps / 2)
        {
            textPos = wxPoint(center.x - textSize.x / 2,
                              center.y - radius - textSize.y - 10);
        }
        else
        {
            textPos = wxPoint(
                center.x - textRadius * cos(rad) - textSize.x / 2,
                center.y + textRadius * sin(rad) - textSize.y / 2);

            if (i > steps / 2)
            {
                textPos.y += 10;
            }
            else
            {
                textPos.y -= 10;
            }
        }

        dc.DrawText(text, textPos);
    }

    // Aiguille
    dc.SetPen(wxPen(*wxRED, 4));
    double rad = wxDegToRad(angleAiguille);
    wxPoint needleEnd(
        center.x - radius * cos(rad),
        center.y + radius * sin(rad));
    dc.DrawLine(center, needleEnd);
}

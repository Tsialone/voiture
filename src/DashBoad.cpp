#include "DashBoard.hpp"
#include <iomanip>
#include "Voiture.hpp"
#include <wx/dcbuffer.h>

#define TIMER_MAIN 1000  // Identifiant unique pour timer
#define TIMER_CONSO 1001 // Identifiant unique pour consommation_timer

wxBEGIN_EVENT_TABLE(DashBoard, wxPanel)
    EVT_TIMER(TIMER_MAIN, DashBoard::OnTimer)
        EVT_TIMER(TIMER_CONSO, DashBoard::consommationTimer)
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
                                     vitesse_label + "kmh",
                                     wxPoint(420, 740),
                                     wxSize(300, 50));
    speed_display->SetFont(font);

    timer = new wxTimer(this, TIMER_MAIN);               // Timer principal
    consommation_timer = new wxTimer(this, TIMER_CONSO); // Timer consommation

    SetBackgroundStyle(wxBG_STYLE_PAINT);
    CreateFuelGauge();
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
    speed_display->SetLabel(oss.str() + "kmh");
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
    double vitesse = voiture_dash->getGamma() * t + voiture_dash->getVitesseInital();

    if (vitesse > voiture_dash->getVitesseMaximal())
    {
        vitesse = voiture_dash->getVitesseMaximal();
    }
    else if (vitesse < 0)
    {
        vitesse = 0;
    }
    if (voiture_dash->getCarburantActuel() > 0)
    {
        voiture_dash->setVitesse(vitesse);
        UpdateSpeedDisplay();
    }

    angleAiguille = -180 * ((voiture_dash->getVitesse() / voiture_dash->getVitesseMaximal()));

    if (vitesse != 0 && !consommation_timer->IsRunning())
    {
        consommation_timer->Start(100);
    }

    event.Skip();
}

void DashBoard::consommationTimer(wxTimerEvent &event)
{
    // cout << "Mise à jour consommation\n";
    double s = static_cast<double>(consommation_timer->GetInterval() / 1000.0);
    consommation_t += s;

    if (voiture_dash->getVitesse() > 0)
    {
        voiture_dash->setCarburantActuel(voiture_dash->getCarburantMaximal() - voiture_dash->getConsommation() * consommation_t);

        int fuelPercent = static_cast<int>(voiture_dash->getCarburantActuel() /
                                           voiture_dash->getCarburantMaximal() * 100);
        fuelGauge->SetValue(fuelPercent);
        // cout << "Carburant: " << voiture_dash->getCarburantActuel() << " | Consommation " << consommation_t << endl;
        fuelGauge->Refresh();
        fuelGauge->Update();
    }
    else
    {
        // voiture_dash->setCarburantMaximal(voiture_dash->getCarburantActuel());
        consommation_timer->Stop();
        // consommation_t = 0;
    }

    event.Skip();
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

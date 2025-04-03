#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <wx/wx.h>
#include <string>
#include <wx/timer.h>
#include "Voiture.hpp"
class DashBoard : public wxPanel
{
public:
    double t = 0.0;
    double t_enjehina = 0.0;
    double replay_t = 0.0;
    wxTimer *replay_timer;
     int m_currentIndex;
    vector<Evenement> m_allEv;

    // double consommation_t = 0.0;
    DashBoard(wxWindow *parent, Voiture *voiture_dash);
    Voiture *getVoitureDash() const;
    void setVoitureDash(Voiture *voiture_dash);
    void UpdateSpeedDisplay();
    wxStaticText *getSpeedDisplay();
    void setSpeedDisplay(wxStaticText *speed_display);
    double angleAiguille;
    wxTimer *getTimer();
    void setTimer(wxTimer *timer);
    wxGauge *fuelGauge; // Ajoutez ce membre
    void CreateFuelGauge();
    wxTimer *consommation_timer;
    wxStaticText *parcouru_display;
    double parcouru_t = 0.0;

private:
    Voiture *voiture_dash;
    wxStaticText *speed_display;
    void UpdateParcouruDisplay();

    wxTimer *timer;
    void OnPaint(wxPaintEvent &event);
    void DrawSpeedIndicator(wxDC &dc);
    void OnTimer(wxTimerEvent &event);
    void consommationTimer(wxTimerEvent &event);
    void replayTimer(wxTimerEvent &event);
    wxDECLARE_EVENT_TABLE();
};

#endif
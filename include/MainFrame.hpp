#include <wx/wx.h>
#include <wx/combo.h>
#include "Voiture.hpp"
#include "DashBoard.hpp"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, Voiture * voiture);
    Voiture *getVoiture();
    void setVoiture(Voiture  * voiture);
    void OnKeyDown(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);


private:

    DashBoard * dashboard;
    Voiture *voiture;
    wxComboBox *m_combo;
};
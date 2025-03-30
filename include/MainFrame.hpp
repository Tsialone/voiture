#include <wx/wx.h>
#include <wx/combo.h>
#include "Voiture.hpp"
#include "DashBoard.hpp"
#include <map>
#include <wx/combobox.h>
#include <set>
using namespace std;
class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, Voiture *voiture, vector<Voiture *> all_ptr_car);
    Voiture *getVoiture();
    void setVoiture(Voiture *voiture);
    void OnKeyDown(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);
    wxComboBox *v_combo;
    std::map<wxString, Voiture *> options;
    void OnComboKeyDown(wxKeyEvent &event);
    vector<Voiture *> getAllPtrCar();
    void setAllPtrCar(vector<Voiture *> all_ptr_car);
    set<int> m_currentlyPressedKeys;
private:
    void OnClose(wxCloseEvent &event);
    void OnComboBoxSelect(wxCommandEvent &event);
    vector<Voiture *> all_ptr_car;
    DashBoard *dashboard;
    Voiture *voiture;
};
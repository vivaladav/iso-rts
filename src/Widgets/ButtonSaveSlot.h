#include <sgl/sgui/ImageButton.h>

namespace sgl { namespace sgui { class Label; } }

namespace game
{

enum GameMode : unsigned int;
enum PlanetId : unsigned int;
enum PlayerFaction : unsigned int;

class ButtonSaveSlot : public sgl::sgui::ImageButton
{
public:
    ButtonSaveSlot(sgl::sgui::Widget * parent);
    ButtonSaveSlot(GameMode mode, PlayerFaction f, PlanetId planet, unsigned int map, int turn,
                   std::time_t timestamp, sgl::sgui::Widget * parent);

#ifdef DEMO_MODE
    void MarkNotAvailable();

#endif

private:
    void OnStateChanged(sgl::sgui::AbstractButton::VisualState state) override;
    void UpdateColors(sgl::sgui::AbstractButton::VisualState state);

    void HandleMouseOver() override;
    void HandleButtonDown() override;

private:
    sgl::sgui::Label * mLabelMode = nullptr;
    sgl::sgui::Label * mLabelPlanet = nullptr;
    sgl::sgui::Label * mLabelTurn = nullptr;
    sgl::sgui::Label * mLabelFaction = nullptr;
    sgl::sgui::Label * mLabelTimestamp = nullptr;

    PlayerFaction mFaction;
};

}
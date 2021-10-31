#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Core/Application.h"
#include "Gugu/Events/EventListener.h"
#include "Gugu/Math/UDim.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementText;
    class ElementSprite;
    class SpriteAnimation;
}

////////////////////////////////////////////////////////////////
// File Declarations

namespace demoproject {
    
class Demo : public gugu::Application, public gugu::EventListener
{
protected:

    enum class EButton
    {
        Quit,
        Roll,
        Clear,
        AddDice4,
        AddDice6,
        AddDice8,
        AddDice10,
        AddDice12,
        AddDice20,
        AddDice100,
        RemoveDice,
    };

    enum class EDiceType
    {
        undefined,
        d4,
        d6,
        d8,
        d10,
        d12,
        d20,
        d100,
    };

    struct Dice
    {
        EDiceType type = EDiceType::undefined;
        gugu::Element* pivot = nullptr;
        gugu::ElementSprite* sprite = nullptr;
        gugu::SpriteAnimation* animation = nullptr;
        gugu::ElementText* resultText = nullptr;
        gugu::Element* buttonReroll = nullptr;
        int result = 0;
        int animationTime = 0;
    };

public:

    Demo();
    virtual ~Demo();

    virtual void AppStart() override;
    virtual void AppStop() override;

    virtual void AppUpdate(const gugu::DeltaTime& dt) override;

protected:

    void SetupStandard();
    void DecorateButton(gugu::Element* button, const std::string& textureID, const gugu::UDim2& position, float scale);
    void DecorateButton(gugu::Element* button, const std::string& textureID, const gugu::UDim2& position, const gugu::UDim2& origin, float scale);

    void ClearDices();
    void PrepareDices(EDiceType type, int count);
    void AddDice(EDiceType type);
    void RemoveDice();
    void ClearSingleDice(int index);

    void RollAllDices();
    void RerollDice(int index);
    void RollSingleDice(int index, bool delay);

    void OnButtonClick(EButton button);
    void OnReroll(int index);

protected:

    gugu::Element* m_root = nullptr;
    gugu::ElementSprite* m_spriteBackground = nullptr;
    gugu::ElementText* m_textResult = nullptr;

    std::vector<Dice> m_currentDices;
    bool m_rolling = false;

    bool m_animatingBackground = false;
    int m_animationStep = 0;
    int m_animationTime = 0;
};

}   //namespace demoproject

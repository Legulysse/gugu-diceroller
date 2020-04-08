#pragma once

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Misc/Application.h"
#include "Gugu/Misc/EventListener.h"

////////////////////////////////////////////////////////////////
// Forward Declarations

namespace gugu
{
    class Element;
    class ElementText;
    class ElementSpriteAnimated;
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
        AddDice6,
        AddDice12,
        AddDice20,
    };

    enum class EDiceType
    {
        undefined,
        d6,
        d12,
        d20,
    };

    struct Dice
    {
        EDiceType type = EDiceType::undefined;
        gugu::ElementSpriteAnimated* sprite = nullptr;
        gugu::ElementText* resultText = nullptr;
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

    void ClearDices();
    void PrepareDices(EDiceType type, int count);
    void AddDice(EDiceType type);
    void Roll();

    void OnButtonClick(EButton button);

protected:

    gugu::Element* m_root = nullptr;
    gugu::ElementText* m_textResult = nullptr;

    std::vector<Dice> m_currentDices;
    bool m_rolling = false;
};

}   //namespace demoproject

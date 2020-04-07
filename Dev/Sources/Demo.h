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
    };

public:

    Demo();
    virtual ~Demo();

    virtual void AppStart() override;
    virtual void AppStop() override;

    virtual void AppStep(const gugu::DeltaTime& dt) override;

protected:

    void Test();
    void SetupStandard();

    void OnButtonClick(EButton button);

protected:

    gugu::Element* m_root;
    gugu::ElementText* m_textResult;
};

}   //namespace demoproject

////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteAnimated.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/Utility/System.h"
#include "Gugu/Utility/Random.h"

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace demoproject {
    
Demo::Demo()
: m_root(nullptr)
{
}

Demo::~Demo()
{
}

void Demo::AppStart()
{
    RegisterHandlerEvents(GetGameWindow());

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(UDim(1.f, 0.f), UDim(1.f, 0.f)));

    SetupStandard();
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::Test()
{
    // Grid
    sf::VertexArray* gridVertices = new sf::VertexArray;
    gridVertices->setPrimitiveType(sf::PrimitiveType::Lines);

    float margin = 10.f;
    float cellSize = 200.f;
    int nbCells = 6;
    float lineSize = margin * 2 + (cellSize * (nbCells - 1));
    for (int x = 0; x < nbCells; ++x)
    {
        gridVertices->append(sf::Vertex(sf::Vector2f(0.f, margin + x * cellSize)));
        gridVertices->append(sf::Vertex(sf::Vector2f(lineSize, margin + x * cellSize)));

        gridVertices->append(sf::Vertex(sf::Vector2f(margin + x * cellSize, 0.f)));
        gridVertices->append(sf::Vertex(sf::Vector2f(margin + x * cellSize, lineSize)));
    }

    ElementSFDrawable* gridElement = m_root->AddChild<ElementSFDrawable>();
    gridElement->SetSFDrawable(gridVertices);

    float lineA = margin;
    float lineB = margin + cellSize * 1;
    float lineC = margin + cellSize * 2;
    float lineD = margin + cellSize * 3;
    float lineE = margin + cellSize * 4;


    // Test Animation Idle (no origin set)
    ElementSpriteAnimated* animationIdle = m_root->AddChild<ElementSpriteAnimated>();
    animationIdle->SetPosition(lineB, lineB);
    animationIdle->SetScale(0.2f);
    animationIdle->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    animationIdle->ChangeAnimSet("dice6.animset.xml");
    animationIdle->StartAnimation("roll");

    // Test Animation Walk (no origin set : top left)
    ElementSpriteAnimated* animationWalk1 = m_root->AddChild<ElementSpriteAnimated>();
    animationWalk1->SetPosition(lineC, lineB);
    animationWalk1->SetScale(0.2f);
    animationWalk1->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    animationWalk1->ChangeAnimSet("dice12.animset.xml");
    animationWalk1->StartAnimation("roll");

    // Test Animation Walk (origin set manually : bottom center)
    ElementSpriteAnimated* animationWalk2 = m_root->AddChild<ElementSpriteAnimated>();
    animationWalk2->SetPosition(lineD, lineB);
    animationWalk2->SetScale(0.2f);
    animationWalk2->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    animationWalk2->ChangeAnimSet("dice20.animset.xml");
    animationWalk2->StartAnimation("roll");

    // Test Animation Idle (no origin set)
    ElementSpriteAnimated* animationIdleb = m_root->AddChild<ElementSpriteAnimated>();
    animationIdleb->SetPosition(lineB, lineC);
    animationIdleb->SetScale(0.2f);
    animationIdleb->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    animationIdleb->ChangeAnimSet("dice6.animset.xml");
    animationIdleb->StartAnimation("idle");

    // Test Animation Walk (no origin set : top left)
    ElementSpriteAnimated* animationWalk1b = m_root->AddChild<ElementSpriteAnimated>();
    animationWalk1b->SetPosition(lineC, lineC);
    animationWalk1b->SetScale(0.2f);
    animationWalk1b->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    animationWalk1b->ChangeAnimSet("dice12.animset.xml");
    animationWalk1b->StartAnimation("idle");

    // Test Animation Walk (origin set manually : bottom center)
    ElementSpriteAnimated* animationWalk2b = m_root->AddChild<ElementSpriteAnimated>();
    animationWalk2b->SetPosition(lineD, lineC);
    animationWalk2b->SetScale(0.2f);
    animationWalk2b->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    animationWalk2b->ChangeAnimSet("dice20.animset.xml");
    animationWalk2b->StartAnimation("idle");
}

void Demo::SetupStandard()
{
    // Roll button
    ElementButton* buttonRoll = m_root->AddChild<ElementButton>();
    buttonRoll->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    buttonRoll->SetText("Roll");
    buttonRoll->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    buttonRoll->SetUnifiedPosition(UDim2::POSITION_BOTTOM_CENTER + sf::Vector2f(0, -20));
    buttonRoll->SetOnMouseReleased(new ActionClass1P<Demo, EButton>(this, &Demo::OnButtonClick, EButton::Roll));

    // Result
    m_textResult = m_root->AddChild<ElementText>();
    m_textResult->SetText("123456");
    m_textResult->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    m_textResult->SetUnifiedPosition(UDim2::POSITION_BOTTOM_CENTER + sf::Vector2f(0, -100));

    // Test dices
    for (int i = 0; i < 6; ++i)
    {
        ElementSpriteAnimated* diceSprite = m_root->AddChild<ElementSpriteAnimated>();
        diceSprite->SetPosition(i * 90 + 100, 100);
        diceSprite->SetScale(0.15f);
        diceSprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);
        diceSprite->ChangeAnimSet("dice6.animset.xml");
        diceSprite->StartAnimation("roll");
    }
}

void Demo::AppStep(const DeltaTime& dt)
{
}

void Demo::OnButtonClick(EButton button)
{
    if (button == EButton::Roll)
    {
        int total = 0;

        // Test dices
        for (int i = 0; i < 6; ++i)
        {
            int diceResult = GetRandom(1, 6);
            total += diceResult;
        }

        m_textResult->SetText(ToString(total));
    }
}

}   //namespace demoproject

////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "Demo.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Animation/ManagerAnimations.h"
#include "Gugu/Animation/SpriteAnimation.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/System/SystemUtility.h"
#include "Gugu/math/Random.h"

#include <SFML/Window/Clipboard.hpp>

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

    // Background
    ElementSprite* spriteBackground = m_root->AddChild<ElementSprite>();
    spriteBackground->SetTexture("SuchRoll.png");
    spriteBackground->SetUnifiedPosition(UDim2::POSITION_BOTTOM_RIGHT + sf::Vector2f(100.f, 20.f));
    spriteBackground->SetScale(0.5f);
    spriteBackground->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_RIGHT);
    spriteBackground->SetColor(sf::Color(255,255,255,255));
    m_spriteBackground = spriteBackground;

    SetupStandard();
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::SetupStandard()
{
    sf::Vector2f buttonSize = sf::Vector2f(120, 40);
    sf::Vector2f diceButtonPosition = sf::Vector2f(10, 10);
    sf::Vector2f diceButtonOffset = sf::Vector2f(0, 45);

    // Clear button
    ElementButton* buttonClear = m_root->AddChild<ElementButton>();
    buttonClear->SetTexture("cadre.png", "cadre2.png");
    buttonClear->GetElementText()->SetFontSize(30);
    buttonClear->SetText("Clear");
    buttonClear->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonClear->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + diceButtonPosition);
    buttonClear->SetSize(buttonSize);
    buttonClear->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::Clear));

    diceButtonPosition += sf::Vector2f(0, 65);

    // Dice 4 button
    ElementButton* buttonAddDice4 = m_root->AddChild<ElementButton>();
    buttonAddDice4->SetTexture("cadre.png", "cadre2.png");
    buttonAddDice4->GetElementText()->SetFontSize(30);
    buttonAddDice4->SetText("+ 1d4");
    buttonAddDice4->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonAddDice4->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + diceButtonPosition);
    buttonAddDice4->SetSize(buttonSize);
    buttonAddDice4->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::AddDice4));

    diceButtonPosition += diceButtonOffset;

    // Dice 6 button
    ElementButton* buttonAddDice6 = m_root->AddChild<ElementButton>();
    buttonAddDice6->SetTexture("cadre.png", "cadre2.png");
    buttonAddDice6->GetElementText()->SetFontSize(30);
    buttonAddDice6->SetText("+ 1d6");
    buttonAddDice6->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonAddDice6->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + diceButtonPosition);
    buttonAddDice6->SetSize(buttonSize);
    buttonAddDice6->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::AddDice6));

    diceButtonPosition += diceButtonOffset;

    // Dice 8 button
    ElementButton* buttonAddDice8 = m_root->AddChild<ElementButton>();
    buttonAddDice8->SetTexture("cadre.png", "cadre2.png");
    buttonAddDice8->GetElementText()->SetFontSize(30);
    buttonAddDice8->SetText("+ 1d8");
    buttonAddDice8->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonAddDice8->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + diceButtonPosition);
    buttonAddDice8->SetSize(buttonSize);
    buttonAddDice8->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::AddDice8));

    diceButtonPosition += diceButtonOffset;

    // Dice 10 button
    ElementButton* buttonAddDice10 = m_root->AddChild<ElementButton>();
    buttonAddDice10->SetTexture("cadre.png", "cadre2.png");
    buttonAddDice10->GetElementText()->SetFontSize(30);
    buttonAddDice10->SetText("+ 1d10");
    buttonAddDice10->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonAddDice10->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + diceButtonPosition);
    buttonAddDice10->SetSize(buttonSize);
    buttonAddDice10->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::AddDice10));

    diceButtonPosition += diceButtonOffset;

    // Dice 12 button
    ElementButton* buttonAddDice12 = m_root->AddChild<ElementButton>();
    buttonAddDice12->SetTexture("cadre.png", "cadre2.png");
    buttonAddDice12->GetElementText()->SetFontSize(30);
    buttonAddDice12->SetText("+ 1d12");
    buttonAddDice12->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonAddDice12->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + diceButtonPosition);
    buttonAddDice12->SetSize(buttonSize);
    buttonAddDice12->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::AddDice12));

    diceButtonPosition += diceButtonOffset;

    // Dice 20 button
    ElementButton* buttonAddDice20 = m_root->AddChild<ElementButton>();
    buttonAddDice20->SetTexture("cadre.png", "cadre2.png");
    buttonAddDice20->GetElementText()->SetFontSize(30);
    buttonAddDice20->SetText("+ 1d20");
    buttonAddDice20->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonAddDice20->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + diceButtonPosition);
    buttonAddDice20->SetSize(buttonSize);
    buttonAddDice20->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::AddDice20));

    diceButtonPosition += diceButtonOffset;

    // Dice 100 button
    ElementButton* buttonAddDice100 = m_root->AddChild<ElementButton>();
    buttonAddDice100->SetTexture("cadre.png", "cadre2.png");
    buttonAddDice100->GetElementText()->SetFontSize(30);
    buttonAddDice100->SetText("+ 1d100");
    buttonAddDice100->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonAddDice100->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + diceButtonPosition);
    buttonAddDice100->SetSize(buttonSize);
    buttonAddDice100->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::AddDice100));

    diceButtonPosition += sf::Vector2f(0, 65);

    // Remove dice
    ElementButton* buttonRemoveDice = m_root->AddChild<ElementButton>();
    buttonRemoveDice->SetTexture("cadre.png", "cadre2.png");
    buttonRemoveDice->GetElementText()->SetFontSize(30);
    buttonRemoveDice->SetText("- 1d");
    buttonRemoveDice->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonRemoveDice->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + diceButtonPosition);
    buttonRemoveDice->SetSize(buttonSize);
    buttonRemoveDice->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::RemoveDice));

    // Roll button
    ElementButton* buttonRoll = m_root->AddChild<ElementButton>();
    buttonRoll->SetTexture("cadre.png", "cadre2.png");
    buttonRoll->GetElementText()->SetFontSize(30);
    buttonRoll->SetText("Roll !");
    buttonRoll->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    buttonRoll->SetUnifiedPosition(UDim2(0.f, 300, 1.f, -20));
    buttonRoll->SetSize(buttonSize);
    buttonRoll->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::Roll));

    // Result
    m_textResult = m_root->AddChild<ElementText>();
    m_textResult->SetFontSize(40);
    m_textResult->SetText("");
    m_textResult->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    m_textResult->SetUnifiedPosition(UDim2(0.f, 300, 1.f, -100));
}

void Demo::ClearDices()
{
    for (int i = 0; i < (int)m_currentDices.size(); ++i)
    {
        ClearSingleDice(i);
    }

    m_currentDices.clear();

    m_textResult->SetText("");
}

void Demo::ClearSingleDice(int index)
{
    SafeDelete(m_currentDices[index].sprite);
    SafeDelete(m_currentDices[index].resultText);
    SafeDelete(m_currentDices[index].buttonReroll);
}

void Demo::AddDice(EDiceType type)
{
    int index = m_currentDices.size();

    Dice dice;
    dice.type = type;

    std::string animset = "dice6.animset.xml";

    if (type == EDiceType::d4)
    {
        animset = "dice4.animset.xml";
    }
    else if (type == EDiceType::d8)
    {
        animset = "dice8.animset.xml";
    }
    else if (type == EDiceType::d10)
    {
        animset = "dice10.animset.xml";
    }
    else if (type == EDiceType::d12)
    {
        animset = "dice12.animset.xml";
    }
    else if (type == EDiceType::d20)
    {
        animset = "dice20.animset.xml";
    }
    else if (type == EDiceType::d100)
    {
        animset = "dice10.animset.xml";
    }

    sf::Vector2f basePosition = sf::Vector2f(index * 95.f + 220.f, 50.f);

    ElementSprite* sprite = m_root->AddChild<ElementSprite>();
    sprite->SetPosition(basePosition);
    sprite->SetScale(0.6f);
    sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    dice.sprite = sprite;

    SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
    animation->ChangeAnimSet(animset);
    animation->StartAnimation("idle");
    dice.animation = animation;

    ElementText* resultText = m_root->AddChild<ElementText>();
    resultText->SetFontSize(40);
    resultText->SetText("");
    resultText->SetPosition(basePosition + sf::Vector2f(0, 70));
    resultText->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    dice.resultText = resultText;

    ElementButton* buttonReroll = m_root->AddChild<ElementButton>();
    buttonReroll->SetTexture("cadre.png", "cadre2.png");
    buttonReroll->GetElementText()->SetFontSize(25);
    buttonReroll->SetText("Reroll");
    buttonReroll->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    buttonReroll->SetPosition(basePosition + sf::Vector2f(0, 120));
    buttonReroll->SetSize(sf::Vector2f(90, 40));
    buttonReroll->SetOnMouseReleased(std::bind(&Demo::OnReroll, this, index));
    buttonReroll->SetVisible(false);
    dice.buttonReroll = buttonReroll;

    m_currentDices.push_back(dice);
}

void Demo::RemoveDice()
{
    if (!m_currentDices.empty())
    {
        int index = m_currentDices.size() - 1;
        ClearSingleDice(index);

        StdVectorRemoveAt(m_currentDices, index);
    }
}

void Demo::PrepareDices(EDiceType type, int count)
{
    ClearDices();

    for (int i = 0; i < count; ++i)
    {
        AddDice(type);
    }
}

void Demo::RollAllDices()
{
    for (int i = 0; i < (int)m_currentDices.size(); ++i)
    {
        RollSingleDice(i, true);
    }

    m_textResult->SetText("");
    m_rolling = true;
}

void Demo::RerollDice(int index)
{
    RollSingleDice(index, false);

    m_textResult->SetText("");
    m_rolling = true;
}

void Demo::RollSingleDice(int index, bool delay)
{
    int min = 1;
    int max = 6;

    if (m_currentDices[index].type == EDiceType::d4)
    {
        max = 4;
    }
    else if (m_currentDices[index].type == EDiceType::d8)
    {
        max = 8;
    }
    else if (m_currentDices[index].type == EDiceType::d10)
    {
        max = 10;
    }
    else if (m_currentDices[index].type == EDiceType::d12)
    {
        max = 12;
    }
    else if (m_currentDices[index].type == EDiceType::d20)
    {
        max = 20;
    }
    else if (m_currentDices[index].type == EDiceType::d100)
    {
        max = 100;
    }

    m_currentDices[index].result = GetRandom(min, max);

    m_currentDices[index].animationTime = (delay) ? 400 + index * 200 : 400;
    m_currentDices[index].animation->StartAnimation("roll");
    m_currentDices[index].buttonReroll->SetVisible(false);
    m_currentDices[index].resultText->SetText("");
}

void Demo::AppUpdate(const DeltaTime& dt)
{
    if (m_rolling)
    {
        bool stillRolling = false;

        for (int i = 0; i < (int)m_currentDices.size(); ++i)
        {
            if (m_currentDices[i].animationTime > 0)
            {
                m_currentDices[i].animationTime -= dt.ms();

                if (m_currentDices[i].animationTime <= 0)
                {
                    m_currentDices[i].animation->StartAnimation("idle");
                    m_currentDices[i].buttonReroll->SetVisible(true);
                    m_currentDices[i].resultText->SetText(ToString(m_currentDices[i].result));
                }
                else
                {
                    stillRolling = true;
                }
            }
        }

        m_rolling = stillRolling;

        if (!m_rolling)
        {
            int total = 0;
            std::string dicesResults = "";
            for (int i = 0; i < (int)m_currentDices.size(); ++i)
            {
                total += m_currentDices[i].result;

                if (i > 0)
                {
                    dicesResults += " | ";
                }
                
                dicesResults += ToString(m_currentDices[i].result);
            }

            m_textResult->SetText(ToString(total));

            sf::Clipboard::setString(StringFormat("Roll score: {0} - dices: {1}", total, dicesResults));

            // Animate background
            if (!m_animatingBackground)
            {
                m_animatingBackground = true;
                m_animationStep = 0;
                m_animationTime = 0;
            }
        }
    }

    if (m_animatingBackground)
    {
        m_animationTime += dt.ms();

        int warmupTime = 300;
        int blinkTime = 250;

        if (m_animationStep == 0 && m_animationTime >= warmupTime)
        {
            m_spriteBackground->SetTexture("SuchRoll2.png");
            ++m_animationStep;
        }

        if (m_animationStep == 1 && m_animationTime >= warmupTime + blinkTime)
        {
            m_spriteBackground->SetTexture("SuchRoll.png");
            ++m_animationStep;
        }

        if (m_animationStep == 2)
        {
            m_animatingBackground = false;
            m_animationStep = 0;
            m_animationTime = 0;
        }
    }
}

void Demo::OnButtonClick(EButton button)
{
    if (button == EButton::Roll)
    {
        RollAllDices();
    }
    else if (button == EButton::Clear)
    {
        ClearDices();
    }
    else if (button == EButton::AddDice4)
    {
        AddDice(EDiceType::d4);
    }
    else if (button == EButton::AddDice6)
    {
        AddDice(EDiceType::d6);
    }
    else if (button == EButton::AddDice8)
    {
        AddDice(EDiceType::d8);
    }
    else if (button == EButton::AddDice10)
    {
        AddDice(EDiceType::d10);
    }
    else if (button == EButton::AddDice12)
    {
        AddDice(EDiceType::d12);
    }
    else if (button == EButton::AddDice20)
    {
        AddDice(EDiceType::d20);
    }
    else if (button == EButton::AddDice100)
    {
        AddDice(EDiceType::d100);
    }
    else if (button == EButton::RemoveDice)
    {
        RemoveDice();
    }
}

void Demo::OnReroll(int index)
{
    RerollDice(index);
}

}   //namespace demoproject

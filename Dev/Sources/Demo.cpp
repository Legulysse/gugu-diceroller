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

    SetupStandard();
}

void Demo::AppStop()
{
    SafeDelete(m_root);
}

void Demo::SetupStandard()
{
    sf::Vector2f buttonSize = sf::Vector2f(120, 40);
    sf::Vector2f buttonOffset = sf::Vector2f(10, 10);

    // Clear button
    ElementButton* buttonClear = m_root->AddChild<ElementButton>();
    buttonClear->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    buttonClear->GetElementText()->SetFontSize(30);
    buttonClear->SetText("Clear");
    buttonClear->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonClear->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + buttonOffset);
    buttonClear->SetSize(buttonSize);
    buttonClear->SetOnMouseReleased(new ActionClass1P<Demo, EButton>(this, &Demo::OnButtonClick, EButton::Clear));

    // Dice 6 button
    ElementButton* buttonAddDice6 = m_root->AddChild<ElementButton>();
    buttonAddDice6->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    buttonAddDice6->GetElementText()->SetFontSize(30);
    buttonAddDice6->SetText("+ 1d6");
    buttonAddDice6->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonAddDice6->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + buttonOffset + sf::Vector2f(150, 0));
    buttonAddDice6->SetSize(buttonSize);
    buttonAddDice6->SetOnMouseReleased(new ActionClass1P<Demo, EButton>(this, &Demo::OnButtonClick, EButton::AddDice6));

    // Dice 12 button
    ElementButton* buttonAddDice12 = m_root->AddChild<ElementButton>();
    buttonAddDice12->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    buttonAddDice12->GetElementText()->SetFontSize(30);
    buttonAddDice12->SetText("+ 1d12");
    buttonAddDice12->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonAddDice12->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + buttonOffset + sf::Vector2f(300, 0));
    buttonAddDice12->SetSize(buttonSize);
    buttonAddDice12->SetOnMouseReleased(new ActionClass1P<Demo, EButton>(this, &Demo::OnButtonClick, EButton::AddDice12));

    // Dice 20 button
    ElementButton* buttonAddDice20 = m_root->AddChild<ElementButton>();
    buttonAddDice20->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    buttonAddDice20->GetElementText()->SetFontSize(30);
    buttonAddDice20->SetText("+ 1d20");
    buttonAddDice20->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonAddDice20->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + buttonOffset + sf::Vector2f(450, 0));
    buttonAddDice20->SetSize(buttonSize);
    buttonAddDice20->SetOnMouseReleased(new ActionClass1P<Demo, EButton>(this, &Demo::OnButtonClick, EButton::AddDice20));

    // Remove dice
    ElementButton* buttonRemoveDice = m_root->AddChild<ElementButton>();
    buttonRemoveDice->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    buttonRemoveDice->GetElementText()->SetFontSize(30);
    buttonRemoveDice->SetText("- 1d");
    buttonRemoveDice->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
    buttonRemoveDice->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + buttonOffset + sf::Vector2f(600, 0));
    buttonRemoveDice->SetSize(buttonSize);
    buttonRemoveDice->SetOnMouseReleased(new ActionClass1P<Demo, EButton>(this, &Demo::OnButtonClick, EButton::RemoveDice));

    // Roll button
    ElementButton* buttonRoll = m_root->AddChild<ElementButton>();
    buttonRoll->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    buttonRoll->GetElementText()->SetFontSize(30);
    buttonRoll->SetText("Roll !");
    buttonRoll->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    buttonRoll->SetUnifiedPosition(UDim2(0.f, 200.f, 1.f, -20.f));
    buttonRoll->SetSize(buttonSize);
    buttonRoll->SetOnMouseReleased(new ActionClass1P<Demo, EButton>(this, &Demo::OnButtonClick, EButton::Roll));

    // Result
    m_textResult = m_root->AddChild<ElementText>();
    m_textResult->SetFontSize(40);
    m_textResult->SetText("");
    m_textResult->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    m_textResult->SetUnifiedPosition(UDim2(0.f, 200.f, 1.f, -100.f));
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
    if (type == EDiceType::d12)
    {
        animset = "dice12.animset.xml";
    }
    else if (type == EDiceType::d20)
    {
        animset = "dice20.animset.xml";
    }

    ElementSpriteAnimated* sprite = m_root->AddChild<ElementSpriteAnimated>();
    sprite->SetPosition(index * 95.f + 50.f, 120.f);
    sprite->SetScale(0.6f);
    sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    sprite->ChangeAnimSet(animset);
    sprite->StartAnimation("idle");
    dice.sprite = sprite;

    ElementText* resultText = m_root->AddChild<ElementText>();
    resultText->SetFontSize(40);
    resultText->SetText("");
    resultText->SetPosition(index * 95.f + 50.f, 200.f);
    resultText->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    dice.resultText = resultText;

    ElementButton* buttonReroll = m_root->AddChild<ElementButton>();
    buttonReroll->SetTexture("ButtonNormal.png", "ButtonFocused.png");
    buttonReroll->GetElementText()->SetFontSize(25);
    buttonReroll->SetText("Reroll");
    buttonReroll->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    buttonReroll->SetPosition(index * 95.f + 50.f, 250.f);
    buttonReroll->SetSize(sf::Vector2f(90.f, 40.f));
    buttonReroll->SetOnMouseReleased(new ActionClass1P<Demo, int>(this, &Demo::OnReroll, index));
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
    int max = 6;

    if (m_currentDices[index].type == EDiceType::d12)
    {
        max = 12;
    }
    else if (m_currentDices[index].type == EDiceType::d20)
    {
        max = 20;
    }

    m_currentDices[index].result = GetRandom(1, max);

    m_currentDices[index].animationTime = (delay) ? 400 + index * 200 : 400;
    m_currentDices[index].sprite->StartAnimation("roll");
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
                    m_currentDices[i].sprite->StartAnimation("idle");
                    m_currentDices[i].resultText->SetText(ToString(m_currentDices[i].result));
                    m_currentDices[i].buttonReroll->SetVisible(true);
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
    else if (button == EButton::AddDice6)
    {
        AddDice(EDiceType::d6);
    }
    else if (button == EButton::AddDice12)
    {
        AddDice(EDiceType::d12);
    }
    else if (button == EButton::AddDice20)
    {
        AddDice(EDiceType::d20);
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

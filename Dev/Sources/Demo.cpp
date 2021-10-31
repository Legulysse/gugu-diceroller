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
    sf::Vector2f buttonSize = sf::Vector2f(160, 40);
    sf::Vector2f diceButtonPosition = sf::Vector2f(10, 10);
    sf::Vector2f diceButtonOffset = sf::Vector2f(0, buttonSize.y + 5);

    uint32 fontSize = 26;
    UDim2 alignmentText = UDim2::POSITION_TOP_LEFT;
    Vector2f offsetText = Vector2f(10.f, 0.f);
    UDim2 positionIcon = UDim2::POSITION_CENTER_RIGHT + Vector2f(-25.f, 0.f);
    float scaleDiceIcon = 0.2f;

    auto createButton = [&](const std::string& text, const std::string& icon, float iconScale, EButton buttonID)
    {
        ElementButton* button = m_root->AddChild<ElementButton>();
        button->SetTexture("cadre.png", "cadre2.png");
        button->GetElementText()->SetFontSize(fontSize);
        button->SetText(text);
        button->SetTextAlignment(alignmentText, offsetText);
        button->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
        button->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + diceButtonPosition);
        button->SetSize(buttonSize);
        button->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, buttonID));

        if (icon != "")
        {
            DecorateButton(button, icon, positionIcon, iconScale);
        }
    };

    auto createDiceButton = [&](const std::string& text, const std::string& icon, EButton buttonID)
    {
        createButton(text, icon, scaleDiceIcon, buttonID);
    };

    // Separator
    ElementSprite* separator = m_root->AddChild<ElementSprite>();
    separator->SetTexture("Separator.png");
    separator->SetPosition(180.f, 0.f);
    separator->SetUnifiedSize(UDim2(0.f, 4.f, 1.f, 0.f));

    // Dice buttons
    createButton("Clear", "clear.png", 0.3f, EButton::Clear);
    diceButtonPosition += sf::Vector2f(0, 65);

    createDiceButton("+ 1d4", "dice4_00012.png", EButton::AddDice4);
    diceButtonPosition += diceButtonOffset;

    createDiceButton("+ 1d6", "dice6_00012.png", EButton::AddDice6);
    diceButtonPosition += diceButtonOffset;

    createDiceButton("+ 1d8", "dice8_00012.png", EButton::AddDice8);
    diceButtonPosition += diceButtonOffset;

    createDiceButton("+ 1d10", "dice10_00012.png", EButton::AddDice10);
    diceButtonPosition += diceButtonOffset;

    createDiceButton("+ 1d12", "dice12_00012.png", EButton::AddDice12);
    diceButtonPosition += diceButtonOffset;

    createDiceButton("+ 1d20", "dice20_00012.png", EButton::AddDice20);
    diceButtonPosition += diceButtonOffset;

    createDiceButton("+ 1d100", "dice10_00012.png", EButton::AddDice100);
    diceButtonPosition += sf::Vector2f(0, 65);

    createDiceButton("- 1d", "", EButton::RemoveDice);
    diceButtonPosition += diceButtonOffset;

    // Roll button
    ElementButton* buttonRoll = m_root->AddChild<ElementButton>();
    buttonRoll->SetTexture("cadre.png", "cadre2.png");
    buttonRoll->GetElementText()->SetFontSize(fontSize);
    buttonRoll->SetText("Roll !");
    buttonRoll->SetTextAlignment(alignmentText, offsetText);
    buttonRoll->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    buttonRoll->SetUnifiedPosition(UDim2(0.f, 300, 1.f, -20));
    buttonRoll->SetSize(buttonSize);
    buttonRoll->SetOnMouseReleased(std::bind(&Demo::OnButtonClick, this, EButton::Roll));
    DecorateButton(buttonRoll, "roll.png", positionIcon, 0.3f);

    // Result
    m_textResult = m_root->AddChild<ElementText>();
    m_textResult->SetFontSize(40);
    m_textResult->SetText("");
    m_textResult->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    m_textResult->SetUnifiedPosition(UDim2(0.f, 300, 1.f, -100));
}

void Demo::DecorateButton(gugu::Element* button, const std::string& textureID, const UDim2& position, float scale)
{
    DecorateButton(button, textureID, position, UDim2::POSITION_CENTER, scale);
}

void Demo::DecorateButton(gugu::Element* button, const std::string& textureID, const UDim2& position, const UDim2& origin, float scale)
{
    ElementSprite* decoration = button->AddChild<ElementSprite>();
    decoration->SetTexture(textureID);
    decoration->SetUnifiedOrigin(origin);
    decoration->SetUnifiedPosition(position);
    decoration->SetScale(scale);
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
    SafeDelete(m_currentDices[index].pivot);
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

    sf::Vector2f basePosition = sf::Vector2f(index * 95.f + 240.f, 50.f);

    Element* pivot = m_root->AddChild<Element>();
    dice.pivot = pivot;

    ElementSprite* sprite = pivot->AddChild<ElementSprite>();
    sprite->SetPosition(basePosition);
    sprite->SetScale(0.6f);
    sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    dice.sprite = sprite;

    SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
    animation->ChangeAnimSet(animset);
    animation->StartAnimation("idle");
    dice.animation = animation;

    ElementText* resultText = pivot->AddChild<ElementText>();
    resultText->SetFontSize(40);
    resultText->SetText("");
    resultText->SetPosition(basePosition + sf::Vector2f(0, 70));
    resultText->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    dice.resultText = resultText;

    ElementSprite* interaction = pivot->AddChild<ElementSprite>();
    interaction->SetPosition(basePosition);
    interaction->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    interaction->SetSize(70.f, 80.f);
    interaction->InitInteractions();
    interaction->AddInteractionFlag(EInteraction::Click);
    interaction->GetInteractions()->AddCallback(EInteraction::Click, std::bind(&Demo::OnReroll, this, index));
    DecorateButton(interaction, "reroll.png", UDim2::POSITION_BOTTOM_RIGHT + Vector2f(10.f, 10.f), UDim2::POSITION_BOTTOM_RIGHT, 0.15f);
    dice.buttonReroll = interaction;

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
                    dicesResults += " ";
                }
                
                dicesResults += ToString(m_currentDices[i].result);
            }

            m_textResult->SetText(ToString(total));

            sf::Clipboard::setString(StringFormat("dices: {1} - score: {0}", total, dicesResults));

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

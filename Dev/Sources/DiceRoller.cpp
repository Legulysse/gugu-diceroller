////////////////////////////////////////////////////////////////
// Header

#include "Gugu/Common.h"
#include "DiceRoller.h"

////////////////////////////////////////////////////////////////
// Includes

#include "Gugu/Window/Window.h"
#include "Gugu/Resources/ManagerResources.h"
#include "Gugu/Resources/Texture.h"
#include "Gugu/Animation/ManagerAnimations.h"
#include "Gugu/Animation/SpriteAnimation.h"
#include "Gugu/Element/2D/ElementSprite.h"
#include "Gugu/Element/2D/ElementSpriteGroup.h"
#include "Gugu/Element/2D/ElementSFDrawable.h"
#include "Gugu/Element/2D/ElementText.h"
#include "Gugu/Element/UI/ElementButton.h"
#include "Gugu/Events/ElementEventHandler.h"
#include "Gugu/System/Container.h"
#include "Gugu/System/Memory.h"
#include "Gugu/System/String.h"
#include "Gugu/Math/MathUtility.h"
#include "Gugu/Math/Random.h"

#include <SFML/Window/Clipboard.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

using namespace gugu;

////////////////////////////////////////////////////////////////
// File Implementation

namespace project {
    
DiceRoller::DiceRoller()
: m_root(nullptr)
{
}

DiceRoller::~DiceRoller()
{
}

void DiceRoller::AppStart()
{
    RegisterEventHandler(GetGameWindow());

    // Setup textures if needed
    Texture* textureBox9 = GetResources()->GetTexture("Box9_Default_Black.png");
    if (textureBox9)
    {
        textureBox9->SetSmooth(false);
    }

    Texture* textureButton = GetResources()->GetTexture("Button01.png");
    if (textureButton)
    {
        textureButton->SetSmooth(false);
    }

    // Root
    m_root = GetGameWindow()->GetUINode()->AddChild<Element>();
    m_root->SetUnifiedSize(UDim2(1.f, 1.f));

    // Background
    ElementSprite* spriteBackground = m_root->AddChild<ElementSprite>();
    spriteBackground->SetTexture("SuchRoll.png");
    spriteBackground->SetUnifiedPosition(UDim2::POSITION_BOTTOM_RIGHT + sf::Vector2f(100.f, 20.f));
    spriteBackground->SetScale(0.4f);
    spriteBackground->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_RIGHT);
    spriteBackground->SetColor(sf::Color(255,255,255,255));
    m_spriteBackground = spriteBackground;

    // Separator
    sf::RectangleShape* separatorShape = new sf::RectangleShape;
    separatorShape->setSize(Vector2f(150.f, 150.f));
    separatorShape->setFillColor(sf::Color(128, 128, 128, 255));

    ElementSFDrawable* separator = m_root->AddChild<ElementSFDrawable>();
    separator->SetSFDrawable(separatorShape);
    separator->SetPosition(0.f, 0.f);
    separator->SetCallbackOnSizeChanged([separatorShape](ElementSFDrawable* element)
    {
        separatorShape->setSize(element->GetSize());
    });
    separator->SetUnifiedSize(UDim2(Vector2f(0.f, 1.f), Vector2f(180.f, 0.f)));

    //ElementSprite* separator = m_root->AddChild<ElementSprite>();
    //separator->SetTexture("Separator.png");
    //separator->SetPosition(180.f, 0.f);
    //separator->SetUnifiedSize(UDim2(0.f, 4.f, 1.f, 0.f));

    // Version
    ElementText* textVersion = m_root->AddChild<ElementText>();
    textVersion->SetText("Version 1.1");
    textVersion->SetFontSize(15);
    textVersion->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    textVersion->SetUnifiedPosition(UDim2::POSITION_BOTTOM_LEFT + sf::Vector2f(5.f, -5.f));

    //ElementText* textWeb = m_root->AddChild<ElementText>();
    //textWeb->SetText("https://github.com/Legulysse/gugu-diceroller");
    //textWeb->SetFontSize(15);
    //textWeb->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_RIGHT);
    //textWeb->SetUnifiedPosition(UDim2::POSITION_BOTTOM_RIGHT + sf::Vector2f(-5.f, -5.f));

    SetupStandard();
}

void DiceRoller::AppStop()
{
    SafeDelete(m_root);
}

void DiceRoller::SetupStandard()
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
        button->LoadFromFile("Button01.xml");
        button->GetElementText()->SetFontSize(fontSize);
        button->SetText(text);
        button->SetTextAlignment(alignmentText, offsetText);
        button->SetUnifiedOrigin(UDim2::POSITION_TOP_LEFT);
        button->SetUnifiedPosition(UDim2::POSITION_TOP_LEFT + diceButtonPosition);
        button->SetSize(buttonSize);
        button->SetOnMouseReleased(std::bind(&DiceRoller::OnButtonClick, this, buttonID));

        if (icon != "")
        {
            DecorateButton(button, icon, positionIcon, iconScale);
        }
    };

    auto createDiceButton = [&](const std::string& text, const std::string& icon, EButton buttonID)
    {
        createButton(text, icon, scaleDiceIcon, buttonID);
    };

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
    m_pivotRoll = m_root->AddChild<Element>();
    m_pivotRoll->SetPosition(240.f, 380.f);

    ElementButton* buttonRoll = m_pivotRoll->AddChild<ElementButton>();
    buttonRoll->LoadFromFile("Button01.xml");
    buttonRoll->GetElementText()->SetFontSize(fontSize);
    buttonRoll->SetText("Roll !");
    buttonRoll->SetTextAlignment(alignmentText, offsetText);
    buttonRoll->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_LEFT);
    //buttonRoll->SetPosition(0.f, -50.f);
    buttonRoll->SetSize(buttonSize);
    buttonRoll->SetOnMouseReleased(std::bind(&DiceRoller::OnButtonClick, this, EButton::Roll));
    DecorateButton(buttonRoll, "roll.png", positionIcon, 0.3f);

    // Result
    ElementSpriteGroup* boxResult = buttonRoll->AddChild<ElementSpriteGroup>();
    boxResult->SetColor(sf::Color(255, 255, 255, 30));
    boxResult->LoadFromFile("Box9_02_Black.xml");
    boxResult->SetSize(130.f, 50.f);
    boxResult->SetUnifiedOrigin(UDim2::POSITION_BOTTOM_CENTER);
    boxResult->SetUnifiedPosition(UDim2::POSITION_TOP_CENTER + Vector2f(0.f, -15.f));
    //boxResult->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
    //boxResult->SetUnifiedPosition(UDim2::POSITION_BOTTOM_CENTER + Vector2f(0.f, 15.f));

    m_textResult = boxResult->AddChild<ElementText>();
    m_textResult->SetFontSize(40);
    m_textResult->SetText("");
    m_textResult->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
    m_textResult->SetUnifiedPosition(UDim2::POSITION_TOP_CENTER);
}

void DiceRoller::DecorateButton(gugu::Element* button, const std::string& textureID, const UDim2& position, float scale)
{
    DecorateButton(button, textureID, position, UDim2::POSITION_CENTER, scale);
}

void DiceRoller::DecorateButton(gugu::Element* button, const std::string& textureID, const UDim2& position, const UDim2& origin, float scale)
{
    ElementSprite* decoration = button->AddChild<ElementSprite>();
    decoration->SetTexture(textureID);
    decoration->SetUnifiedOrigin(origin);
    decoration->SetUnifiedPosition(position);
    decoration->SetScale(scale);
}

void DiceRoller::ClearDices()
{
    for (size_t i = 0; i < m_currentDices.size(); ++i)
    {
        ClearSingleDice(i);
    }

    m_currentDices.clear();

    m_textResult->SetText("");
}

void DiceRoller::ClearSingleDice(size_t index)
{
    SafeDelete(m_currentDices[index].pivot);
}

void DiceRoller::AddDice(EDiceType type)
{
    size_t index = m_currentDices.size();

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

    Element* pivot = m_root->AddChild<Element>();
    pivot->SetPosition(sf::Vector2f(((int)index) * 95.f + 240.f, 50.f));
    dice.pivot = pivot;

    ElementSprite* sprite = pivot->AddChild<ElementSprite>();
    sprite->SetScale(0.6f);
    sprite->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    dice.sprite = sprite;

    SpriteAnimation* animation = GetAnimations()->AddAnimation(sprite);
    animation->ChangeAnimSet(animset);
    animation->StartAnimation("idle");
    dice.animation = animation;

    ElementSpriteGroup* boxResult = pivot->AddChild<ElementSpriteGroup>();
    boxResult->SetColor(sf::Color(255, 255, 255, 30));
    boxResult->LoadFromFile("Box9_02_Black.xml");
    boxResult->SetSize(80.f, 50.f);
    boxResult->SetPosition(0, 60);
    boxResult->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);

    ElementText* resultText = boxResult->AddChild<ElementText>();
    resultText->SetFontSize(40);
    resultText->SetText("");
    resultText->SetUnifiedOrigin(UDim2::POSITION_TOP_CENTER);
    resultText->SetUnifiedPosition(UDim2::POSITION_TOP_CENTER);
    dice.resultText = resultText;

    ElementSprite* interaction = pivot->AddChild<ElementSprite>();
    interaction->SetUnifiedOrigin(UDim2::POSITION_CENTER);
    interaction->SetSize(70.f, 80.f);
    interaction->GetEvents()->AddCallback(EInteractionEvent::MousePressed, std::bind(&DiceRoller::OnReroll, this, index));

    DecorateButton(interaction, "reroll.png", UDim2::POSITION_BOTTOM_RIGHT + Vector2f(10.f, 10.f), UDim2::POSITION_BOTTOM_RIGHT, 0.15f);
    dice.buttonReroll = interaction;

    m_currentDices.push_back(dice);
}

void DiceRoller::RemoveDice()
{
    if (!m_currentDices.empty())
    {
        size_t index = m_currentDices.size() - 1;
        ClearSingleDice(index);

        StdVectorRemoveAt(m_currentDices, index);
    }
}

void DiceRoller::PrepareDices(EDiceType type, size_t count)
{
    ClearDices();

    for (size_t i = 0; i < count; ++i)
    {
        AddDice(type);
    }
}

void DiceRoller::RollAllDices()
{
    for (size_t i = 0; i < m_currentDices.size(); ++i)
    {
        RollSingleDice(i, true);
    }

    m_textResult->SetText("");
    m_rolling = true;
}

void DiceRoller::RerollDice(size_t index)
{
    RollSingleDice(index, false);

    m_textResult->SetText("");
    m_rolling = true;
}

void DiceRoller::RollSingleDice(size_t index, bool delay)
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

    m_currentDices[index].animationTime = (delay) ? 0.4f + (int)index * 0.2f : 0.4f;
    m_currentDices[index].animation->StartAnimation("roll");
    m_currentDices[index].buttonReroll->SetVisible(false);
    m_currentDices[index].resultText->SetText("");
}

void DiceRoller::AppUpdate(const DeltaTime& dt)
{
    bool moveDices = true;
    if (moveDices)
    {
        Vector2f basePosition = Vector2f(240.f, 50.f);
        float rowOffset = 180.f;
        float diceOffset = 95.f;

        int test = GetGameWindow()->GetSize().x;
        int nbDicesPerRow = ((int)GetGameWindow()->GetSize().x - (int)basePosition.x + (int)(diceOffset / 2)) / (int)diceOffset;
        nbDicesPerRow = Max(1, nbDicesPerRow);

        int nbRows = ((int)m_currentDices.size() / nbDicesPerRow) + ((int)m_currentDices.size() % nbDicesPerRow != 0 ? 1 : 0);

        for (size_t i = 0; i < m_currentDices.size(); ++i)
        {
            Vector2f position = basePosition + Vector2f((i % nbDicesPerRow) * diceOffset, (i / nbDicesPerRow) * rowOffset);
            m_currentDices[i].pivot->SetPosition(position);
        }

        if (nbRows <= 1)
        {
            Vector2f basePositionRoll = Vector2f(240.f, 380.f);
            m_pivotRoll->SetPosition(basePositionRoll);
        }
        else
        {
            Vector2f basePositionRoll = Vector2f(240.f, 120.f);
            m_pivotRoll->SetPosition(basePositionRoll + Vector2f(0.f, nbRows * rowOffset));
        }
    }

    if (m_rolling)
    {
        bool stillRolling = false;

        for (size_t i = 0; i < m_currentDices.size(); ++i)
        {
            if (m_currentDices[i].animationTime > 0.f)
            {
                m_currentDices[i].animationTime -= dt.s();

                if (m_currentDices[i].animationTime <= 0.f)
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
            for (size_t i = 0; i < m_currentDices.size(); ++i)
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
                m_animationTime = 0.f;
            }
        }
    }

    if (m_animatingBackground)
    {
        m_animationTime += dt.s();

        float warmupTime = 0.3f;
        float blinkTime = 0.25f;

        if (m_animationStep == 0 && ApproxSuperiorOrEqual(m_animationTime, warmupTime, math::Epsilon3))
        {
            m_spriteBackground->SetTexture("SuchRoll2.png");
            ++m_animationStep;
        }

        if (m_animationStep == 1 && ApproxSuperiorOrEqual(m_animationTime, warmupTime + blinkTime, math::Epsilon3))
        {
            m_spriteBackground->SetTexture("SuchRoll.png");
            ++m_animationStep;
        }

        if (m_animationStep == 2)
        {
            m_animatingBackground = false;
            m_animationStep = 0;
            m_animationTime = 0.f;
        }
    }
}

void DiceRoller::OnButtonClick(EButton button)
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

void DiceRoller::OnReroll(size_t index)
{
    RerollDice(index);
}

}   //namespace project

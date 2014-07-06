/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus's Graphical User Interface
// Copyright (C) 2012-2014 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Container.hpp>
#include <TGUI/Label.hpp>

#include <SFML/OpenGL.hpp>

#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Label::Label()
    {
        m_callback.widgetType = Type_Label;

        m_background.setFillColor(sf::Color::Transparent);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Label::Ptr Label::create(const std::string& configFileFilename)
    {
        auto label = std::make_shared<Label>();

        if (!configFileFilename.empty())
        {
            label->m_loadedConfigFile = getResourcePath() + configFileFilename;

            // Open the config file
            ConfigFile configFile{label->m_loadedConfigFile, "Label"};

            // Handle the read properties
            for (auto it = configFile.getProperties().cbegin(); it != configFile.getProperties().cend(); ++it)
            {
                if (it->first == "textcolor")
                    label->setTextColor(extractColor(it->second));
                else
                    throw Exception{"Unrecognized property '" + it->first + "' in section Label in " + label->m_loadedConfigFile + "."};
            }
        }

        return label;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Label::setPosition(const Layout& position)
    {
        Widget::setPosition(position);

        m_background.setPosition(getPosition());
        m_text.setPosition(std::floor(getPosition().x - m_text.getLocalBounds().left + 0.5f),
                           std::floor(getPosition().y - m_text.getLocalBounds().top + 0.5f));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Label::setSize(const Layout& size)
    {
        Widget::setSize(size);

        m_background.setSize(getSize());

        // You are no longer auto-sizing
        m_autoSize = false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Label::setText(const sf::String& string)
    {
        m_text.setString(string);

        // Update the position of the text
        m_text.setPosition(std::floor(getPosition().x - m_text.getLocalBounds().left + 0.5f),
                           std::floor(getPosition().y - m_text.getLocalBounds().top + 0.5f));

        // Change the size of the label if necessary
        if (m_autoSize)
        {
            setSize({m_text.getLocalBounds().width, m_text.getLocalBounds().height});
            m_autoSize = true;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Label::setTextFont(const sf::Font& font)
    {
        m_text.setFont(font);
        setText(getText());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Label::setTextSize(unsigned int size)
    {
        m_text.setCharacterSize(size);

        updatePosition();

        // Change the size of the label if necessary
        if (m_autoSize)
        {
            setSize({m_text.getLocalBounds().width, m_text.getLocalBounds().height});
            m_autoSize = true;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Label::setAutoSize(bool autoSize)
    {
        m_autoSize = autoSize;

        // Change the size of the label if necessary
        if (m_autoSize)
        {
            setSize({m_text.getLocalBounds().width, m_text.getLocalBounds().height});
            m_autoSize = true;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Label::initialize(Container *const parent)
    {
        Widget::initialize(parent);

        if (!getTextFont() && m_parent->getGlobalFont())
            setTextFont(*m_parent->getGlobalFont());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // When there is no text then there is nothing to draw
        if (m_text.getString().isEmpty())
            return;

        if (m_autoSize)
        {
            // Draw the background
            if (m_background.getFillColor() != sf::Color::Transparent)
                target.draw(m_background, states);

            // Draw the text
            target.draw(m_text, states);
        }
        else
        {
            const sf::View& view = target.getView();

            // Calculate the scale factor of the view
            float scaleViewX = target.getSize().x / view.getSize().x;
            float scaleViewY = target.getSize().y / view.getSize().y;

            // Get the global position
            sf::Vector2f topLeftPosition = {((getAbsolutePosition().x - view.getCenter().x + (view.getSize().x / 2.f)) * view.getViewport().width) + (view.getSize().x * view.getViewport().left),
                                            ((getAbsolutePosition().y - view.getCenter().y + (view.getSize().y / 2.f)) * view.getViewport().height) + (view.getSize().y * view.getViewport().top)};
            sf::Vector2f bottomRightPosition = {(getAbsolutePosition().x + getSize().x - view.getCenter().x + (view.getSize().x / 2.f)) * view.getViewport().width + (view.getSize().x * view.getViewport().left),
                                                (getAbsolutePosition().y + getSize().y - view.getCenter().y + (view.getSize().y / 2.f)) * view.getViewport().height + (view.getSize().y * view.getViewport().top)};

            // Get the old clipping area
            GLint scissor[4];
            glGetIntegerv(GL_SCISSOR_BOX, scissor);

            // Calculate the clipping area
            GLint scissorLeft = TGUI_MAXIMUM(static_cast<GLint>(topLeftPosition.x * scaleViewX), scissor[0]);
            GLint scissorTop = TGUI_MAXIMUM(static_cast<GLint>(topLeftPosition.y * scaleViewY), static_cast<GLint>(target.getSize().y) - scissor[1] - scissor[3]);
            GLint scissorRight = TGUI_MINIMUM(static_cast<GLint>(bottomRightPosition.x * scaleViewX), scissor[0] + scissor[2]);
            GLint scissorBottom = TGUI_MINIMUM(static_cast<GLint>(bottomRightPosition.y * scaleViewY), static_cast<GLint>(target.getSize().y) - scissor[1]);

            // If the object outside the window then don't draw anything
            if (scissorRight < scissorLeft)
                scissorRight = scissorLeft;
            else if (scissorBottom < scissorTop)
                scissorTop = scissorBottom;

            // Set the clipping area
            glScissor(scissorLeft, target.getSize().y - scissorBottom, scissorRight - scissorLeft, scissorBottom - scissorTop);

            // Draw the background
            if (m_background.getFillColor() != sf::Color::Transparent)
                target.draw(m_background, states);

            // Draw the text
            target.draw(m_text, states);

            // Reset the old clipping area
            glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

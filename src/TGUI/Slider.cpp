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
#include <TGUI/Slider.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Slider::Slider()
    {
        m_callback.widgetType = Type_Slider;
        m_draggableWidget = true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Slider::Ptr Slider::create(const std::string& configFileFilename)
    {
        auto slider = std::make_shared<Slider>();

        slider->m_loadedConfigFile = getResourcePath() + configFileFilename;

        // Open the config file
        ConfigFile configFile{slider->m_loadedConfigFile, "Slider"};

        // Find the folder that contains the config file
        std::string configFileFolder = "";
        std::string::size_type slashPos = slider->m_loadedConfigFile.find_last_of("/\\");
        if (slashPos != std::string::npos)
            configFileFolder = slider->m_loadedConfigFile.substr(0, slashPos+1);

        // Handle the read properties
        for (auto it = configFile.getProperties().cbegin(); it != configFile.getProperties().cend(); ++it)
        {
            if (it->first == "separatehoverimage")
                slider->m_separateHoverImage = configFile.readBool(it);
            else if (it->first == "verticalimage")
            {
                slider->m_verticalImage = configFile.readBool(it);
                slider->m_verticalScroll = slider->m_verticalImage;
            }
            else if (it->first == "tracknormalimage")
                configFile.readTexture(it, configFileFolder, slider->m_textureTrackNormal);
            else if (it->first == "trackhoverimage")
                configFile.readTexture(it, configFileFolder, slider->m_textureTrackHover);
            else if (it->first == "thumbnormalimage")
                configFile.readTexture(it, configFileFolder, slider->m_textureThumbNormal);
            else if (it->first == "thumbhoverimage")
                configFile.readTexture(it, configFileFolder, slider->m_textureThumbHover);
            else
                throw Exception{"Unrecognized property '" + it->first + "' in section Slider in " + slider->m_loadedConfigFile + "."};
        }

        // Make sure the required textures were loaded
        if ((slider->m_textureTrackNormal.getData() == nullptr) && (slider->m_textureThumbNormal.getData() == nullptr))
            throw Exception{"Not all needed images were loaded for the slider. Is the Slider section in " + slider->m_loadedConfigFile + " complete?"};

        slider->setSize(slider->m_textureTrackNormal.getImageSize());

        return slider;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setPosition(const Layout& position)
    {
        Widget::setPosition(position);

        m_textureTrackNormal.setPosition(position.getValue());
        m_textureTrackHover.setPosition(position.getValue());

        if (m_verticalScroll)
        {
            m_textureThumbNormal.setPosition({position.getValue().x + ((getSize().x - getThumbSize().x) / 2.0f),
                                              position.getValue().y - (getThumbSize().y / 2.0f) + (getSize().y / (m_maximum - m_minimum) * (m_value - m_minimum))});
        }
        else
        {
            m_textureThumbNormal.setPosition({position.getValue().x - (getThumbSize().x / 2.0f) + (getSize().x / (m_maximum - m_minimum) * (m_value - m_minimum)),
                                              position.getValue().y + ((getSize().y - getThumbSize().y) / 2.0f)});
        }

        m_textureThumbHover.setPosition(m_textureThumbNormal.getPosition());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setSize(const Layout& size)
    {
        Widget::setSize(size);

        if (m_verticalImage == m_verticalScroll)
        {
            m_textureTrackNormal.setSize(getSize());

            if (m_verticalScroll)
            {
                m_textureThumbNormal.setSize({getSize().x / m_textureTrackNormal.getImageSize().x * m_textureThumbNormal.getImageSize().x,
                                              getSize().x / m_textureTrackNormal.getImageSize().x * m_textureThumbNormal.getImageSize().y});
            }
            else
            {
                m_textureThumbNormal.setSize({getSize().y / m_textureTrackNormal.getImageSize().y * m_textureThumbNormal.getImageSize().x,
                                              getSize().y / m_textureTrackNormal.getImageSize().y * m_textureThumbNormal.getImageSize().y});
            }
        }
        else
        {
            m_textureTrackNormal.setSize({getSize().y, getSize().x});

            if (m_verticalScroll)
            {
                m_textureThumbNormal.setSize({getSize().x / m_textureTrackNormal.getImageSize().y * m_textureThumbNormal.getImageSize().x,
                                              getSize().x / m_textureTrackNormal.getImageSize().y * m_textureThumbNormal.getImageSize().y});
            }
            else
            {
                m_textureThumbNormal.setSize({getSize().y / m_textureTrackNormal.getImageSize().x * m_textureThumbNormal.getImageSize().x,
                                              getSize().y / m_textureTrackNormal.getImageSize().x * m_textureThumbNormal.getImageSize().y});
            }
        }

        m_textureTrackHover.setSize(m_textureTrackNormal.getSize());
        m_textureThumbHover.setSize(m_textureThumbNormal.getSize());

        // Recalculate the position of the images
        updatePosition();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setMinimum(unsigned int minimum)
    {
        // Set the new minimum
        m_minimum = minimum;

        // The maximum can't be below the minimum
        if (m_maximum < m_minimum)
            m_maximum = m_minimum;

        // When the value is below the minimum then adjust it
        if (m_value < m_minimum)
            setValue(m_minimum);

        // Recalculate the position of the thumb image
        updatePosition();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setMaximum(unsigned int maximum)
    {
        // Set the new maximum
        if (maximum > 0)
            m_maximum = maximum;
        else
            m_maximum = 1;

        // The minimum can't be below the maximum
        if (m_minimum > m_maximum)
            setMinimum(m_maximum);

        // When the value is above the maximum then adjust it
        if (m_value > m_maximum)
            setValue(m_maximum);

        // Recalculate the position of the thumb image
        updatePosition();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setValue(unsigned int value)
    {
        if (m_value != value)
        {
            // Set the new value
            m_value = value;

            // When the value is below the minimum or above the maximum then adjust it
            if (m_value < m_minimum)
                m_value = m_minimum;
            else if (m_value > m_maximum)
                m_value = m_maximum;

            // Add the callback (if the user requested it)
            if (m_callbackFunctions[ValueChanged].empty() == false)
            {
                m_callback.trigger = ValueChanged;
                m_callback.value   = static_cast<int>(m_value);
                addCallback();
            }

            // Recalculate the position of the thumb image
            updatePosition();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setVerticalScroll(bool verticalScroll)
    {
        // Only continue when the value changed
        if (m_verticalScroll != verticalScroll)
        {
            // Change the internal value
            m_verticalScroll = verticalScroll;

            if (m_verticalScroll != m_verticalImage)
            {
                m_textureTrackNormal.setRotation(-90);
                m_textureTrackHover.setRotation(-90);
                m_textureThumbNormal.setRotation(-90);
                m_textureThumbHover.setRotation(-90);
            }
            else
            {
                m_textureTrackNormal.setRotation(0);
                m_textureTrackHover.setRotation(0);
                m_textureThumbNormal.setRotation(0);
                m_textureThumbHover.setRotation(0);
            }

            // Swap the width and height if needed
            sf::Vector2f size = getSize();
            if (m_verticalScroll)
            {
                if (size.x > size.y)
                    setSize({size.y, size.x});
                else
                    setSize(size);
            }
            else // The slider lies horizontal
            {
                if (size.y > size.x)
                    setSize({size.y, size.x});
                else
                    setSize(size);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setTransparency(unsigned char transparency)
    {
        Widget::setTransparency(transparency);

        m_textureTrackNormal.setColor(sf::Color(255, 255, 255, m_opacity));
        m_textureTrackHover.setColor(sf::Color(255, 255, 255, m_opacity));
        m_textureThumbNormal.setColor(sf::Color(255, 255, 255, m_opacity));
        m_textureThumbHover.setColor(sf::Color(255, 255, 255, m_opacity));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Slider::mouseOnWidget(float x, float y)
    {
        // Check if the mouse is on top of the thumb
        if (sf::FloatRect(m_textureThumbNormal.getPosition().x, m_textureThumbNormal.getPosition().y, getThumbSize().x, getThumbSize().y).contains(x, y))
        {
            m_mouseDownOnThumb = true;
            m_mouseDownOnThumbPos.x = x - m_textureThumbNormal.getPosition().x;
            m_mouseDownOnThumbPos.y = y - m_textureThumbNormal.getPosition().y;
            return true;
        }
        else // The mouse is not on top of the thumb
            m_mouseDownOnThumb = false;

        // Check if the mouse is on top of the track
        if (getTransform().transformRect(sf::FloatRect(0, 0, getSize().x, getSize().y)).contains(x, y))
            return true;

        if (m_mouseHover)
            mouseLeftWidget();

        // The mouse is not on top of the slider
        m_mouseHover = false;
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::leftMousePressed(float x, float y)
    {
        m_mouseDown = true;

        // Refresh the value
        mouseMoved(x, y);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::leftMouseReleased(float, float)
    {
        m_mouseDown = false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::mouseMoved(float x, float y)
    {
        if (m_mouseHover == false)
            mouseEnteredWidget();

        m_mouseHover = true;

        // Check if the mouse button is down
        if (m_mouseDown)
        {
            // Check in which direction the slider goes
            if (m_verticalScroll)
            {
                // Check if the click occured on the track
                if (!m_mouseDownOnThumb)
                {
                    m_mouseDownOnThumb = true;
                    m_mouseDownOnThumbPos.x = x - m_textureThumbNormal.getPosition().x;
                    m_mouseDownOnThumbPos.y = getThumbSize().y / 2.0f;
                }

                // Set the new value
                if ((y - m_mouseDownOnThumbPos.y + (getThumbSize().y / 2.0f) - getPosition().y) > 0)
                    setValue(static_cast <unsigned int> ((((y + (getThumbSize().y / 2.0f) - m_mouseDownOnThumbPos.y - getPosition().y) / getSize().y) * (m_maximum - m_minimum)) + m_minimum + 0.5f));
                else
                    setValue(m_minimum);
            }
            else // the slider lies horizontal
            {
                // Check if the click occured on the track
                if (!m_mouseDownOnThumb)
                {
                    m_mouseDownOnThumb = true;
                    m_mouseDownOnThumbPos.x = getThumbSize().x / 2.0f;
                    m_mouseDownOnThumbPos.y = y - m_textureThumbNormal.getPosition().y;
                }

                // Set the new value
                if ((x - m_mouseDownOnThumbPos.x + (getThumbSize().x / 2.0f) - getPosition().x) > 0)
                    setValue(static_cast <unsigned int> ((((x + (getThumbSize().x / 2.0f) - m_mouseDownOnThumbPos.x - getPosition().x) / getSize().x) * (m_maximum - m_minimum)) + m_minimum + 0.5f));
                else
                    setValue(m_minimum);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::mouseWheelMoved(int delta, int, int)
    {
        if (static_cast<int>(m_value) - delta < static_cast<int>(m_minimum))
            setValue(m_minimum);
        else
            setValue(static_cast<unsigned int>(m_value - delta));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::widgetFocused()
    {
        // A slider can't be focused (yet)
        unfocus();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Vector2f Slider::getThumbSize() const
    {
        if (m_verticalImage == m_verticalScroll)
            return m_textureThumbNormal.getSize();
        else
            return {m_textureThumbNormal.getSize().y, m_textureThumbNormal.getSize().x};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (m_separateHoverImage)
        {
            if (m_mouseHover)
            {
                if (m_textureTrackHover.getData())
                    target.draw(m_textureTrackHover, states);

                if (m_textureThumbHover.getData())
                    target.draw(m_textureThumbHover, states);
            }
            else
            {
                target.draw(m_textureTrackNormal, states);
                target.draw(m_textureThumbNormal, states);
            }
        }
        else // The hover image is drawn on top of the normal one
        {
            target.draw(m_textureTrackNormal, states);

            if (m_mouseHover)
            {
                if (m_textureTrackHover.getData())
                    target.draw(m_textureTrackHover, states);

                target.draw(m_textureThumbNormal, states);
                if (m_textureThumbHover.getData())
                    target.draw(m_textureThumbHover, states);
            }
            else
                target.draw(m_textureThumbNormal, states);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

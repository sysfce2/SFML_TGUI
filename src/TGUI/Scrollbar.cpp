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


#include <TGUI/Scrollbar.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Scrollbar::Scrollbar()
    {
        m_callback.widgetType = Type_Scrollbar;
        m_draggableWidget = true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Scrollbar::Ptr Scrollbar::create(const std::string& configFileFilename)
    {
        auto scrollbar = std::make_shared<Scrollbar>();

        scrollbar->m_loadedConfigFile = getResourcePath() + configFileFilename;

        // Open the config file
        ConfigFile configFile{scrollbar->m_loadedConfigFile, "Scrollbar"};

        // Find the folder that contains the config file
        std::string configFileFolder = "";
        std::string::size_type slashPos = scrollbar->m_loadedConfigFile.find_last_of("/\\");
        if (slashPos != std::string::npos)
            configFileFolder = scrollbar->m_loadedConfigFile.substr(0, slashPos+1);

        // Handle the read properties
        for (auto it = configFile.getProperties().cbegin(); it != configFile.getProperties().cend(); ++it)
        {
            if (it->first == "separatehoverimage")
                scrollbar->m_separateHoverImage = configFile.readBool(it);
            else if (it->first == "verticalimage")
            {
                scrollbar->m_verticalImage = configFile.readBool(it);
                scrollbar->m_verticalScroll = scrollbar->m_verticalImage;
            }
            else if (it->first == "tracknormalimage")
                configFile.readTexture(it, configFileFolder, scrollbar->m_textureTrackNormal);
            else if (it->first == "trackhoverimage")
                configFile.readTexture(it, configFileFolder, scrollbar->m_textureTrackHover);
            else if (it->first == "thumbnormalimage")
                configFile.readTexture(it, configFileFolder, scrollbar->m_textureThumbNormal);
            else if (it->first == "thumbhoverimage")
                configFile.readTexture(it, configFileFolder, scrollbar->m_textureThumbHover);
            else if (it->first == "arrowupnormalimage")
                configFile.readTexture(it, configFileFolder, scrollbar->m_textureArrowUpNormal);
            else if (it->first == "arrowuphoverimage")
                configFile.readTexture(it, configFileFolder, scrollbar->m_textureArrowUpHover);
            else if (it->first == "arrowdownnormalimage")
                configFile.readTexture(it, configFileFolder, scrollbar->m_textureArrowDownNormal);
            else if (it->first == "arrowdownhoverimage")
                configFile.readTexture(it, configFileFolder, scrollbar->m_textureArrowDownHover);
            else
                throw Exception{"Unrecognized property '" + it->first + "' in section Scrollbar in " + scrollbar->m_loadedConfigFile + "."};
        }

        // Make sure the required textures were loaded
        if ((scrollbar->m_textureTrackNormal.getData() == nullptr) || (scrollbar->m_textureThumbNormal.getData() == nullptr)
         || (scrollbar->m_textureArrowUpNormal.getData() == nullptr) || (scrollbar->m_textureArrowDownNormal.getData() == nullptr))
            throw Exception{"Not all needed images were loaded for the scrollbar. Is the Scrollbar section in " + scrollbar->m_loadedConfigFile + " complete?"};

        scrollbar->setSize(scrollbar->m_textureTrackNormal.getImageSize());
        scrollbar->setVerticalScroll(scrollbar->m_verticalScroll);

        return scrollbar;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setPosition(const Layout& position)
    {
        Widget::setPosition(position);

        m_textureTrackNormal.setPosition(position.getValue());
        m_textureTrackHover.setPosition(position.getValue());

        m_textureArrowUpNormal.setPosition(position.getValue());
        m_textureArrowUpHover.setPosition(position.getValue());

        if (m_verticalScroll)
        {
            float realTrackHeight = getSize().y;
            realTrackHeight -= m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y;

            m_textureThumbNormal.setPosition(position.getValue().x, position.getValue().y + (realTrackHeight - getThumbSize().y) * m_value / (m_maximum - m_lowValue) + m_textureArrowUpNormal.getSize().y);
            m_textureArrowDownNormal.setPosition(position.getValue().x, position.getValue().y + getSize().y - m_textureArrowUpNormal.getSize().y);
        }
        else
        {
            float realTrackWidth = getSize().x;
            realTrackWidth -= m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y;

            m_textureThumbNormal.setPosition(position.getValue().x + (realTrackWidth - getThumbSize().x) * m_value / (m_maximum - m_lowValue) + m_textureArrowUpNormal.getSize().y, position.getValue().y);
            m_textureArrowDownNormal.setPosition(position.getValue().x + getSize().x - m_textureArrowUpNormal.getSize().x, position.getValue().y);
        }

        m_textureThumbHover.setPosition(m_textureThumbNormal.getPosition());
        m_textureArrowDownHover.setPosition(m_textureArrowDownNormal.getPosition());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setSize(const Layout& size)
    {
        Widget::setSize(size);

        if (m_verticalImage == m_verticalScroll)
            m_textureTrackNormal.setSize(getSize());
        else
            m_textureTrackNormal.setSize({getSize().y, getSize().x});

        float thumbWidth;
        float realTrackSize;
        if (m_verticalScroll)
        {
            m_textureArrowUpNormal.setSize({getSize().x, getSize().x * m_textureArrowUpNormal.getImageSize().x / m_textureArrowUpNormal.getImageSize().y});
            m_textureArrowDownNormal.setSize({getSize().x, getSize().x * m_textureArrowDownNormal.getImageSize().x / m_textureArrowDownNormal.getImageSize().y});

            realTrackSize = getSize().y;
            realTrackSize -= m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y;

            if (m_verticalImage)
                thumbWidth = getSize().x;
            else
                thumbWidth = getSize().y;
        }
        else
        {
            m_textureArrowUpNormal.setSize({getSize().y * m_textureArrowUpNormal.getImageSize().x / m_textureArrowUpNormal.getImageSize().y, getSize().y});
            m_textureArrowDownNormal.setSize({getSize().y * m_textureArrowDownNormal.getImageSize().x / m_textureArrowDownNormal.getImageSize().y, getSize().y});

            realTrackSize = getSize().x;
            realTrackSize -= m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y;

            if (m_verticalImage)
                thumbWidth = getSize().y;
            else
                thumbWidth = getSize().x;
        }

        if (realTrackSize < 0)
            realTrackSize = 0;

        if (m_maximum > m_lowValue)
            m_textureThumbNormal.setSize({thumbWidth, realTrackSize * m_lowValue / m_maximum});
        else
            m_textureThumbNormal.setSize({thumbWidth, realTrackSize});

        m_textureTrackHover.setSize(m_textureTrackNormal.getSize());
        m_textureThumbHover.setSize(m_textureThumbNormal.getSize());
        m_textureArrowUpHover.setSize(m_textureArrowUpNormal.getSize());
        m_textureArrowDownHover.setSize(m_textureArrowUpNormal.getSize());

        // Recalculate the position of the images
        updatePosition();

        // Make sure the transparency isn't lost
        setTransparency(m_opacity);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setMaximum(unsigned int maximum)
    {
        // Set the new maximum
        if (maximum > 0)
            m_maximum = maximum;
        else
            m_maximum = 1;

        // When the value is above the maximum then adjust it
        if (m_maximum < m_lowValue)
            setValue(0);
        else if (m_value > m_maximum - m_lowValue)
            setValue(m_maximum - m_lowValue);

        // Recalculate the size and position of the thumb image
        setSize(getSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setValue(unsigned int value)
    {
        if (m_value != value)
        {
            // Set the new value
            m_value = value;

            // When the value is above the maximum then adjust it
            if (m_maximum < m_lowValue)
                m_value = 0;
            else if (m_value > m_maximum - m_lowValue)
                m_value = m_maximum - m_lowValue;

            // Add the callback (if the user requested it)
            if (m_callbackFunctions[ValueChanged].empty() == false)
            {
                m_callback.trigger = ValueChanged;
                m_callback.value   = static_cast<int>(m_value);
                addCallback();
            }

            // Recalculate the size and position of the thumb image
            setSize(getSize());
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setLowValue(unsigned int lowValue)
    {
        // Set the new value
        m_lowValue = lowValue;

        // When the value is above the maximum then adjust it
        if (m_maximum < m_lowValue)
            setValue(0);
        else if (m_value > m_maximum - m_lowValue)
            setValue(m_maximum - m_lowValue);

        // Recalculate the size and position of the thumb image
        setSize(getSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setVerticalScroll(bool verticalScroll)
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

        if (m_verticalScroll)
        {
            m_textureArrowUpNormal.setRotation(0);
            m_textureArrowUpHover.setRotation(0);
            m_textureArrowDownNormal.setRotation(0);
            m_textureArrowDownHover.setRotation(0);
        }
        else
        {
            m_textureArrowUpNormal.setRotation(-90);
            m_textureArrowUpHover.setRotation(-90);
            m_textureArrowDownNormal.setRotation(-90);
            m_textureArrowDownHover.setRotation(-90);
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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::setTransparency(unsigned char transparency)
    {
        Widget::setTransparency(transparency);

        m_textureTrackNormal.setColor(sf::Color(255, 255, 255, m_opacity));
        m_textureTrackHover.setColor(sf::Color(255, 255, 255, m_opacity));

        m_textureThumbNormal.setColor(sf::Color(255, 255, 255, m_opacity));
        m_textureThumbHover.setColor(sf::Color(255, 255, 255, m_opacity));

        m_textureArrowUpNormal.setColor(sf::Color(255, 255, 255, m_opacity));
        m_textureArrowUpHover.setColor(sf::Color(255, 255, 255, m_opacity));

        m_textureArrowDownNormal.setColor(sf::Color(255, 255, 255, m_opacity));
        m_textureArrowDownHover.setColor(sf::Color(255, 255, 255, m_opacity));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Scrollbar::mouseOnWidget(float x, float y)
    {
        // Don't make any calculations when no scrollbar is needed
        if ((m_maximum <= m_lowValue) && (m_autoHide == true))
            return false;

        // Check if the mouse is on top of the scrollbar
        if (getTransform().transformRect(sf::FloatRect(0, 0, getSize().x, getSize().y)).contains(x, y))
            return true;

        if (m_mouseHover)
            mouseLeftWidget();

        // The mouse is not on top of the scrollbar
        m_mouseHover = false;
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::leftMousePressed(float x, float y)
    {
        m_mouseDown = true;
        m_mouseDownOnArrow = false;

        if (m_verticalScroll)
        {
            // Check if the arrows are drawn at full size
            if (getSize().y > (m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y))
            {
                // Check if you clicked on one of the arrows
                if (y < getPosition().y + m_textureArrowUpNormal.getSize().y)
                    m_mouseDownOnArrow = true;
                else if (y > getPosition().y + getSize().y - m_textureArrowUpNormal.getSize().y)
                    m_mouseDownOnArrow = true;
            }
            else // The arrows are not drawn at full size (there is no track)
                m_mouseDownOnArrow = true;
        }
        else
        {
            // Check if the arrows are drawn at full size
            if (getSize().x > (m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y))
            {
                // Check if you clicked on one of the arrows
                if (x < getPosition().x + m_textureArrowUpNormal.getSize().y)
                    m_mouseDownOnArrow = true;
                else if (x > getPosition().x + getSize().x - m_textureArrowUpNormal.getSize().y)
                    m_mouseDownOnArrow = true;
            }
            else // The arrows are not drawn at full size (there is no track)
                m_mouseDownOnArrow = true;
        }

        // Check if the mouse is on top of the thumb
        if (sf::FloatRect(m_textureThumbNormal.getPosition().x, m_textureThumbNormal.getPosition().y, getThumbSize().x, getThumbSize().y).contains(x, y))
        {
            m_mouseDownOnThumbPos.x = x - m_textureThumbNormal.getPosition().x;
            m_mouseDownOnThumbPos.y = y - m_textureThumbNormal.getPosition().y;

            m_mouseDownOnThumb = true;
        }
        else // The mouse is not on top of the thumb
            m_mouseDownOnThumb = false;

        // Refresh the scrollbar value
        if (m_mouseDownOnArrow == false)
            mouseMoved(x, y);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::leftMouseReleased(float x, float y)
    {

        // Check if one of the arrows was clicked
        if ((m_mouseDown) && (m_mouseDownOnArrow))
        {
            // Only continue when the calculations can be made
            if (m_maximum > m_lowValue)
            {
                bool valueDown = false;
                bool valueUp = false;

                // Check in which direction the scrollbar lies
                if (m_verticalScroll)
                {
                    // Check if the arrows are drawn at full size
                    if (getSize().y > (m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y))
                    {
                        // Check if you clicked on the top arrow
                        if (y < getPosition().y + m_textureArrowUpNormal.getSize().y)
                            valueDown = true;

                        // Check if you clicked the down arrow
                        else if (y > getPosition().y + getSize().y - m_textureArrowUpNormal.getSize().y)
                            valueUp = true;
                    }
                    else // The arrows are not drawn at full size
                    {
                        // Check on which arrow you clicked
                        if (y < getPosition().y + (getSize().y * 0.5f))
                            valueDown = true;
                        else // You clicked on the bottom arrow
                            valueUp = true;
                    }
                }
                else // the scrollbar lies horizontal
                {
                    // Check if the arrows are drawn at full size
                    if (getSize().x > (m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y))
                    {
                        // Check if you clicked on the top arrow
                        if (x < getPosition().x + m_textureArrowUpNormal.getSize().y)
                            valueDown = true;

                        // Check if you clicked the down arrow
                        else if (x > getPosition().x + getSize().x - m_textureArrowUpNormal.getSize().y)
                            valueUp = true;
                    }
                    else // The arrows are not drawn at full size
                    {
                        // Check on which arrow you clicked
                        if (x < getPosition().x + (getSize().x * 0.5f))
                            valueDown = true;
                        else // You clicked on the bottom arrow
                            valueUp = true;
                    }
                }

                if (valueDown)
                {
                    if (m_value > m_scrollAmount)
                        setValue(m_value - m_scrollAmount);
                    else
                        setValue(0);
                }
                else if (valueUp)
                {
                    if (m_value + m_scrollAmount < m_maximum - m_lowValue + 1)
                        setValue(m_value + m_scrollAmount);
                    else
                        setValue(m_maximum - m_lowValue);
                }
            }
        }

        // The mouse is no longer down
        m_mouseDown = false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::mouseMoved(float x, float y)
    {
        if (m_mouseHover == false)
            mouseEnteredWidget();

        m_mouseHover = true;

        // Check if the mouse button went down on top of the track (or thumb)
        if ((m_mouseDown) && (m_mouseDownOnArrow == false))
        {
            // Don't continue if the calculations can't be made
            if ((m_maximum <= m_lowValue) && (m_autoHide == false))
                return;

            // Check in which direction the scrollbar lies
            if (m_verticalScroll)
            {
                // Check if the thumb is being dragged
                if (m_mouseDownOnThumb)
                {
                    // Set the new value
                    if ((y - m_mouseDownOnThumbPos.y - getPosition().y - m_textureArrowUpNormal.getSize().y) > 0)
                    {
                        // Calculate the new value
                        unsigned int value = static_cast<unsigned int>((((y - m_mouseDownOnThumbPos.y - getPosition().y - m_textureArrowUpNormal.getSize().y) / (getSize().y - (m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y))) * m_maximum) + 0.5f);

                        // If the value isn't too high then change it
                        if (value <= (m_maximum - m_lowValue))
                            setValue(value);
                        else
                            setValue(m_maximum - m_lowValue);
                    }
                    else // The mouse was above the scrollbar
                        setValue(0);
                }
                else // The click occured on the track
                {
                    // If the position is positive then calculate the correct value
                    if (y > getPosition().y + m_textureArrowUpNormal.getSize().y)
                    {
                        // Make sure that you didn't click on the down arrow
                        if (y <= getPosition().y + getSize().y - m_textureArrowUpNormal.getSize().y)
                        {
                            // Calculate the exact position (a number between 0 and maximum)
                            float value = (((y - getPosition().y - m_textureArrowUpNormal.getSize().y) / (getSize().y - (m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y))) * m_maximum);

                            // Check if you clicked above the thumb
                            if (value <= m_value)
                            {
                                float subtractValue = m_lowValue / 3.0f;

                                // Try to place the thumb on 2/3 of the clicked position
                                if (value >= subtractValue)
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(0);
                            }
                            else // The click occured below the thumb
                            {
                                float subtractValue = m_lowValue * 2.0f / 3.0f;

                                // Try to place the thumb on 2/3 of the clicked position
                                if (value <= (m_maximum - m_lowValue + subtractValue))
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(m_maximum - m_lowValue);
                            }
                        }
                    }

                    m_mouseDownOnThumbPos.x = x - m_textureThumbNormal.getPosition().x;
                    m_mouseDownOnThumbPos.y = y - m_textureThumbNormal.getPosition().y;
                    m_mouseDownOnThumb = true;
                }
            }
            else // the scrollbar lies horizontal
            {
                // Check if the thumb is being dragged
                if (m_mouseDownOnThumb)
                {
                    // Set the new value
                    if ((x - m_mouseDownOnThumbPos.x - getPosition().x - m_textureArrowUpNormal.getSize().y) > 0)
                    {
                        // Calculate the new value
                        unsigned int value = static_cast<unsigned int>((((x - m_mouseDownOnThumbPos.x - getPosition().x - m_textureArrowUpNormal.getSize().y) / (getSize().x - (m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y))) * m_maximum) + 0.5f);

                        // If the value isn't too high then change it
                        if (value <= (m_maximum - m_lowValue))
                            setValue(value);
                        else
                            setValue(m_maximum - m_lowValue);
                    }
                    else // The mouse was above the scrollbar
                        setValue(0);
                }
                else // The click occured on the track
                {
                    // If the position is positive then calculate the correct value
                    if (x > getPosition().x + m_textureArrowUpNormal.getSize().y)
                    {
                        // Make sure that you didn't click on the down arrow
                        if (x <= getPosition().x + getSize().x - m_textureArrowUpNormal.getSize().y)
                        {
                            // Calculate the exact position (a number between 0 and maximum)
                            float value = (((x - getPosition().x - m_textureArrowUpNormal.getSize().y) / (getSize().x - (m_textureArrowUpNormal.getSize().y + m_textureArrowDownNormal.getSize().y))) * m_maximum);

                            // Check if you clicked above the thumb
                            if (value <= m_value)
                            {
                                float subtractValue = m_lowValue / 3.0f;

                                // Try to place the thumb on 2/3 of the clicked position
                                if (value >= subtractValue)
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(0);
                            }
                            else // The click occured below the thumb
                            {
                                float subtractValue = m_lowValue * 2.0f / 3.0f;

                                // Try to place the thumb on 2/3 of the clicked position
                                if (value <= (m_maximum - m_lowValue + subtractValue))
                                    setValue(static_cast<unsigned int>(value - subtractValue + 0.5f));
                                else
                                    setValue(m_maximum - m_lowValue);
                            }
                        }
                    }

                    m_mouseDownOnThumbPos.x = x - m_textureThumbNormal.getPosition().x;
                    m_mouseDownOnThumbPos.y = y - m_textureThumbNormal.getPosition().y;
                    m_mouseDownOnThumb = true;
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::mouseWheelMoved(int delta, int, int)
    {
        if (static_cast<int>(m_value) - static_cast<int>(delta * m_scrollAmount) < 0)
            setValue(0);
        else
            setValue(static_cast<unsigned int>(m_value - (delta * m_scrollAmount)));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::widgetFocused()
    {
        // A scrollbar can't be focused (yet)
        unfocus();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Vector2f Scrollbar::getThumbSize() const
    {
        if (m_verticalImage == m_verticalScroll)
            return m_textureThumbNormal.getSize();
        else
            return {m_textureThumbNormal.getSize().y, m_textureThumbNormal.getSize().x};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Scrollbar::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // Don't draw the scrollbar when it isn't needed
        if ((m_autoHide == true) && (m_maximum <= m_lowValue))
            return;

        if (m_separateHoverImage)
        {
            if (m_mouseHover)
            {
                if (m_textureTrackHover.getData())
                    target.draw(m_textureTrackHover, states);

                if (m_textureThumbHover.getData())
                    target.draw(m_textureThumbHover, states);

                if (m_textureArrowUpHover.getData())
                    target.draw(m_textureArrowUpHover, states);

                if (m_textureArrowDownHover.getData())
                    target.draw(m_textureArrowDownHover, states);
            }
            else
            {
                target.draw(m_textureTrackNormal, states);
                target.draw(m_textureThumbNormal, states);
                target.draw(m_textureArrowUpNormal, states);
                target.draw(m_textureArrowDownNormal, states);
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

                target.draw(m_textureArrowUpNormal, states);
                if (m_textureArrowUpHover.getData())
                    target.draw(m_textureArrowUpHover, states);

                target.draw(m_textureArrowDownNormal, states);
                if (m_textureArrowDownHover.getData())
                    target.draw(m_textureArrowDownHover, states);
            }
            else
            {
                target.draw(m_textureThumbNormal, states);
                target.draw(m_textureArrowUpNormal, states);
                target.draw(m_textureArrowDownNormal, states);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

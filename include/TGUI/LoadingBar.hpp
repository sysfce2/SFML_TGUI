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


#ifndef TGUI_LOADING_BAR_HPP
#define TGUI_LOADING_BAR_HPP


#include <TGUI/Label.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API LoadingBar : public ClickableWidget
    {
    public:

        typedef std::shared_ptr<LoadingBar> Ptr;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        LoadingBar();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual destructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ~LoadingBar() {}


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Create the loading bar
        ///
        /// @param configFileFilename  Filename of the config file.
        ///
        /// @throw Exception when the config file couldn't be opened.
        /// @throw Exception when the config file didn't contain a "Loadingbar" section with the needed information.
        /// @throw Exception when one of the images, described in the config file, couldn't be loaded.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static LoadingBar::Ptr create(const std::string& configFileFilename);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Makes a copy of another loading bar
        ///
        /// @param loadingBar  The other loading bar
        ///
        /// @return The new loading bar
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static LoadingBar::Ptr copy(const LoadingBar::Ptr& loadingBar)
        {
            return std::make_shared<LoadingBar>(*loadingBar);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the filename of the config file that was used to load the widget.
        ///
        /// @return Filename of loaded config file.
        ///         Empty string when no config file was loaded yet.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::string& getLoadedConfigFile() const
        {
            return m_loadedConfigFile;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Set the position of the widget
        ///
        /// This function completely overwrites the previous position.
        /// See the move function to apply an offset based on the previous position instead.
        /// The default position of a transformable widget is (0, 0).
        ///
        /// @param position  New position
        ///
        /// @see move, getPosition
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setPosition(const Layout& position) override;
        using Transformable::setPosition;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the size of the loading bar.
        ///
        /// @param size  The new size of the loading bar
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSize(const Layout& size) override;
        using Transformable::setSize;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets a minimum value.
        ///
        /// @param minimum  The new minimum value
        ///
        /// When the value is too small then it will be changed to this minimum.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setMinimum(unsigned int minimum);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets a maximum value.
        ///
        /// @param maximum  The new maximum value
        ///
        /// When the value is too big then it will be changed to this maximum.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setMaximum(unsigned int maximum);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the current value.
        ///
        /// @param value  The new value
        ///
        /// The value can't be smaller than the minimum or bigger than the maximum.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setValue(unsigned int value);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the minimum value.
        ///
        /// @return The current minimum value
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getMinimum() const
        {
            return m_minimum;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the maximum value.
        ///
        /// @return The current maximum value
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getMaximum() const
        {
            return m_maximum;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the current value.
        ///
        /// @return The current value
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getValue() const
        {
            return m_value;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Increment the value.
        ///
        /// @return the new value
        ///
        /// The value can never exceed the maximum.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int incrementValue();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the caption of the loading bar.
        ///
        /// @param text  New text to draw on top of the loading bar
        ///
        /// This text will be centered in the loading bar. It could e.g. contain the progress.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setText(const sf::String& text);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the caption of the loading bar.
        ///
        /// @return Text that is drawn on top of the loading bar
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::String getText() const
        {
            return m_text.getText();
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the font of the text.
        ///
        /// When you don't call this function then the global font will be use.
        /// This global font can be changed with the setGlobalFont function from the parent.
        ///
        /// @param font  The new font
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextFont(const sf::Font& font);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the font of the text.
        ///
        /// @return  Pointer to the font that is currently being used
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Font* getTextFont() const
        {
            return m_text.getTextFont();
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the color of the text.
        ///
        /// @param color  New text color
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextColor(const sf::Color& color)
        {
            m_text.setTextColor(color);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the color of the text.
        ///
        /// @return The current text color
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Color& getTextColor() const
        {
            return m_text.getTextColor();
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the character size of the text.
        ///
        /// @param size  The new text size.
        ///              If the size is set to 0 then the text will be auto-sized to fit inside the loading bar.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextSize(unsigned int size);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the character size of the text.
        ///
        /// @return The current text size.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getTextSize() const
        {
            return m_text.getTextSize();
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the transparency of the widget.
        ///
        /// @param transparency  The transparency of the widget.
        ///                      0 is completely transparent, while 255 (default) means fully opaque.
        ///
        /// Note that this will only change the transparency of the images. The parts of the widgets that use a color will not
        /// be changed. You must change them yourself by setting the alpha channel of the color.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setTransparency(unsigned char transparency) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This function is called when the widget is added to a container.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void initialize(Container *const container) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Widget::Ptr clone() override
        {
            return std::make_shared<LoadingBar>(*this);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // When the value changes, or when the minimum/maximum limits change then a smaller of bigger piece of the front image
        // must be drawn. This function is called to calculate the size of the piece to draw.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void recalculateSize();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Draws the widget on the render target.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Defines specific triggers to LoadingBar.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        enum LoadingBarCallbacks
        {
            ValueChanged = ClickableWidgetCallbacksCount * 1,               ///< Value changed
            LoadingBarFull = ClickableWidgetCallbacksCount * 2,             ///< Value reached the max value.
            AllLoadingBarCallbacks = ClickableWidgetCallbacksCount * 4 - 1, ///< All triggers defined in LoadingBar and its base classes
            LoadingBarCallbacksCount = ClickableWidgetCallbacksCount * 4
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        std::string  m_loadedConfigFile;

        unsigned int m_minimum = 0;
        unsigned int m_maximum = 100;
        unsigned int m_value = 0;

        Texture  m_textureBack;
        Texture  m_textureFront;

        // The text that is (optionally) drawn on top of the loading bar
        Label m_text;
        unsigned int m_textSize = 0;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_LOADING_BAR_HPP

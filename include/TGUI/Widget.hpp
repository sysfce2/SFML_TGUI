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


#ifndef TGUI_WIDGETS_HPP
#define TGUI_WIDGETS_HPP


#include <TGUI/Global.hpp>
#include <TGUI/ConfigFile.hpp>
#include <TGUI/Callback.hpp>
#include <TGUI/Transformable.hpp>
#include <TGUI/Texture.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    class Container;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief The parent class for every widget.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API Widget : public sf::Drawable, public Transformable, public CallbackManager
    {
      public:

        typedef std::shared_ptr<Widget> Ptr;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Widget() {};


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Copy constructor
        ///
        /// @param copy  Instance to copy
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Widget(const Widget& copy);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual destructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ~Widget() {}


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Overload of assignment operator
        ///
        /// @param right  Instance to assign
        ///
        /// @return Reference to itself
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Widget& operator= (const Widget& right);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief set the position of the widget
        ///
        /// This function completely overwrites the previous position.
        /// See the move function to apply an offset based on the previous position instead.
        /// The default position of a transformable widget is (0, 0).
        ///
        /// @param position New position
        ///
        /// @see move, getPosition
        ///
        /// Usage examples:
        /// @code
        /// // Place the widget on an exact position
        /// widget->setPosition({40, 30});
        ///
        /// // Place the widget 50 pixels below another widget
        /// widget->setPosition(otherWidget->getPosition() + sf::Vector2f{0, otherWidget->getSize().y + 50});
        ///
        /// // Place the widget 50 pixels below another widget and automatically move it when the other widget moves
        /// widget->setPosition({tgui::bindLeft(otherWidget), tgui::bindBottom(otherWidget) + 50});
        /// @endcode
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setPosition(const Layout& position) override;
        using Transformable::setPosition;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the size of the widget.
        ///
        /// @param size  Size of the widget
        ///
        /// Usage examples:
        /// @code
        /// // Give the widget an exact size
        /// widget->setSize({40, 30});
        ///
        /// // Make the widget 50 pixels higher than some other widget
        /// widget->setSize(otherWidget->getSize() + sf::Vector2f{0, 50});
        ///
        /// // Make the widget 50 pixels higher than some other widget and automatically resize it when the other widget resizes
        /// widget->setSize(tgui::bindSize(otherWidget) + sf::Vector2f{0, 50});
        /// @endcode
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setSize(const Layout& size) override;
        using Transformable::setSize;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Get the absolute position of the widget on the screen
        ///
        /// @return Position of the widget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual sf::Vector2f getAbsolutePosition() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Shows the widget.
        ///
        /// The widget won't receive events nor will it be drawn when hidden. The widget is visible by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void show();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Hides the widget.
        ///
        /// The widget won't receive events nor will it be drawn when hidden. The widget is visible by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void hide();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns true when the widget is visible.
        ///
        /// @return Is the widget visible?
        ///
        /// If this function returns false then the widget is hidden, which means that it won't receive events and it won't be drawn.
        /// All widgets are visible by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isVisible() const
        {
            return m_visible;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Enables the widget.
        ///
        /// The widget will receive events and send callbacks again.
        /// All widgets are enabled by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void enable();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Disables the widget.
        ///
        /// The widget will no longer receive events and it will thus no longer send callbacks.
        /// All widgets are enabled by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void disable();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns true when the widget is enabled.
        ///
        /// @return Is the widget enabled?
        ///
        /// If this function returns false then the widget is disabled and will longer receive events and it will thus no longer send callbacks.
        /// All widgets are enabled by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isEnabled() const
        {
            return m_enabled;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns true when the widget is disabled.
        ///
        /// @return Is the widget disabled?
        ///
        /// This means that the widget will longer receive events and it will thus no longer send callbacks.
        /// All widgets are enabled by default.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isDisabled() const
        {
            return !m_enabled;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Focus the widget.
        ///
        /// The previously focused widget will be unfocused.
        ///
        /// @see unfocus
        /// @see focusNextWidget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void focus();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Unfocus the widget.
        ///
        /// @see focus
        /// @see focusNextWidget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void unfocus();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns true when the widget is focused and false otherwise.
        ///
        /// @return Is the widget focused?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool isFocused() const
        {
            return m_focused;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the type of the widget.
        ///
        /// @return Type of the widget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WidgetTypes getWidgetType() const
        {
            return m_callback.widgetType;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns a pointer to the parent widget.
        ///
        /// @return Pointer to the parent.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Container* getParent() const
        {
            return m_parent;
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
        virtual void setTransparency(unsigned char transparency)
        {
            m_opacity = transparency;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the transparency of the widget.
        ///
        /// @return The transparency of the widget.
        ///         0 is completely transparent, while 255 (default) means fully opaque.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned char getTransparency() const
        {
            return m_opacity;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Places the widget before all other widgets.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void moveToFront();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Places the widget behind all other widgets.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void moveToBack();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This function is called right after the elapsed time is changed.
        // The elapsed time is only changed when the widget has set m_animatedWidget to true.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void update();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This function is called when the mouse enters the widget. If requested, a callback will be send.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseEnteredWidget();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // This function is called when the mouse leaves the widget. If requested, a callback will be send.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseLeftWidget();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual bool mouseOnWidget(float x, float y) = 0;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void leftMousePressed(float x, float y);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void leftMouseReleased(float x, float y);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseMoved(float x, float y);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void keyPressed(const sf::Event::KeyEvent& event);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void textEntered(sf::Uint32 key);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseWheelMoved(int delta, int x, int y);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void widgetFocused();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void widgetUnfocused();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseNotOnWidget();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void mouseNoLongerDown();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        // This function is called when the widget is added to a container.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void initialize(Container *const container);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Makes a copy of the widget if you don't know its exact type
        ///
        /// This function should only be used when you don't know the type of the widget.
        /// If you know what kind of widget you are copying, you should use the copy function.
        ///
        /// @return Copy of the widget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Widget::Ptr clone() = 0;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      protected:


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // The widgets use this function to send their callbacks to their parent and/or to a callback function.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void addCallback();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Defines specific triggers to Widget.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        enum WidgetCallbacks
        {
            None = 0,                  ///< No trigger
            PositionChanged = 1,       ///< Position of the widget has changed
            SizeChanged = 2,           ///< Size of the widget has changed
            Focused = 4,               ///< Widget gained focus.
            Unfocused = 8,             ///< Widget lost focus.
            MouseEntered = 16,         ///< Mouse cursor entered in the Widget area.
            MouseLeft = 32,            ///< Mouse cursor left the Widget area.
            WidgetCallbacksCount = 64
        };


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      protected:

        // When a widget is disabled, it will no longer receive events
        bool m_enabled = true;

        // Is the widget visible? When it is invisible it will not receive events and it won't be drawn.
        bool m_visible = true;

        // This will point to our parent widget. If there is no parent then this will be nullptr.
        Container* m_parent = nullptr;

        // How transparent is the widget
        unsigned char m_opacity = 255;

        // Is the mouse on top of the widget? Did the mouse go down on the widget?
        bool m_mouseHover = false;
        bool m_mouseDown = false;

        // Are you focused on the widget?
        bool m_focused = false;

        // Can the widget be focused?
        bool m_allowFocus = false;

        // Keep track of the elapsed time.
        bool m_animatedWidget = false;
        sf::Time m_animationTimeElapsed;

        // This is set to true for widgets that have something to be dragged around (e.g. sliders and scrollbars)
        bool m_draggableWidget = false;

        // This is set to true for widgets that store other widgets inside them
        bool m_containerWidget = false;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        friend class Container;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_WIDGETS_HPP

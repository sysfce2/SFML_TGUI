/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2025 Bruno Van de Velde (vdv_b@tgui.eu)
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

#ifndef TGUI_SPLIT_CONTAINER_HPP
#define TGUI_SPLIT_CONTAINER_HPP

#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Renderers/SplitContainerRenderer.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGUI_MODULE_EXPORT namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Container that should contains exactly 2 child widgets which will be resized to fill the container together,
    ///        with a splitter between them that can be dragged to change the amount of space each widget takes up.
    ///
    /// The first widget added to the container will be the left/top one, the second widget that is added is the right/bottom one.
    ///
    /// @since TGUI 1.4
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API SplitContainer : public Group
    {
    public:

        using Ptr = std::shared_ptr<SplitContainer>; //!< Shared widget pointer
        using ConstPtr = std::shared_ptr<const SplitContainer>; //!< Shared constant widget pointer

        static constexpr const char StaticWidgetType[] = "SplitContainer"; //!< Type name of the widget

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Constructor
        /// @param typeName     Type of the widget
        /// @param initRenderer Should the renderer be initialized? Should be true unless a derived class initializes it.
        /// @see create
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SplitContainer(const char* typeName = StaticWidgetType, bool initRenderer = true);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Creates a new split container widget
        ///
        /// @param size  Size of the split container
        ///
        /// @return The new split container
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD static SplitContainer::Ptr create(const Layout2d& size = {"100%", "100%"});

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Makes a copy of another split container
        ///
        /// @param splitContainer  The other split container
        ///
        /// @return The new split container
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD static SplitContainer::Ptr copy(const SplitContainer::ConstPtr& splitContainer);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the renderer, which gives access to functions that determine how the widget is displayed
        /// @return Temporary pointer to the renderer that may be shared with other widgets using the same renderer
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD SplitContainerRenderer* getSharedRenderer() override;
        TGUI_NODISCARD const SplitContainerRenderer* getSharedRenderer() const override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the renderer, which gives access to functions that determine how the widget is displayed
        /// @return Temporary pointer to the renderer
        /// @warning After calling this function, the widget has its own copy of the renderer and it will no longer be shared.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD SplitContainerRenderer* getRenderer() override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the size of the split container
        ///
        /// @param size  The new size of the split container
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSize(const Layout2d& size) override;
        using Widget::setSize;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Adds a widget to the container
        ///
        /// @param widgetPtr   Pointer to the widget you would like to add
        /// @param widgetName  You can give the widget a unique name to retrieve it from the container later
        ///
        /// @warning Widgets should be named as if they are C++ variables, i.e. names must not include any whitespace, or most
        ///          symbols (e.g.: +, -, *, /, ., &), and should not start with a number. If you do not follow these rules,
        ///          layout expressions may give unexpected results. Alphanumeric characters and underscores are safe to use,
        ///          and widgets are permitted to have no name.
        ///
        /// The first widget added to the container will be placed on the left/top side of the splitter.
        /// The second widget added to the container will be placed on the right/bottom side of the splitter.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void add(const Widget::Ptr& widgetPtr, const String& widgetName = "") override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the index of a widget in this container
        ///
        /// Widgets are drawn in the order of the list, so overlapping widgets with a higher index will appear on top of others.
        ///
        /// By changing the order of the first and second widget, you swap which widget is placed on the left/top side and which
        /// widget is placed on the right/bottom side of the splitter.
        ///
        /// @param widget  Widget that is to be moved to a different index
        /// @param index   New index of the widget, corresponding to the widget position after the widget has been moved
        ///
        /// @return True when the index was changed, false if widget wasn't found in the container or index was too high
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool setWidgetIndex(const Widget::Ptr& widget, std::size_t index) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets whether the widgets are placed left and right of the splitter or above and below the splitter
        ///
        /// @param orientation  Horizontal to place widgets beside each other, vertical to place widgets below each other
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setOrientation(Orientation orientation);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns whether the widgets are placed left and right of the splitter or above and below the splitter
        ///
        /// @return Horizontal if widgets are placed beside each other, vertical if widgets are placed below each other
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD Orientation getOrientation() const;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the minimum value for the splitter, effectively providing a minimum size for the top/left widget
        ///
        /// @param minOffset  Minimum splitter offset
        ///
        /// Example code:
        /// @code
        /// splitContainer1->setMinValidSplitterOffset(200);
        /// splitContainer2->setMinValidSplitterOffset("30%");
        /// @endcode
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setMinValidSplitterOffset(AbsoluteOrRelativeValue minOffset);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the minimum value for the splitter
        ///
        /// @return Current minimum splitter offset
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD float getMinValidSplitterOffset() const;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the maximum value for the splitter, effectively providing a minimum size for the bottom/right widget
        ///
        /// @param maxOffset  Maximum splitter offset
        ///
        /// Example code:
        /// @code
        /// splitContainer1->setMaxValidSplitterOffset(600);
        /// splitContainer2->setMaxValidSplitterOffset("80%");
        /// @endcode
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setMaxValidSplitterOffset(AbsoluteOrRelativeValue maxOffset);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the maximum value for the splitter
        ///
        /// @return Current maximum splitter offset
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD float getMaxValidSplitterOffset() const;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the position of the splitter from the left or top position (depending on orientation)
        ///
        /// @param offset  Splitter position, which equals the size of the left or top widget (depending on orientation)
        ///
        /// Example code:
        /// @code
        /// splitContainer1->setSplitterOffset(100);
        /// splitContainer2->setSplitterOffset("50%");
        /// @endcode
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSplitterOffset(AbsoluteOrRelativeValue offset);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the position of the splitter from the left or top position (depending on orientation)
        ///
        /// @return Splitter position, which equals the size of the left or top widget (depending on orientation)
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD float getSplitterOffset() const;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the width of the draggable splitter line that separates the two widgets in the container
        ///
        /// @param width  Thickness of the splitter line
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSplitterWidth(float width);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the width of the draggable splitter line that separates the two widgets in the container
        ///
        /// @return Thickness of the splitter line
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD float getSplitterWidth() const;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the minimum width of the area where the splitter can be dragged
        ///
        /// @param width  Minimum width of draggable area located at the splitter
        ///
        /// If this value is less or equal to SplitterWidth then the splitter can be grabbed when the mouse is on top of it.
        /// If the value is larger than SplitterWidth then there is a margin around the splitter where you can also grab it.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setMinimumGrabWidth(float width);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the minimum width of the area where the splitter can be dragged
        ///
        /// @return Minimum width of draggable area located at the splitter
        ///
        /// If this value is less or equal to SplitterWidth then the splitter can be grabbed when the mouse is on top of it.
        /// If the value is larger than SplitterWidth then there is a margin around the splitter where you can also grab it.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD float getMinimumGrabWidth() const;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns whether the mouse position (which is relative to the parent widget) lies on top of the widget
        ///
        /// @return Is the mouse on top of the widget?
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD bool isMouseOnWidget(Vector2f pos) const override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool leftMousePressed(Vector2f pos) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void mouseMoved(Vector2f pos) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void mouseNoLongerOnWidget() override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Draw the child widgets to a render target
        ///
        /// @param target Render target to draw to
        /// @param states Current render states
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void draw(BackendRenderTarget& target, RenderStates states) const override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Function called when one of the properties of the renderer is changed
        ///
        /// @param property  Name of the property that was changed
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void rendererChanged(const String& property) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Saves the widget as a tree node in order to save it to a file
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD std::unique_ptr<DataIO::Node> save(SavingRenderersMap& renderers) const override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Loads the widget from a tree of nodes
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers) override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief This function is called when the mouse enters the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void mouseEnteredWidget() override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief This function is called when the mouse leaves the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void mouseLeftWidget() override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD Widget::Ptr clone() const override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Updates the splitter offset if it is defined as a relative value, and then calls updateChildren()
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void updateAvailableSize();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Updates the position and sizes of the 2 child widgets based on the current splitter offset
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void updateChildren();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Returns the area occupied by the splitter, with a marin around it if m_minimumGrabWidth > m_splitterWidth
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD FloatRect getSplitterInteractRect() const;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        Orientation m_orientation = Orientation::Horizontal;
        AbsoluteOrRelativeValue m_splitterMinOffset;
        AbsoluteOrRelativeValue m_splitterMaxOffset = RelativeValue(1);
        AbsoluteOrRelativeValue m_splitterOffset = RelativeValue(0.5f);
        float m_currentSplitterOffset = 0;
        float m_splitterWidth = 3;
        float m_minimumGrabWidth = 5;

        bool m_mouseHoverOnSplitter = false;
        bool m_mouseDownOnSplitter = false;
        float m_mouseDownOnSplitterOffset = 0;
        Cursor::Type m_currentSplitContainerMouseCursor = Cursor::Type::Arrow;

        // Cached renderer properties
        Color   m_splitterColorCached;
        Color   m_splitterColorHoverCached;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_SPLIT_CONTAINER_HPP

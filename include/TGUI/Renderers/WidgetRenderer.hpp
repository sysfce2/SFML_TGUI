/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2017 Bruno Van de Velde (vdv_b@tgui.eu)
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


#ifndef TGUI_WIDGET_RENDERER_HPP
#define TGUI_WIDGET_RENDERER_HPP


#include <TGUI/Config.hpp>
#include <TGUI/ObjectConverter.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Shared data used in renderer classes
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct RendererData
    {
        RendererData() = default;

        static std::shared_ptr<RendererData> create(const std::map<std::string, ObjectConverter>& init = {})
        {
            auto data = std::make_shared<RendererData>();
            data->propertyValuePairs = init;
            return data;
        }

        std::map<std::string, ObjectConverter> propertyValuePairs;
        std::map<void*, std::function<void(const std::string& property)>> observers;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Base class for all renderer classes
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API WidgetRenderer
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WidgetRenderer() = default;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Construct the renderer from renderer data
        ///
        /// @param data  Renderer data to initialize the renderer with
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        WidgetRenderer(const std::shared_ptr<RendererData>& data)
        {
            setData(data);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Virtual destructor
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ~WidgetRenderer() {};


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the opacity of the widget
        ///
        /// @param opacity  The opacity of the widget. 0 means completely transparent, while 1 (default) means fully opaque
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setOpacity(float opacity);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the opacity of the widget
        ///
        /// @return The opacity of the widget. 0 means completely transparent, while 1 (default) means fully opaque
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getOpacity() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the font used for the text in the widget
        ///
        /// @param font  The new font
        ///
        /// When you don't call this function then the font from the parent widget will be used.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setFont(Font font);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the font associated with the widget (if any)
        ///
        /// @return Font used by widget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Font getFont() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes a property of the renderer
        ///
        /// @param property  The property that you would like to change
        /// @param value     The new value that you like to assign to the property.
        ///                  The ObjectConverter is implicitly constructed from the possible value types
        ///
        /// @throw Exception for unknown properties or when value was of a wrong type
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setProperty(const std::string& property, ObjectConverter&& value);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Retrieves the value of a certain property
        ///
        /// @param property  The property that you would like to retrieve
        ///
        /// @return The value inside a ObjectConverter object which you can extract with the correct get function or
        ///         an ObjectConverter object with type ObjectConverter::Type::None when the property did not exist.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ObjectConverter getProperty(const std::string& property) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Gets a map with all properties and their values
        ///
        /// @return Property-value pairs of the renderer
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::map<std::string, ObjectConverter>& getPropertyValuePairs() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Subscribes a callback function to changes in the renderer
        ///
        /// @param id       Unique identifier for this callback function so that you can unsubscribe it later
        /// @param function Callback function to call when the renderer changes
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void subscribe(void* id, const std::function<void(const std::string& property)>& function);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Subscribes a callback function to changes in the renderer
        ///
        /// @param id  Unique identifier used when subscribing the callback function
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void unsubscribe(void* id);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Directly change all the data of this renderer
        ///
        /// You are not supposed to call this function yourself, use the setRenderer function of the widget instead.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setData(const std::shared_ptr<RendererData>& data);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the renderer data
        ///
        /// @return Data that is shared between the renderers
        ///
        /// @warning You should not make changed to this data directly. Instead, use the function from the renderer classes
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<RendererData> getData() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Gets a clone of the renderer data
        ///
        /// You can pass this to a widget with the setRenderer function to have a separate non-shared copy of this renderer.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<RendererData> clone() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        std::shared_ptr<RendererData> m_data = RendererData::create();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_WIDGETS_HPP

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


#include <TGUI/Global.hpp>
#include <TGUI/Clipboard.hpp>

#include <cctype>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    Clipboard TGUI_Clipboard;

    bool TGUI_TabKeyUsageEnabled = true;

    std::string TGUI_ResourcePath = "";

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void enableTabKeyUsage()
    {
        TGUI_TabKeyUsageEnabled = true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void disableTabKeyUsage()
    {
        TGUI_TabKeyUsageEnabled = false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void setResourcePath(const std::string& path)
    {
        TGUI_ResourcePath = path;

        if (!TGUI_ResourcePath.empty())
        {
            if (TGUI_ResourcePath[TGUI_ResourcePath.length()-1] != '/')
                TGUI_ResourcePath.push_back('/');
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::string& getResourcePath()
    {
        return TGUI_ResourcePath;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int stoi(const std::string& value)
    {
        return std::atoi(value.c_str());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float stof(const std::string& value)
    {
        return static_cast<float>(std::atof(value.c_str()));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned long stoul(const std::string& value)
    {
        return static_cast<unsigned long>(std::atoi(value.c_str()));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Color extractColor(std::string string)
    {
        int red;
        int green;
        int blue;
        int alpha = 255;

        // Make sure that the line isn't empty
        if (string.empty() == false)
        {
            // The first and last character have to be brackets
            if ((string[0] == '(') && (string[string.length()-1] == ')'))
            {
                // Remove the brackets
                string.erase(0, 1);
                string.erase(string.length()-1);

                // Search for the first comma
                std::string::size_type commaPos = string.find(',');
                if (commaPos != std::string::npos)
                {
                    // Get the red value and delete this part of the string
                    red = tgui::stoi(string.substr(0, commaPos));
                    string.erase(0, commaPos+1);

                    // Search for the second comma
                    commaPos = string.find(',');
                    if (commaPos != std::string::npos)
                    {
                        // Get the green value and delete this part of the string
                        green = tgui::stoi(string.substr(0, commaPos));
                        string.erase(0, commaPos+1);

                        // Search for the third comma (optional)
                        commaPos = string.find(',');
                        if (commaPos != std::string::npos)
                        {
                            // Get the blue value and delete this part of the string
                            blue = tgui::stoi(string.substr(0, commaPos));
                            string.erase(0, commaPos+1);

                            // Get the alpha value
                            alpha = tgui::stoi(string);
                        }
                        else // No alpha value was passed
                        {
                            // Get the blue value
                            blue = tgui::stoi(string.substr(0, commaPos));
                        }

                        // All values have to be unsigned chars
                        return sf::Color(static_cast <unsigned char> (red),
                                         static_cast <unsigned char> (green),
                                         static_cast <unsigned char> (blue),
                                         static_cast <unsigned char> (alpha));
                    }
                }
            }
        }

        // If you pass here then something is wrong about the line
        throw Exception{"Failed to parse color from string."};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string convertColorToString(const sf::Color& color)
    {
        // Return the color as a string
        if (color.a < 255)
            return "(" + tgui::to_string((unsigned int)color.r)
                 + "," + tgui::to_string((unsigned int)color.g)
                 + "," + tgui::to_string((unsigned int)color.b)
                 + "," + tgui::to_string((unsigned int)color.a)
                 + ")";
        else
            return "(" + tgui::to_string((unsigned int)color.r)
                 + "," + tgui::to_string((unsigned int)color.g)
                 + "," + tgui::to_string((unsigned int)color.b)
                 + ")";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool extractBorders(std::string string, Borders& borders)
    {
        // Make sure that the line isn't empty
        if (string.empty() == false)
        {
            // The first and last character have to be brackets
            if ((string[0] == '(') && (string[string.length()-1] == ')'))
            {
                // Remove the brackets
                string.erase(0, 1);
                string.erase(string.length()-1);

                // Search for the first comma
                std::string::size_type commaPos = string.find(',');
                if (commaPos != std::string::npos)
                {
                    // Get the first value and delete this part of the string
                    borders.left = tgui::stof(string.substr(0, commaPos));
                    string.erase(0, commaPos+1);

                    // Search for the second comma
                    commaPos = string.find(',');
                    if (commaPos != std::string::npos)
                    {
                        // Get the second value and delete this part of the string
                        borders.top = tgui::stof(string.substr(0, commaPos));
                        string.erase(0, commaPos+1);

                        // Search for the third comma
                        commaPos = string.find(',');
                        if (commaPos != std::string::npos)
                        {
                            // Get the third value and delete this part of the string
                            borders.right = tgui::stof(string.substr(0, commaPos));
                            string.erase(0, commaPos+1);

                            // Get the fourth value
                            borders.bottom = tgui::stof(string);

                            return true;
                        }
                    }
                }
            }
        }

        // If you pass here then something is wrong with the string
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string toLower(std::string str)
    {
        for (std::string::iterator i = str.begin(); i != str.end(); ++i)
            *i = static_cast<char>(std::tolower(*i));

        return str;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

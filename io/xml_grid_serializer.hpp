#ifndef XML_GRID_SERIALIZER_HPP
#define XML_GRID_SERIALIZER_HPP

#include <string>
#include <stdexcept>
#include <vector>

#include "tinyxml2_error.hpp"
#include "../lib/tinyxml2/tinyxml2.hpp"
#include "../core/grid.hpp"

namespace Picross
{
    class XMLGridSerialzer
    {
        private:
            // Reading utilities
            void loadXMLFile(std::string path, tinyxml2::XMLDocument& doc);
            tinyxml2::XMLElement* findFirstChildOrThrow(tinyxml2::XMLElement* root, std::string name);
            std::vector<std::vector<int>> parseHintEntryCollection(tinyxml2::XMLElement* hintsElt);
            void parseGridContent(tinyxml2::XMLElement* contentElt, Grid& grid);
            cell_t stringToCellState(std::string value);

            // Writing utilities
            void saveXMLFile(tinyxml2::XMLDocument& doc, std::string path);
            void addXMLHints(tinyxml2::XMLDocument& sourceDoc, tinyxml2::XMLElement* root, const std::vector<std::vector<int>>& hints);
            void addXMLGridContent(tinyxml2::XMLDocument& sourceDoc, tinyxml2::XMLElement* root, const Grid& grid);
            std::string cellStateToString(cell_t value);

            template<typename T>
            inline T getValueFromText(tinyxml2::XMLElement* element);

            template<typename T>
            inline T getValueFromAttribute(tinyxml2::XMLElement* element, std::string attrName);

        public:
            XMLGridSerialzer();
            void saveGridToFile(const Grid& grid, std::string path);
            Grid loadGridFromFile(std::string path);
    };

    class InvalidXMLGridError : public std::exception
    {
        private:
            std::string _message;

        public:
            InvalidXMLGridError(const char* message);
            InvalidXMLGridError(std::string message);
            const char* what() const noexcept;
    };

    template<>
    inline int XMLGridSerialzer::getValueFromText(tinyxml2::XMLElement* element)
    {
        int value;
        tinyxml2::XMLError err = element->QueryIntText(&value);
        if (err)
        {
            throw tinyxml2::TinyXML2Error(err);
        }

        return value;
    }

    template<>
    inline std::string XMLGridSerialzer::getValueFromText(tinyxml2::XMLElement* element)
    {
        return element->GetText();
    }

    template<>
    inline int XMLGridSerialzer::getValueFromAttribute(tinyxml2::XMLElement* element, std::string attrName)
    {
        int value;
        tinyxml2::XMLError err = element->QueryIntAttribute(attrName.c_str(), &value);
        if (err)
        {
            throw tinyxml2::TinyXML2Error(err);
        }

        return value;
    }

    template<>
    inline std::string XMLGridSerialzer::getValueFromAttribute(tinyxml2::XMLElement* element, std::string attrName)
    {
        std::string value;
        const char* res = element->Attribute(attrName.c_str());
        if (!res)
        {
            throw InvalidXMLGridError("Missing attribute " + attrName + " in element " + element->Name() + " in provided file.");
        }

        return value;
    }
}

#endif//XML_GRID_SERIALIZER_HPP
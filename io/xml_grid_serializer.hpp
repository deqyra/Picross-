#ifndef IO__XML_GRID_SERIALIZER_HPP
#define IO__XML_GRID_SERIALIZER_HPP

#include <string>
#include <stdexcept>
#include <vector>

#include "exceptions/tinyxml2_error.hpp"
#include "exceptions/invalid_xml_grid_error.hpp"
#include "../lib/tinyxml2/tinyxml2.hpp"
#include "../core/grid.hpp"

namespace Picross
{
    class XMLGridSerialzer
    {
        public:     // Public methods
            XMLGridSerialzer();

            void saveGridToFile(const Grid& grid, std::string path);
            Grid loadGridFromFile(std::string path);

        private:    // Private methods
        // Reading utilities
            // Load the specified xml file into the given document object.
            void loadXMLFile(std::string path, tinyxml2::XMLDocument& doc);
            // Find the first child of the given name in the provided node and auto-throw if not found.
            tinyxml2::XMLElement* findFirstChildOrThrow(tinyxml2::XMLElement* root, const std::string& name);
            // Parse all hints from the provided XML element into a vector.
            std::vector<std::vector<int>> parseHintEntryCollection(tinyxml2::XMLElement* hintsElt);
            // Fill provided grid with the contents of the provided XML node.
            void parseGridContent(tinyxml2::XMLElement* contentElt, Grid& grid);
            // Get a cell state from a string.
            cell_t stringToCellState(std::string value);

        // Writing utilities
            // Save the provided grid to an XML file of given name.
            void saveXMLFile(tinyxml2::XMLDocument& doc, std::string path);
            // Add hints from a vector of vectors into an XML element.
            void addXMLHints(tinyxml2::XMLDocument& sourceDoc, tinyxml2::XMLElement* root, const std::vector<std::vector<int>>& hints);
            // Add grid content from a grid into an XML element.
            void addXMLGridContent(tinyxml2::XMLDocument& sourceDoc, tinyxml2::XMLElement* root, const Grid& grid);
            // Convert a cell state into a string
            std::string cellStateToString(cell_t value);

            // Generic functions to query the text of a node and extract a value of templated type from it.
            template<typename T>
            inline T getValueFromText(tinyxml2::XMLElement* element);

            // Generic functions to query an attribute of a node and extract a value of templated type from it.
            template<typename T>
            inline T getValueFromAttribute(tinyxml2::XMLElement* element, const std::string& attrName);
    };

    template<>
    inline int XMLGridSerialzer::getValueFromText(tinyxml2::XMLElement* element)
    {
        int value;
        // Query the integer value and throw any error.
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
    inline int XMLGridSerialzer::getValueFromAttribute(tinyxml2::XMLElement* element, const std::string& attrName)
    {
        int value;
        // Query the integer attribute and throw any error.
        tinyxml2::XMLError err = element->QueryIntAttribute(attrName.c_str(), &value);
        if (err)
        {
            throw tinyxml2::TinyXML2Error(err);
        }

        return value;
    }

    template<>
    inline std::string XMLGridSerialzer::getValueFromAttribute(tinyxml2::XMLElement* element, const std::string& attrName)
    {
        // Query the attribute and throw if it couldn't be found.
        const char* res = element->Attribute(attrName.c_str());
        if (!res)
        {
            throw InvalidXMLGridError("Missing attribute " + attrName + " in element " + element->Name() + " in provided file.");
        }

        std::string value = std::string(res);
        return value;
    }
} // namespace Picross

#endif//IO__XML_GRID_SERIALIZER_HPP
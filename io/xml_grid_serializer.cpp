#include "xml_grid_serializer.hpp"

#include <string>
#include <stdexcept>
#include <vector>

#include "exceptions/tinyxml2_error.hpp"
#include "exceptions/invalid_xml_grid_error.hpp"
#include "../lib/tinyxml2/tinyxml2.hpp"
#include "../core/grid.hpp"
#include "../core/utility.hpp"
#include "../tools/exceptions/index_out_of_bounds_error.hpp"

namespace Picross
{
    XMLGridSerialzer::XMLGridSerialzer()
    {

    }

    void XMLGridSerialzer::saveGridToFile(const Grid& grid, std::string path)
    {
        // Construct an XML grid from the given grid.
        tinyxml2::XMLDocument xmlDoc;

        // Create the main element, with both width and height.
        tinyxml2::XMLElement* gridElt = xmlDoc.NewElement("grid");
        gridElt->SetAttribute("width", grid.getWidth());
        gridElt->SetAttribute("height", grid.getHeight());
        xmlDoc.InsertFirstChild(gridElt);

        // Create the hints element.
        tinyxml2::XMLElement* hintsElt = xmlDoc.NewElement("hints");
        gridElt->InsertEndChild(hintsElt);

        // Make an XML element out of the horizontal hints and attach it to the hints element.
        tinyxml2::XMLElement* hHintsElt = xmlDoc.NewElement("horizontal");
        hintsElt->InsertEndChild(hHintsElt);
        addXMLHints(xmlDoc, hHintsElt, grid.getAllRowHints());

        // Same for vertical hints.
        tinyxml2::XMLElement* vHintsElt = xmlDoc.NewElement("vertical");
        hintsElt->InsertEndChild(vHintsElt);
        addXMLHints(xmlDoc, vHintsElt, grid.getAllColHints());

        // Make an XML element out of the grid content and attach it to the whole grid.
        tinyxml2::XMLElement* contentElt = xmlDoc.NewElement("content");
        gridElt->InsertEndChild(contentElt);
        addXMLGridContent(xmlDoc, contentElt, grid);

        // Save it all.
        saveXMLFile(xmlDoc, path);
    }

    Grid XMLGridSerialzer::loadGridFromFile(std::string path)
    {
        // Load the provided file.
        tinyxml2::XMLDocument doc;
        loadXMLFile(path, doc);

        // Get the root node.
        tinyxml2::XMLElement* gridElt = doc.FirstChildElement("grid");
        if (!gridElt) throw InvalidXMLGridError("Missing root node \"grid\" in provided file.");

        // Parse width and height of the grid.
        int width = getValueFromAttribute<int>(gridElt, "width");
        int height = getValueFromAttribute<int>(gridElt, "height");

        // Get all elements related to hints.
        tinyxml2::XMLElement* hintsElt = findFirstChildOrThrow(gridElt, "hints");
        tinyxml2::XMLElement* hHintsElt = findFirstChildOrThrow(hintsElt, "horizontal");
        tinyxml2::XMLElement* vHintsElt = findFirstChildOrThrow(hintsElt, "vertical");

        // Parse horizontal hints and get a vector of them.
        std::vector<std::vector<int>> hHints = parseHintEntryCollection(hHintsElt);
        if (hHints.size() != height) throw InvalidXMLGridError("Not enough horizontal hint entries for specified grid height.");

        // Same for vertical hints.
        std::vector<std::vector<int>> vHints = parseHintEntryCollection(vHintsElt);
        if (vHints.size() != width) throw InvalidXMLGridError("Not enough vertical hint entries for specified grid width.");

        // Construct the grid with all parsed elements.
        Grid grid(width, height, hHints, vHints);

        // Parse contents and fill the grid with it.
        tinyxml2::XMLElement* contentElt = findFirstChildOrThrow(gridElt, "content");
        parseGridContent(contentElt, grid);

        return grid;
    }

    void XMLGridSerialzer::loadXMLFile(std::string path, tinyxml2::XMLDocument& doc)
    {
        // Atempt to load the file and throw if any error is returned.
        tinyxml2::XMLError err = doc.LoadFile(path.c_str());
        if (err)
        {
            throw tinyxml2::TinyXML2Error(err);
        }
    }

    std::vector<std::vector<int>> XMLGridSerialzer::parseHintEntryCollection(tinyxml2::XMLElement* hintsElt)
    {
        std::vector<std::vector<int>> allHints;

        // Parse all "entry" elements in the provided node.
        tinyxml2::XMLElement* hintEntryElt = findFirstChildOrThrow(hintsElt, "entry");
        while (hintEntryElt)
        {
            std::vector<int> hints;

            // Parse all "hintValue" elements in the provided "entry" node.
            tinyxml2::XMLElement* hintValueElt = hintEntryElt->FirstChildElement("hintValue");
            while (hintValueElt)
            {
                // Get an int value and push it in the vector.
                hints.push_back(getValueFromText<int>(hintValueElt));
                hintValueElt = hintValueElt->NextSiblingElement("hintValue");
            }

            // Push the created vector into the big vector.
            allHints.push_back(hints);
            hintEntryElt = hintEntryElt->NextSiblingElement("entry");
        }

        return allHints;
    }

    void XMLGridSerialzer::parseGridContent(tinyxml2::XMLElement* contentElt, Grid& grid)
    {
        // Retrieve the default state.
        std::string defaultStateStr = getValueFromAttribute<std::string>(contentElt, "default");
        cell_t defaultState = stringToCellState(defaultStateStr);

        int width = grid.getWidth();
        int height = grid.getHeight();

        // Set the whole grid to that default state.
        grid.setCellRange(0, height - 1, 0, width - 1, defaultState);

        // Retrieve all cell values from XML.
        tinyxml2::XMLElement* cellElt = contentElt->FirstChildElement("cell");
        while (cellElt)
        {
            // Get cell coordinates.
            int row = getValueFromAttribute<int>(cellElt, "row");
            int col = getValueFromAttribute<int>(cellElt, "col");

            // Get cell state.
            std::string stateStr = getValueFromAttribute<std::string>(cellElt, "state");
            cell_t state = stringToCellState(stateStr);

            // Set the corresponding cell in the grid.
            try
            {
                grid.setCell(row, col, state);
            }
            catch(const IndexOutOfBoundsError& e)
            {
                throw InvalidXMLGridError("Specified cell coordinates exceed grid boundaries in provided file.");
            }

            cellElt = cellElt->NextSiblingElement("cell");
        }
    }

    cell_t XMLGridSerialzer::stringToCellState(std::string value)
    {
        if (value == "checked") return CELL_CHECKED;
        if (value == "cleared") return CELL_CLEARED;
        if (value == "crossed") return CELL_CROSSED;
        throw InvalidXMLGridError("String \"" + value + "\" cannot be bound to a cell_t value.");
    }

    void XMLGridSerialzer::saveXMLFile(tinyxml2::XMLDocument& doc, std::string path)
    {
        // Atempt to save to the specified file and throw if any error is returned.
        tinyxml2::XMLError err = doc.SaveFile(path.c_str());
        if (err)
        {
            throw tinyxml2::TinyXML2Error(err);
        }
    }

    void XMLGridSerialzer::addXMLHints(tinyxml2::XMLDocument& sourceDoc, tinyxml2::XMLElement* root, const std::vector<std::vector<int>>& hints)
    {
        // For every hint sequence...
        for (auto it = hints.begin(); it != hints.end(); it++)
        {
            // Generate an "entry" element.
            tinyxml2::XMLElement* entryElt = sourceDoc.NewElement("entry");
            root->InsertEndChild(entryElt);

            // For every hint in the sequence...
            for (auto jt = it->begin(); jt != it->end(); jt++)
            {
                // Generate a hint value.
                tinyxml2::XMLElement* valueElt = sourceDoc.NewElement("hintValue");
                valueElt->SetText(*jt);
                entryElt->InsertEndChild(valueElt);
            }
        }
    }

    void XMLGridSerialzer::addXMLGridContent(tinyxml2::XMLDocument& sourceDoc, tinyxml2::XMLElement* root, const Grid& grid)
    {
        // Compute and set the default cell value for the grid.
        cell_t defaultState = grid.mostPresentState();
        root->SetAttribute("default", cellStateToString(defaultState).c_str());

        int width = grid.getWidth();
        int height = grid.getHeight();
        // Iterate over all cells.
        for (int row = 0; row < height; row++)
        {
            for (int col = 0; col < width; col++)
            {
                cell_t state = grid.getCell(row, col);
                if (state != defaultState)
                {
                    // Generate a "cell" element for every cell whose state is different from the computed default.
                    tinyxml2::XMLElement* cellElt = sourceDoc.NewElement("cell");
                    cellElt->SetAttribute("row", row);
                    cellElt->SetAttribute("col", col);
                    cellElt->SetAttribute("state", cellStateToString(state).c_str());
                    root->InsertEndChild(cellElt);
                }
            }
        }
    }

    std::string XMLGridSerialzer::cellStateToString(cell_t value)
    {
        if (value == CELL_CHECKED) return "checked";
        if (value == CELL_CLEARED) return "cleared";
        if (value == CELL_CROSSED) return "crossed";
        throw InvalidXMLGridError("Unexpected cell_t value could not be represented into a string.");
    }

    tinyxml2::XMLElement* XMLGridSerialzer::findFirstChildOrThrow(tinyxml2::XMLElement* root, std::string name)
    {
        tinyxml2::XMLElement* elt = root->FirstChildElement(name.c_str());
        if (!elt)
        {
            throw InvalidXMLGridError("Missing element \"" + name + "\" in provided file.");
        }
        return elt;
    }
}
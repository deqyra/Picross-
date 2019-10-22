#include "xml_grid_serializer.hpp"

#include <string>
#include <stdexcept>
#include <vector>

#include "tinyxml2_error.hpp"
#include "invalid_xml_grid_error.hpp"
#include "../lib/tinyxml2/tinyxml2.hpp"
#include "../core/grid.hpp"
#include "../core/utility.hpp"

namespace Picross
{
    XMLGridSerialzer::XMLGridSerialzer()
    {

    }

    void XMLGridSerialzer::saveGridToFile(const Grid& grid, std::string path)
    {
        tinyxml2::XMLDocument xmlDoc;

        tinyxml2::XMLElement* gridElt = xmlDoc.NewElement("grid");
        gridElt->SetAttribute("width", grid.getWidth());
        gridElt->SetAttribute("height", grid.getHeight());
        xmlDoc.InsertFirstChild(gridElt);

        tinyxml2::XMLElement* hintsElt = xmlDoc.NewElement("hints");
        gridElt->InsertEndChild(hintsElt);

        tinyxml2::XMLElement* hHintsElt = xmlDoc.NewElement("horizontal");
        hintsElt->InsertEndChild(hHintsElt);
        addXMLHints(xmlDoc, hHintsElt, grid.getAllRowHints());

        tinyxml2::XMLElement* vHintsElt = xmlDoc.NewElement("vertical");
        hintsElt->InsertEndChild(vHintsElt);
        addXMLHints(xmlDoc, vHintsElt, grid.getAllColHints());

        tinyxml2::XMLElement* contentElt = xmlDoc.NewElement("content");
        gridElt->InsertEndChild(contentElt);
        addXMLGridContent(xmlDoc, contentElt, grid);

        saveXMLFile(xmlDoc, path);
    }

    Grid XMLGridSerialzer::loadGridFromFile(std::string path)
    {
        tinyxml2::XMLDocument doc;
        loadXMLFile(path, doc);

        tinyxml2::XMLElement* gridElt = doc.FirstChildElement("grid");
        if (!gridElt) throw InvalidXMLGridError("Missing root node \"grid\" in provided file.");

        int width = getValueFromAttribute<int>(gridElt, "width");
        int height = getValueFromAttribute<int>(gridElt, "height");

        tinyxml2::XMLElement* hintsElt = findFirstChildOrThrow(gridElt, "hints");
        tinyxml2::XMLElement* hHintsElt = findFirstChildOrThrow(hintsElt, "horizontal");
        tinyxml2::XMLElement* vHintsElt = findFirstChildOrThrow(hintsElt, "vertical");

        std::vector<std::vector<int>> hHints = parseHintEntryCollection(hHintsElt);
        if (hHints.size() != height) throw InvalidXMLGridError("Not enough horizontal hint entries for specified grid height.");

        std::vector<std::vector<int>> vHints = parseHintEntryCollection(vHintsElt);
        if (vHints.size() != width) throw InvalidXMLGridError("Not enough vertical hint entries for specified grid width.");

        Grid grid(width, height, hHints, vHints);

        tinyxml2::XMLElement* contentElt = findFirstChildOrThrow(gridElt, "content");
        parseGridContent(contentElt, grid);

        return grid;
    }

    void XMLGridSerialzer::loadXMLFile(std::string path, tinyxml2::XMLDocument& doc)
    {
        tinyxml2::XMLError err = doc.LoadFile(path.c_str());
        if (err)
        {
            throw tinyxml2::TinyXML2Error(err);
        }
    }

    std::vector<std::vector<int>> XMLGridSerialzer::parseHintEntryCollection(tinyxml2::XMLElement* hintsElt)
    {
        std::vector<std::vector<int>> allHints;
        tinyxml2::XMLElement* hintEntryElt = findFirstChildOrThrow(hintsElt, "entry");
        while (hintEntryElt)
        {
            std::vector<int> hints;

            tinyxml2::XMLElement* hintValueElt = hintEntryElt->FirstChildElement("hintValue");
            while (hintValueElt)
            {
                hints.push_back(getValueFromText<int>(hintValueElt));
                hintValueElt = hintValueElt->NextSiblingElement("hintValue");
            }

            allHints.push_back(hints);
            hintEntryElt = hintEntryElt->NextSiblingElement("entry");
        }

        return allHints;
    }

    void XMLGridSerialzer::parseGridContent(tinyxml2::XMLElement* contentElt, Grid& grid)
    {
        std::string defaultStateStr = getValueFromAttribute<std::string>(contentElt, "default");
        cell_t defaultState = stringToCellState(defaultStateStr);

        int width = grid.getWidth();
        int height = grid.getHeight();

        for (int row = 0; row < height; row++)
        {
            for (int col = 0; col < width; col++)
            {
                grid.setCell(row, col, defaultState);
            }
        }

        tinyxml2::XMLElement* cellElt = findFirstChildOrThrow(contentElt, "entry");
        while (cellElt)
        {
            int row = getValueFromAttribute<int>(cellElt, "row");
            int col = getValueFromAttribute<int>(cellElt, "col");
            std::string stateStr = getValueFromAttribute<std::string>(cellElt, "state");
            cell_t state = stringToCellState(stateStr);

            try
            {
                grid.setCell(row, col, state);
            }
            catch(const std::runtime_error& e)
            {
                throw InvalidXMLGridError("Specified cell coordinates exceed grid boundaries in provided file.");
            }
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
        tinyxml2::XMLError err = doc.SaveFile(path.c_str());
        if (err)
        {
            throw tinyxml2::TinyXML2Error(err);
        }
    }

    void XMLGridSerialzer::addXMLHints(tinyxml2::XMLDocument& sourceDoc, tinyxml2::XMLElement* root, const std::vector<std::vector<int>>& hints)
    {
        for (auto it = hints.begin(); it != hints.end(); it++)
        {
            tinyxml2::XMLElement* entryElt = sourceDoc.NewElement("entry");
            root->InsertEndChild(entryElt);

            for (auto jt = it->begin(); jt != it->end(); jt++)
            {
                tinyxml2::XMLElement* valueElt = sourceDoc.NewElement("hintValue");
                valueElt->SetText(*jt);
                entryElt->InsertEndChild(valueElt);
            }
        }
    }

    void XMLGridSerialzer::addXMLGridContent(tinyxml2::XMLDocument& sourceDoc, tinyxml2::XMLElement* root, const Grid& grid)
    {
        int width = grid.getWidth();
        int height = grid.getHeight();
        cell_t defaultState = mostPresentState(grid);
        root->SetAttribute("default", cellStateToString(defaultState).c_str());

        for (int row = 0; row < height; row++)
        {
            for (int col = 0; col < width; col++)
            {
                cell_t state = grid.getCell(row, col);
                if (state != defaultState)
                {
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
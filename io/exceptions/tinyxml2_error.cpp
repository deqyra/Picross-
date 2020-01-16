#include "tinyxml2_error.hpp"

namespace tinyxml2
{
    TinyXML2Error::TinyXML2Error(const char* message) :
        _message(message)
    {

    }

    TinyXML2Error::TinyXML2Error(std::string message) :
        _message(message)
    {

    }
    
    TinyXML2Error::TinyXML2Error(XMLError err) :
        _message(xmlErrorToString(err))
    {

    }

    TinyXML2Error::TinyXML2Error(XMLError err, const char* message) :
        _message(xmlErrorToString(err) + ": " + message)
    {

    }

    TinyXML2Error::TinyXML2Error(XMLError err, std::string message) :
        _message(xmlErrorToString(err) + ": " + message)
    {
        
    }

    const char* TinyXML2Error::what() const noexcept
    {
        return _message.c_str();
    }

    std::string xmlErrorToString(XMLError err)
    {
        switch (err)
        {
            case XML_SUCCESS:
                return "XML_SUCCESS";
                break;
            case XML_NO_ATTRIBUTE:
                return "XML_NO_ATTRIBUTE";
                break;
            case XML_WRONG_ATTRIBUTE_TYPE:
                return "XML_WRONG_ATTRIBUTE_TYPE";
                break;
            case XML_ERROR_FILE_NOT_FOUND:
                return "XML_ERROR_FILE_NOT_FOUND";
                break;
            case XML_ERROR_FILE_COULD_NOT_BE_OPENED:
                return "XML_ERROR_FILE_COULD_NOT_BE_OPENED";
                break;
            case XML_ERROR_FILE_READ_ERROR:
                return "XML_ERROR_FILE_READ_ERROR";
                break;
            case XML_ERROR_PARSING_ELEMENT:
                return "XML_ERROR_PARSING_ELEMENT";
                break;
            case XML_ERROR_PARSING_ATTRIBUTE:
                return "XML_ERROR_PARSING_ATTRIBUTE";
                break;
            case XML_ERROR_PARSING_TEXT:
                return "XML_ERROR_PARSING_TEXT";
                break;
            case XML_ERROR_PARSING_CDATA:
                return "XML_ERROR_PARSING_CDATA";
                break;
            case XML_ERROR_PARSING_COMMENT:
                return "XML_ERROR_PARSING_COMMENT";
                break;
            case XML_ERROR_PARSING_DECLARATION:
                return "XML_ERROR_PARSING_DECLARATION";
                break;
            case XML_ERROR_PARSING_UNKNOWN:
                return "XML_ERROR_PARSING_UNKNOWN";
                break;
            case XML_ERROR_EMPTY_DOCUMENT:
                return "XML_ERROR_EMPTY_DOCUMENT";
                break;
            case XML_ERROR_MISMATCHED_ELEMENT:
                return "XML_ERROR_MISMATCHED_ELEMENT";
                break;
            case XML_ERROR_PARSING:
                return "XML_ERROR_PARSING";
                break;
            case XML_CAN_NOT_CONVERT_TEXT:
                return "XML_CAN_NOT_CONVERT_TEXT";
                break;
            case XML_NO_TEXT_NODE:
                return "XML_NO_TEXT_NODE";
                break;
            case XML_ELEMENT_DEPTH_EXCEEDED:
                return "XML_ELEMENT_DEPTH_EXCEEDED";
                break;
            case XML_ERROR_COUNT:
                return "XML_ERROR_COUNT";
                break;
            default:
                return "Unknow XML error";
        }
    }
} // namespace tinyxml2

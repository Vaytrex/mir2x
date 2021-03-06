/*
 * =====================================================================================
 *
 *       Filename: xmlparagraphleaf.cpp
 *        Created: 12/22/2018 07:38:31
 *    Description:
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#include <utf8.h>
#include <tinyxml2.h>

#include "log.hpp"
#include "bevent.hpp"
#include "xmlfunc.hpp"
#include "utf8func.hpp"
#include "colorf.hpp"
#include "xmlparagraphleaf.hpp"

extern Log *g_Log;

XMLParagraphLeaf::XMLParagraphLeaf(tinyxml2::XMLNode *pNode)
    : m_Node([pNode]()
      {
          if(pNode == nullptr){
              throw std::invalid_argument(str_fflprintf(": Invalid argument: (nullptr)"));
          }

          if(!XMLFunc::CheckValidLeaf(pNode)){
              throw std::invalid_argument(str_fflprintf(": Invalid argument: not a valid XMParagraph leaf"));
          }
          return pNode;
      }())
    , m_type([this]()
      {
          if(XMLFunc::CheckTextLeaf(xmlNode())){
              if(!utf8::is_valid(xmlNode()->Value(), xmlNode()->Value() + std::strlen(xmlNode()->Value()))){
                  throw fflerror("not a utf8 string: %s", xmlNode()->Value());
              }
              return LEAF_UTF8GROUP;
          }

          if(XMLFunc::CheckEmojiLeaf(xmlNode())){
              return LEAF_EMOJI;
          }

          if(XMLFunc::CheckImageLeaf(xmlNode())){
              return LEAF_IMAGE;
          }

          throw fflerror("invalid argument: node type not recognized");
      }())
    , m_U64Key([this]() -> uint64_t
      {
          switch(m_type){
              case LEAF_EMOJI:
                  {
                      int emojiId = 0;
                      if((m_Node->ToElement()->QueryIntAttribute("id", &emojiId) != tinyxml2::XML_SUCCESS) &&
                         (m_Node->ToElement()->QueryIntAttribute("Id", &emojiId) != tinyxml2::XML_SUCCESS) &&
                         (m_Node->ToElement()->QueryIntAttribute("ID", &emojiId) != tinyxml2::XML_SUCCESS)){

                          // no id attribute
                          // use default emojiId zero
                      }
                      return emojiId;
                  }
              default:
                  {
                      return 0;
                  }
          }
      }())
    , m_Event(BEVENT_OFF)
{
    if(Type() == LEAF_UTF8GROUP){
        m_UTF8CharOff = UTF8Func::buildUTF8Off(UTF8Text());
    }
}

void XMLParagraphLeaf::markEvent(int nEvent)
{
    switch(nEvent){
        case BEVENT_ON:
        case BEVENT_OFF:
        case BEVENT_DOWN:
            {
                break;
            }
        default:
            {
                throw std::invalid_argument(str_fflprintf(": Invalid event: %d", nEvent));
            }
    }
    m_Event = nEvent;
}

uint32_t XMLParagraphLeaf::peekUTF8Code(int leafOff) const
{
    if(leafOff >= length()){
        throw std::invalid_argument(str_fflprintf(": Provided LeafOff exceeds leaf length: %zu", length()));
    }

    if(Type() != LEAF_UTF8GROUP){
        throw std::runtime_error(str_fflprintf(": Try peek utf8 code from a leaf with type: %d", Type()));
    }

    return UTF8Func::peekUTF8Code(xmlNode()->Value() + utf8CharOffRef()[leafOff]);
}

std::optional<uint32_t> XMLParagraphLeaf::Color() const
{
    if(const auto pszColor = XMLFunc::FindAttribute(xmlNode(), "font_color", true)){
        try{
            return colorf::String2RGBA(pszColor);
        }catch(...){}
    }
    return {};
}

std::optional<uint32_t> XMLParagraphLeaf::BGColor() const
{
    if(const auto pszBGColor = XMLFunc::FindAttribute(xmlNode(), "font_bgcolor", true)){
        try{
            return colorf::String2RGBA(pszBGColor);
        }catch(...){}
    }
    return {};
}

std::optional<uint8_t> XMLParagraphLeaf::Font() const
{
    if(const auto pszFont = XMLFunc::FindAttribute(xmlNode(), "font", true)){
        try{
            if(auto nFontIndex = std::atoi(pszFont); (nFontIndex < 0 || nFontIndex > 255)){
                throw std::runtime_error(str_fflprintf(": Invalid font index, not an uint8_t: %d", nFontIndex));
            }else{
                return (uint8_t)(nFontIndex);
            }
        }catch(const std::exception &e){
            g_Log->addLog(LOGTYPE_DEBUG, "Caught exception: %s", e.what());
        }catch(...){
            g_Log->addLog(LOGTYPE_DEBUG, "Caught unknown exception");
        }
    }
    return {};
}

std::optional<uint8_t> XMLParagraphLeaf::FontSize() const
{
    if(const auto pszFontSize = XMLFunc::FindAttribute(xmlNode(), "font_size", true)){
        try{
            if(auto nFontSize = std::atoi(pszFontSize); (nFontSize < 0 || nFontSize > 255)){
                throw std::runtime_error(str_fflprintf(": Invalid font size, not an uint8_t: %d", nFontSize));
            }else{
                return (uint8_t)(nFontSize);
            }
        }catch(const std::exception &e){
            g_Log->addLog(LOGTYPE_DEBUG, "Caught exception: %s", e.what());
        }catch(...){
            g_Log->addLog(LOGTYPE_DEBUG, "Caught unknown exception");
        }
    }
    return {};
}

std::optional<uint8_t> XMLParagraphLeaf::FontStyle() const
{
    if(const auto pszFontStyle = XMLFunc::FindAttribute(xmlNode(), "font_style", true)){
        try{
            if(auto nFontStyle = std::atoi(pszFontStyle); (nFontStyle < 0 || nFontStyle > 255)){
                throw std::runtime_error(str_fflprintf(": Invalid font style, not an uint8_t: %d", nFontStyle));
            }else{
                return (uint8_t)(nFontStyle);
            }
        }catch(const std::exception &e){
            g_Log->addLog(LOGTYPE_DEBUG, "Caught exception: %s", e.what());
        }catch(...){
            g_Log->addLog(LOGTYPE_DEBUG, "Caught unknown exception");
        }
    }
    return {};
}

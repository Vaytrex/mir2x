/*
 * =====================================================================================
 *
 *       Filename: npcchatboard.hpp
 *        Created: 04/12/2020 18:56:22
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

#pragma once
#include <cstdint>
#include "widget.hpp"
#include "layoutboard.hpp"
#include "tritexbutton.hpp"

class ProcessRun;
class NPCChatBoard: public Widget
{
    private:
        int m_margin;
        ProcessRun *m_processRun;

    private:
        LayoutBoard  m_chatBoard;
        TritexButton m_buttonClose;

    private:
        uint64_t m_NPCUID;

    public:
        NPCChatBoard(ProcessRun *);

    public:
        void drawEx(int, int, int, int, int, int) override;

    private:
        void drawPlain();
        void drawWithNPCFace();

    public:
        void loadXML(const char *xmlString)
        {
            m_chatBoard.loadXML(xmlString);
        }
};

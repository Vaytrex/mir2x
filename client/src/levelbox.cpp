/*
 * =====================================================================================
 *
 *       Filename: levelbox.cpp
 *        Created: 03/28/2020 05:47:00
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

#include "levelbox.hpp"
#include "sdldevice.hpp"

LevelBox::LevelBox(
        int x,
        int y,

        const std::function<void(int)> &onDrag,
        const std::function<void(   )> &onDoubleClick,

        Widget *parent,
        bool autoDelete)
    : Widget(x, y, 0, 0, parent, autoDelete)
    , m_label
      {
          0,
          0,
          "",
          0,
          12,
          0,
          colorf::YELLOW + 128,
          this,
          false,
      }
    , m_texture([]()
      {
          const int r = 8;
          const int w = r * 2 - 1;
          const int h = r * 2 - 1;

          std::vector<uint32_t> buf(w * h);
          for(int y = 0; y < h; ++y){
              for(int x = 0; x < w; ++x){
                  const int currR2 = (x - r + 1) * (x - r + 1) + (y - r + 1) * (y - r + 1);
                  const uint8_t alpha = 255 - std::min<uint8_t>(255, std::lround(255.0 * currR2 / (r * r)));
                  if(currR2 < r * r){
                      buf[x + y * w] = colorf::RGBA(0XFF, 0XFF, 0XFF, alpha);
                  }
                  else{
                      buf[x + y * w] = colorf::RGBA(0, 0, 0, 0);
                  }
              }
          }

          if(auto p = g_SDLDevice->createTexture(buf.data(), w, h)){
              return p;
          }
          throw fflerror("failed to create texture");
      }())
    , m_onDrag(onDrag)
    , m_onDoubleClick(onDoubleClick)
{
    setLevel(0);
}

bool LevelBox::processEvent(const SDL_Event &event, bool valid)
{
    if(!valid){
        return false;
    }

    switch(event.type){
        case SDL_MOUSEBUTTONDOWN:
            {
                if(!in(event.button.x, event.button.y)){
                    m_state = BEVENT_OFF;
                    return false;
                }

                if(event.button.clicks == 2){
                    m_onDoubleClick();
                }
                m_state = BEVENT_DOWN;
                return true;
            }
        case SDL_MOUSEBUTTONUP:
            {
                if(!in(event.button.x, event.button.y)){
                    m_state = BEVENT_OFF;
                    return false;
                }

                m_state = BEVENT_ON;
                return true;
            }
        case SDL_MOUSEMOTION:
            {
                // even not in the box
                // we still need to drag the widget

                if(m_state == BEVENT_DOWN){
                    if(event.motion.state & SDL_BUTTON_LMASK){
                        m_onDrag(event.motion.yrel);
                        return true;
                    }
                    else{
                        if(in(event.motion.x, event.motion.y)){
                            m_state = BEVENT_ON;
                            return true;
                        }
                        else{
                            m_state = BEVENT_OFF;
                            return false;
                        }
                    }
                }

                if(in(event.motion.x, event.motion.y)){
                    m_state = BEVENT_ON;
                    return true;
                }
                else{
                    m_state = BEVENT_OFF;
                    return false;
                }
            }
        default:
            {
                return false;
            }
    }
}

void LevelBox::drawEx(int dstX, int dstY, int, int, int, int)
{
    // don't worry too much here
    // we always draw fully for LevelBox

    int bgW = -1;
    int bgH = -1;
    SDL_QueryTexture(m_texture, 0, 0, &bgW, &bgH);

    switch(m_state){
        case BEVENT_ON:
            {
                SDL_SetTextureColorMod(m_texture, 0XFF, 0, 0);
                SDLDevice::EnableDrawBlendMode enableDrawBlendMode(SDL_BLENDMODE_BLEND);
                g_SDLDevice->DrawTexture(m_texture, dstX + 1, dstY, 0, 0, bgW, bgH);
                break;
            }
        case BEVENT_DOWN:
            {
                SDL_SetTextureColorMod(m_texture, 0, 0, 0XFF);
                SDLDevice::EnableDrawBlendMode enableDrawBlendMode(SDL_BLENDMODE_BLEND);
                g_SDLDevice->DrawTexture(m_texture, dstX + 1, dstY, 0, 0, bgW, bgH);
                break;
            }
        default:
            {
                break;
            }
    }

    const int dx = (w() - m_label.w()) / 2;
    const int dy = (h() - m_label.h()) / 2;

    m_label.drawEx(dstX + dx, dstY + dy, 0, 0, m_label.w(), m_label.h());
}

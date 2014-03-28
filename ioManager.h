#ifndef SINGLE__H
#define SINGLE__H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <sstream>
#include <map>
#include "gamedata.h"
#include "xmlparser.h"

class IOManager {
public:
  static IOManager& getInstance();
  SDL_Surface * getScreen() const { return screen; }
  ~IOManager() 
  { 
      for(std::map<std::string, TTF_Font*>::const_iterator it=fonts.begin(); it!= fonts.end(); ++it)
      {
        TTF_CloseFont((*it).second); 
      }
  }

  void parseFonts();
  void parseColors();

  SDL_Surface* loadAndSet(const std::string& filename, 
                          bool setcolorkey) const;

  void printMessageAt(const std::string& msg, Uint32 x, Uint32 y) const;
  void printMessageCenteredAt(const std::string& msg, Uint32 y) const;
  void printStringAfterMessage(const std::string&, 
                               Uint32 x, Uint32 y) const;
  template <typename T>
  void printMessageValueAt(const std::string& msg, T value, 
         Uint32 x, Uint32 y) const;
         
  void buildString(SDL_Event);
  void clearString() { inputString = ""; }
  const std::string& getString() const { return inputString; }
private:
  IOManager();
  IOManager(const IOManager&);
  IOManager& operator=(const IOManager&);
  XMLParser parser;
  const Gamedata& gdata;
  int viewWidth;
  int viewHeight;
  const std::string DEFAULT_FONT;
  const std::string DEFAULT_COLOR;
  const unsigned MAX_STRING_SIZE;
  SDL_Surface * screen;
  std::map<const std::string, TTF_Font *> fonts;
  std::map<const std::string, SDL_Color> colors;
  std::string title;
  std::string inputString;
};

template <typename T>
void IOManager::printMessageValueAt(const std::string& msg, T value, 
     Uint32 x, Uint32 y) const {
   std::stringstream strm;
   std::string message = msg;
   strm << message << value << "\0";
   message = strm.str();
   SDL_Rect dest = {x,y,0,0};
   SDL_Surface *stext = 
       TTF_RenderText_Blended(fonts.find(DEFAULT_FONT)->second, message.c_str(), colors.find(DEFAULT_COLOR)->second);
   if (stext) {
     SDL_BlitSurface( stext, NULL, screen, &dest );
     SDL_FreeSurface(stext);
   }
   else {
     throw 
     std::string("Couldn't allocate text sureface in printMessageValueAt");
   }
}

#endif

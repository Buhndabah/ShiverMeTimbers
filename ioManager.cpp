#include <iostream>
#include <iomanip>
#include "ioManager.h"
using std::string;

IOManager& IOManager::getInstance() {
  static IOManager instance;
  return instance;
}

IOManager::IOManager( ) :
  parser( XMLParser::getInstance() ),
  gdata( Gamedata::getInstance() ),
  viewWidth( gdata.getXmlInt("viewWidth") ),
  viewHeight( gdata.getXmlInt("viewHeight") ),
  DEFAULT_FONT(gdata.getXmlStr("defaultFont") ),
  DEFAULT_COLOR(gdata.getXmlStr("defaultColor") ),
  MAX_STRING_SIZE( gdata.getXmlInt("maxStringSize") ),
    // The 3rd and 4th parameters are just as important as the first 2!
    screen(SDL_SetVideoMode(viewWidth, viewHeight, 32, SDL_DOUBLEBUF)),
    fonts(), 
    colors(),
    title( gdata.getXmlStr("screenTitle") ),
    inputString()  
{
  if (screen == NULL) { 
    throw string("Unable to set video mode; screen is NULL in IOMAnager"); 
  }
  if ( TTF_Init() == -1 ) {
    throw string("TTF_Init failed: ") + TTF_GetError();
  }
  parser.parse("xmlSpec/font.xml");
  parseFonts();
  parseColors();
  parser.removeDoc("xmlSpec/font.xml");
  SDL_EnableUNICODE( SDL_ENABLE );
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(TTF_Quit);
}

void IOManager::parseFonts() {
    std::list<std::map<std::string,std::string> > fontList = parser.parseNodesWithTag("font");
    std::map<std::string,std::string> font;
    for(std::list<std::map<std::string,std::string> >::const_iterator it = fontList.begin(); it!= fontList.end(); ++it)
    {
        font = (*it);
        fonts.insert(std::pair<std::string, TTF_Font*>(font["name"],TTF_OpenFont(
            font["file"].c_str(),
            atoi(font["size"].c_str()))));
        if(fonts.find(font["name"])==fonts.end())
        {
            throw string("TTF_OpenFont failed: ") + TTF_GetError();
        }
    }
}

void IOManager::parseColors() {
    std::list<std::map<std::string, std::string> > colorList = parser.parseNodesWithTag("color");
    SDL_Color newColor;
    std::map<std::string, std::string> colorParams;
    for(std::list<std::map<std::string,std::string> >::const_iterator it= colorList.begin(); it!= colorList.end(); ++it)
    {
        colorParams=(*it);
        newColor.r=atoi(colorParams["r"].c_str());
        newColor.g=atoi(colorParams["g"].c_str());
        newColor.b=atoi(colorParams["b"].c_str());
        newColor.unused=atoi(colorParams["unused"].c_str());

        colors.insert(std::pair<std::string, SDL_Color>(colorParams["name"], newColor));
    }
}

SDL_Surface* IOManager::loadAndSet(const string& filename, bool setcolorkey) const {
  SDL_Surface *tmp = IMG_Load(filename.c_str());
  if (tmp == NULL) {
    throw string("Unable to load bitmap ")+filename;
  }
  if ( setcolorkey ) {
    Uint32 colorkey = SDL_MapRGB(tmp->format, 255, 0, 255);
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY|SDL_RLEACCEL, colorkey);
  }
  // Optimize the strip for fast display
  SDL_Surface *image = SDL_DisplayFormatAlpha(tmp);
  if (image == NULL) {
    image = tmp;
  } 
  else {
    SDL_FreeSurface(tmp);
  }
  return image;
}

void IOManager::printMessageAt(const string& msg, Uint32 x, Uint32 y, const std::string& font, const std::string& color)  const{

   if(fonts.find(font)==fonts.end())
   {
       throw std::string("Unknown font ")+font+std::string("requested");
   }
   if(colors.find(color)==colors.end())
   {
       throw std::string("Unknown color ")+color+std::string("requested");
   }

   SDL_Rect dest = {x,y,0,0};
   SDL_Surface * stext = TTF_RenderText_Blended(fonts.find(font)->second, msg.c_str(), colors.find(color)->second);
   if (stext) {
     SDL_BlitSurface( stext, NULL, screen, &dest );
     SDL_FreeSurface(stext);
   }
   else {
     throw 
     string("Couldn't allocate text sureface in printMessageAt");
   }
}

void IOManager::printMessageCenteredAt( const string& msg, Uint32 y,const std::string& font, const std::string& color) const{
    if(fonts.find(font)==fonts.end())
    {
        throw std::string("Unknown font ")+font+std::string("requested");
    }
    if(colors.find(color)==colors.end())
    {
        throw std::string("Unknown color ")+color+std::string("requested");
    }
   SDL_Surface *stext = TTF_RenderText_Blended(fonts.find(font)->second, msg.c_str(), colors.find(color)->second);
   if (stext) {
     Uint32 x = ( viewWidth - stext->w ) / 2;
     SDL_Rect dest = {x,y,0,0};
     SDL_BlitSurface( stext, NULL, screen, &dest );
     SDL_FreeSurface(stext);
   }
   else {
     throw 
     string("Couldn't allocate text sureface in printMessageCenteredAt");
   }
}

void IOManager::printStringAfterMessage( const string& msg,
       Uint32 x, Uint32 y ) const {
   printMessageAt(msg+inputString, x, y);
}

void IOManager::buildString(SDL_Event event) {
  if( inputString.size() <= MAX_STRING_SIZE) {
    unsigned ch = event.key.keysym.sym;
    if ( isalpha(ch) || isdigit(ch) || ch == ' ') {
      inputString += char(event.key.keysym.unicode);
    }
  }     
  if( event.key.keysym.sym == SDLK_BACKSPACE
      && inputString.length() > 0 ) {
      // remove a character from the end
      int length = inputString.size();
      inputString.erase( length - 1 );
  }
}


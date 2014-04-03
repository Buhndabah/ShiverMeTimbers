import pygame
from pygame.locals import *

from globals import E_VARS,STRINGS,COLORS

class Player:
    
    #private vars
    __coords={}
    __direction=''    # used in movement, will also later be used in facing

    def __init__(self,w_width,w_height):
        self.__coords={STRINGS.X:w_width/2,STRINGS.Y:w_height/2}

    def move(self):
        if self.__direction==STRINGS.UP:
            self.__coords[STRINGS.Y]=self.__coords[STRINGS.Y]-E_VARS.CELLHEIGHT/2
        elif self.__direction==STRINGS.DOWN:
            self.__coords[STRINGS.Y]=self.__coords[STRINGS.Y]+E_VARS.CELLHEIGHT/2
        elif self.__direction==STRINGS.LEFT:
            self.__coords[STRINGS.X]=self.__coords[STRINGS.X]-E_VARS.CELLWIDTH/2
        elif self.__direction==STRINGS.RIGHT:
            self.__coords[STRINGS.X]=self.__coords[STRINGS.X]+E_VARS.CELLWIDTH/2

    def setDir(self,key):
        if key==K_LEFT or key==K_a:
            self.__direction=STRINGS.LEFT
        elif key==K_RIGHT or key==K_d:
            self.__direction=STRINGS.RIGHT
        elif key==K_UP or key==K_w:
            self.__direction=STRINGS.UP
        elif key==K_DOWN or key==K_s:
            self.__direction=STRINGS.DOWN
        else:
            self.__direction=''
        self.move()
        
    def getCoords(self):
        return self.__coords

    def draw(self,DISPLAYSURF):
        x = self.__coords[STRINGS.X]
        y = self.__coords[STRINGS.Y]
        '''pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x,y),(x+E_VARS.CELLWIDTH,y),4)
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x,y),(x,y+E_VARS.CELLHEIGHT),4)
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x+E_VARS.CELLWIDTH,y),(x+E_VARS.CELLWIDTH,y+E_VARS.CELLHEIGHT),4)
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x,y+E_VARS.CELLHEIGHT),(x+E_VARS.CELLWIDTH,y+E_VARS.CELLHEIGHT),4)

        pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x,y),(x+E_VARS.CELLWIDTH/3, y+E_VARS.CELLHEIGHT*1.5-10),4)    
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x+E_VARS.CELLWIDTH,y),(x+2*E_VARS.CELLWIDTH/3,y+E_VARS.CELLHEIGHT*1.5-10),4)
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x+E_VARS.CELLWIDTH,y+E_VARS.CELLHEIGHT),(x+2*E_VARS.CELLWIDTH/3-5,y+E_VARS.CELLHEIGHT*1.5+10),4)
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x,y+E_VARS.CELLHEIGHT),(x+E_VARS.CELLWIDTH/3+5,y+E_VARS.CELLHEIGHT*1.5+10),4)
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x+E_VARS.CELLWIDTH/3,y+E_VARS.CELLHEIGHT*1.5-10),(x+E_VARS.CELLWIDTH/3+5,y+E_VARS.CELLHEIGHT*1.5+10),4)
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x+E_VARS.CELLWIDTH/3,y+E_VARS.CELLHEIGHT*1.5-10),(x+2*E_VARS.CELLWIDTH/3, y+E_VARS.CELLHEIGHT*1.5-10),4)
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x+2*E_VARS.CELLWIDTH/3,y+E_VARS.CELLHEIGHT*1.5-10),(x+2*E_VARS.CELLWIDTH/3-5,y+E_VARS.CELLHEIGHT*1.5+10),4)
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK,(x+2*E_VARS.CELLWIDTH/3-5,y+E_VARS.CELLHEIGHT*1.5+10),(x+E_VARS.CELLWIDTH/3+5,y+E_VARS.CELLHEIGHT*1.5+10),4)'''


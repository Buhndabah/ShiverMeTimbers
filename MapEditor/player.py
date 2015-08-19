import copy
import pygame
from pygame.locals import *

from globals import E_VARS,STRINGS,COLORS

class Player:

    def __init__(self):
        self.__mapX=0
        self.__mapY=0
        self.__level=0
        self.__coords={STRINGS.X:E_VARS().W_WIDTH/2-E_VARS().CELLWIDTH/2,STRINGS.Y:E_VARS().W_HEIGHT/2-E_VARS().CELLHEIGHT*E_VARS().MAPHEIGHT/2}

    def move(self,key):
        if key==K_UP or key==K_w:
            self.__coords[STRINGS.Y]=self.__coords[STRINGS.Y]-E_VARS().CELLHEIGHT/2
            self.__mapX=self.__mapX-0.5
            self.__mapY=self.__mapY-0.5
        elif key==K_DOWN or key==K_s:
            self.__coords[STRINGS.Y]=self.__coords[STRINGS.Y]+E_VARS().CELLHEIGHT/2
            self.__mapX=self.__mapX+0.5
            self.__mapY=self.__mapY+0.5
        elif key==K_LEFT or key==K_a:
            self.__coords[STRINGS.X]=self.__coords[STRINGS.X]-E_VARS().CELLWIDTH/2
            self.__mapX=self.__mapX+0.5
            self.__mapY=self.__mapY-0.5
        elif key==K_RIGHT or key==K_d:
            self.__coords[STRINGS.X]=self.__coords[STRINGS.X]+E_VARS().CELLWIDTH/2
            self.__mapX=self.__mapX-0.5
            self.__mapY=self.__mapY+0.5

    @property
    def mapX(self):
        return self.__mapX

    @property
    def mapY(self):
        return self.__mapY

    @property
    def level(self):
        return self.__level

    @level.setter
    def level(self, newVal):
        self.__level = newVal

    @property
    def coords(self):
        return copy.copy(self.__coords)

    def getMapPos(self):
        return copy.copy([self.__mapX, self.__mapY])


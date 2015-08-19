import copy
import pygame
from pygame.locals import *

from globals import E_VARS,STRINGS,COLORS

class Player:

    def __init__(self):
        self.__mapX=0
        self.__mapY=0
        self.__level=0
        self.__coords={STRINGS.X:E_VARS().getWinWidth()/2-E_VARS().getCellWidth()/2,STRINGS.Y:E_VARS().getWinHeight()/2-E_VARS().getCellHeight()*E_VARS().getMapHeight()/2}

    def move(self,key):
        if key==K_UP or key==K_w:
            self.__coords[STRINGS.Y]=self.__coords[STRINGS.Y]-E_VARS().getCellHeight()/2
            self.__mapX=self.__mapX-0.5
            self.__mapY=self.__mapY-0.5
        elif key==K_DOWN or key==K_s:
            self.__coords[STRINGS.Y]=self.__coords[STRINGS.Y]+E_VARS().getCellHeight()/2
            self.__mapX=self.__mapX+0.5
            self.__mapY=self.__mapY+0.5
        elif key==K_LEFT or key==K_a:
            self.__coords[STRINGS.X]=self.__coords[STRINGS.X]-E_VARS().getCellWidth()/2
            self.__mapX=self.__mapX+0.5
            self.__mapY=self.__mapY-0.5
        elif key==K_RIGHT or key==K_d:
            self.__coords[STRINGS.X]=self.__coords[STRINGS.X]+E_VARS().getCellWidth()/2
            self.__mapX=self.__mapX-0.5
            self.__mapY=self.__mapY+0.5

    def goUp(self):
        self.__level = self.__level+1

    def goDown(self):
        self.__level = self.__level-1

    def getLevel(self):
        return self.__level
        
    def getCoords(self):
        return copy.copy(self.__coords)

    def getMapPos(self):
        return copy.copy([self.__mapX, self.__mapY])


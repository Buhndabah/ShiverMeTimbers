import copy
import pygame
from pygame.locals import *

from globals import E_VARS,STRINGS,COLORS

class Player:
    
    #private vars
    __coords={}
    __direction=''    # used in movement, will also later be used in facing
    __mapX=0
    __mapY=0
    __level=0
    __vars=0

    def __init__(self,env):
        self.__vars=env
        self.__mapX=0
        self.__mapY=0
        self.__level=0
        self.__coords={STRINGS.X:self.__vars.getWinWidth()/2-self.__vars.getCellWidth()/2,STRINGS.Y:self.__vars.getWinHeight()/2-self.__vars.getCellHeight()*self.__vars.getMapHeight()/2}

    def move(self):
        if self.__direction==STRINGS.UP:
            self.__coords[STRINGS.Y]=self.__coords[STRINGS.Y]-self.__vars.getCellHeight()/2
            self.__mapX=self.__mapX-0.5
            self.__mapY=self.__mapY-0.5
        elif self.__direction==STRINGS.DOWN:
            self.__coords[STRINGS.Y]=self.__coords[STRINGS.Y]+self.__vars.getCellHeight()/2
            self.__mapX=self.__mapX+0.5
            self.__mapY=self.__mapY+0.5
        elif self.__direction==STRINGS.LEFT:
            self.__coords[STRINGS.X]=self.__coords[STRINGS.X]-self.__vars.getCellWidth()/2
            self.__mapX=self.__mapX+0.5
            self.__mapY=self.__mapY-0.5
        elif self.__direction==STRINGS.RIGHT:
            self.__coords[STRINGS.X]=self.__coords[STRINGS.X]+self.__vars.getCellWidth()/2
            self.__mapX=self.__mapX-0.5
            self.__mapY=self.__mapY+0.5

    def goUp(self):
        self.__level = self.__level+1

    def goDown(self):
        self.__level = self.__level-1

    def getLevel(self):
        return self.__level

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
        return copy.copy(self.__coords)

    def getMapPos(self):
        return copy.copy([self.__mapX, self.__mapY])


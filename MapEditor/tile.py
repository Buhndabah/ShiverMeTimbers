import pygame, os
from pygame.locals import *

from globals import E_VARS

class Tile:

    def __init__(self, name=None):
        if name:
            self.__path = name
            self.__pic = pygame.image.load(name)
            self.__name = name.split('.')[0]
            temp = self.__name.split('/')
            self.__name = temp[len(temp)-1]
        else:
            self.__path = E_VARS().getPath() + "defaults/empty.png"
            self.__pic = pygame.image.load(self.__path)
            self.__name= "empty"
    
    def getPic(self):
        return self.__pic

    def getName(self):
        return self.__name

    def getWidth(self):
        return self.__pic.get_width()

    def getHeight(self):
        return self.__pic.get_height()

    @staticmethod
    def color_surface(surface, r, g, b):
        arr = pygame.surfarray.pixels3d(surface)
        arr[:,:,0] = arr[:,:,0] - r
        arr[:,:,1] = arr[:,:,1] - g
        arr[:,:,2] = arr[:,:,2] - b


    @staticmethod
    def loadTiles():
        tileList=[]
        listing = os.listdir(E_VARS().getPath())
        tileList.append(Tile())
        for infile in listing:
            if "defaults" not in infile:
                newtile = E_VARS().getPath()+infile
                tileList.append(Tile(newtile))
        return tileList

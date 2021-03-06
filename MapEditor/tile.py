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
            self.__path = E_VARS().PATH + "defaults/empty.png"
            self.__pic = pygame.image.load(self.__path)
            self.__name= "empty"
    
    @property
    def path(self):
        return self.__path

    @property
    def pic(self):
        return self.__pic

    @property
    def name(self):
        return self.__name

    @property
    def width(self):
        return self.__width

    @property
    def height(self):
        return self.__height

    @staticmethod
    def color_surface(surface, r, g, b):
        arr = pygame.surfarray.pixels3d(surface)
        arr[:,:,0] = arr[:,:,0] - r
        arr[:,:,1] = arr[:,:,1] - g
        arr[:,:,2] = arr[:,:,2] - b

    @staticmethod
    def loadTiles():
        tileList=[]
        listing = os.listdir(E_VARS().PATH)
        tileList.append(Tile())
        for infile in listing:
            if "defaults" not in infile:
                newtile = E_VARS().PATH+infile
                tileList.append(Tile(newtile))
        return tileList

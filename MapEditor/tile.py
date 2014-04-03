import pygame, os
from pygame.locals import *

from globals import E_VARS

class Tile:
    
    #private vars
    __path = ''
    __pic = ''
    __name = ''

    def __init__(self, name=None):
        if name:
            self.__path = name
            self.__pic = pygame.image.load(name)
            types = name.split('/')
            types = name.split('_')
            self.__name = types[len(types)-1].split('.')[0]
        else:
            self.__path = E_VARS.PATH + "defaults/empty.png"
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
    def loadTiles():
        tileList=[]
        listing = os.listdir(E_VARS.PATH)
        tileList.append(Tile())
        for infile in listing:
            if "defaults" not in infile:
                newtile = E_VARS.PATH+infile
                tileList.append(Tile(newtile))
        return tileList

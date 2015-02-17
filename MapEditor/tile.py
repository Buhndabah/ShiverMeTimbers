import pygame, os
from pygame.locals import *

from globals import E_VARS

class Tile:
    
    #private vars
    __path = ''
    __pic = ''
    __name = ''
    __vars = 0

    def __init__(self, env, name=None):
        self.__vars=env
        if name:
            self.__path = name
            self.__pic = pygame.image.load(name)
            self.__name = name.split('.')[0]
            temp = self.__name.split('/')
            self.__name = temp[len(temp)-1]
        else:
            self.__path = self.__vars.getPath() + "defaults/empty.png"
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
    def loadTiles(env):
        tileList=[]
        listing = os.listdir(env.getPath())
        tileList.append(Tile(env))
        for infile in listing:
            if "defaults" not in infile:
                newtile = env.getPath()+infile
                tileList.append(Tile(env,newtile))
        return tileList

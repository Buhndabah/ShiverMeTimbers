#Singleton storing information such as window size, fps, etc
class E_VARS(object):

    __instance=None

    def __new__(cls):
        if E_VARS.__instance is None:
            E_VARS.__instance = object.__new__(cls)
        return E_VARS.__instance

    def __init__(self):
        # environment vars
        self.__FPS=60 # frames per second
        self.__W_HEIGHT=900# height of window
        self.__W_WIDTH=1440# width of window
        self.__CELLWIDTH=128
        self.__CELLHEIGHT=64
        self.__CELLRISE=49
        self.__MAPWIDTH=15
        self.__MAPHEIGHT=15

        #assert W_WIDTH%CELLWIDTH==0, "Window width must be a multiple of cell size."
        #assert W_HEIGHT%CELLHEIGHT==0, "Window height must be a multiple of cell size."
    
        self.__PATH = 'isoTiles/'

    @property
    def FPS(self):
        return self.__FPS
    @property
    def W_WIDTH(self):
        return self.__W_WIDTH
    @property
    def W_HEIGHT(self):
        return self.__W_HEIGHT
    @property
    def CELLWIDTH(self):
        return self.__CELLWIDTH
    @property
    def CELLHEIGHT(self):
        return self.__CELLHEIGHT
    @property
    def CELLRISE(self):
        return self.__CELLRISE
    @property
    def MAPWIDTH(self):
        return self.__MAPWIDTH
    @property
    def MAPHEIGHT(self):
        return self.__MAPHEIGHT
    @property
    def PATH(self):
        return self.__PATH

    def getMapDim(self):
        return (self.__MAPWIDTH,self.__MAPHEIGHT)
    def getCellDim(self):
        return (self.__CELLWIDTH, self.__CELLHEIGHT,self.__CELLRISE)
    def getWinDim(self):
        return (self.__W_WIDTH,self.__W_HEIGHT)
    def setWinDim(self,newDim):
        self.__W_WIDTH=newDim[0]
        self.__W_HEIGHT=newDim[1]
    def setCellDim(self, newDim):
        self.__CELLWIDTH=newDim[0]
        self.__CELLHEIGHT=newDim[1]
        self.__CELLRISE=newDim[2]
    def setMapDim(self,newDim):
        self.__MAPWIDTH=newDim[0]
        self.__MAPHEIGHT=newDim[1]
    
class COLORS:
    # RGB vals
    BLACK = (0,0,0)
    WHITE = (255,255,255)
    GREEN = (0,255,0)
    GRAY = (105,105,105)
    GRAYER = (30,30,30)
    PURPLE= (148,0,211)
    RED = (255,0,0)
    BGCOLOR = GRAY
    
class STRINGS:
    # string constants
    UP = 'up'
    DOWN = 'down'
    LEFT = 'left'
    RIGHT = 'right'
    X = 'x'
    Y = 'y'

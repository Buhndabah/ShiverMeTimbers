class E_VARS:

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

    def getFPS(self):
        return self.__FPS
    def getWinDim(self):
        return (self.__W_WIDTH,self.__W_HEIGHT)
    def getWinWidth(self):
        return self.__W_WIDTH
    def getWinHeight(self):
        return self.__W_HEIGHT
    def getCellDim(self):
        return (self.__CELLWIDTH, self.__CELLHEIGHT,self.__CELLRISE)
    def getCellWidth(self):
        return self.__CELLWIDTH
    def getCellHeight(self):
        return self.__CELLHEIGHT
    def getCellRise(self):
        return self.__CELLRISE
    def getMapDim(self):
        return (self.__MAPWIDTH,self.__MAPHEIGHT)
    def getMapWidth(self):
        return self.__MAPWIDTH
    def getMapHeight(self):
        return self.__MAPHEIGHT
    def getPath(self):
        return self.__PATH

    def setFPS(self, fps):
        self.__FPS=fps
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

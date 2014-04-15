class E_VARS:
    # environment vars
    FPS=60 # frames per second
    W_HEIGHT=900# height of window
    W_WIDTH=1440# width of window
    CELLWIDTH=128
    CELLHEIGHT=64
    CELLRISE=49
    MAPWIDTH=12
    MAPHEIGHT=12


    #assert W_WIDTH%CELLWIDTH==0, "Window width must be a multiple of cell size."
    #assert W_HEIGHT%CELLHEIGHT==0, "Window height must be a multiple of cell size."
    
    #Offset needed to tile current tileset
    PATH = 'isoTiles/'
    
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

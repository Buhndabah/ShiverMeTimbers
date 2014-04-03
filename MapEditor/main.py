import pygame, sys, os, math
from Tkinter import *
from pygame.locals import *
from globals import E_VARS
from globals import STRINGS
from globals import COLORS
from player import Player
from tile import Tile

MAP =[]
TODRAW = 1
for i in range(0,E_VARS.W_HEIGHT,E_VARS.CELLHEIGHT):
    MAP.append([])
    for j in range(0,E_VARS.W_WIDTH,E_VARS.CELLWIDTH):
        MAP[i/E_VARS.CELLHEIGHT].append(0)


def main():
    global FPSCLOCK,DISPLAYSURF,BASICFONT,GRID

    GRID=True
    pygame.init()
   
    tiles = Tile.loadTiles()
    pygame.key.set_repeat(1,100)
    DISPLAYSURF = pygame.display.set_mode((E_VARS.W_WIDTH,E_VARS.W_HEIGHT))
    FPSCLOCK = pygame.time.Clock()
    BASICFONT = pygame.font.Font("freesansbold.ttf",18)
    pygame.display.set_caption('Editor')

    root = Tk()

    runGame(tiles)
    root.destroy()

def runGame(tiles):
    global TODRAW 
    # set player start
    #p = Player(0,-E_VARS.CELLHEIGHT/2)
    p = Player(0,0)
    # main game loop
    while True:
        handleEvents(p,tiles)
       
        # check to see if crossing over an edge (appear at opposite edge), and move
        #edgeCheck(p.getCoords())

        DISPLAYSURF.fill(COLORS.BGCOLOR)
        
        # XXX TODO drawGrid() is broken - doesn't match bases of tiles
        fillTiles(tiles)
        p.draw(DISPLAYSURF)
        #drawPlayer(p.getCoords())
        DISPLAYSURF.blit(tiles[TODRAW].getPic(),(p.getCoords()[STRINGS.X],p.getCoords()[STRINGS.Y]))
        if GRID:
            drawGrid()
        pygame.display.update()
        FPSCLOCK.tick(E_VARS.FPS)

def handleEvents(player,tiles):
    global TODRAW
    for event in pygame.event.get():
        if event.type == QUIT:  #exit program
            terminate()
        elif event.type == KEYDOWN:
            if event.key == K_ESCAPE:   # exit program
                terminate()
            elif event.key == K_RETURN: # edit map to new item (currently defaults to starpad
                insertItem(player)
            elif event.key == K_a:
                if TODRAW > 0:
                    TODRAW = TODRAW - 1
            elif event.key == K_d:
                if TODRAW <  len(tiles)-1:
                    TODRAW = TODRAW + 1
            elif event.key == K_BACKSPACE:
                removeItem(player)
            elif event.key == K_m:
                printMap(tiles)
            else:                       # tell player to move in dir
                player.setDir(event.key)

def printMap(tiles):
   file = open('test.xml','w')
   file.write("<?xml version=\"1.0\"?>\n")
   file.write("<map width=\"")
   file.write(str(len(MAP[0])))
   file.write("\" height=\"")
   file.write(str(len(MAP)))
   file.write("\" tilewidth=\"")
   file.write(str(E_VARS.CELLWIDTH))
   file.write("\" tileheight=\"")
   file.write(str(E_VARS.CELLHEIGHT))
   file.write("\" tileRise=\"")
   file.write("")
   file.write("\" weather=\"snow>\"\n")

   i=0
   file.write("\t<tileset>\n")
   for tile in tiles:
       file.write("\t\t<tile id=\"")
       file.write(str(i))
       file.write("\" name=\"")
       file.write(tile.getName())
       file.write("\"/>\n")
       i=i+1
   file.write("\t</tileset>\n")

   file.write("\t<layer name=\"test\">\n")
   for tileList in MAP:
       for tile in tileList:
        file.write("\t\t<element id=\"")
        file.write(str(tile))
        file.write("\" collision=\"false\"/>\n")
   file.write("\t</layer>\n")
   file.write("</map>")
   file.close()

def insertItem(player):
    MAP[player.getCoords()[STRINGS.X]/E_VARS.CELLWIDTH][player.getCoords()[STRINGS.Y]/E_VARS.CELLHEIGHT] = TODRAW

def removeItem(player):
    MAP[player.getCoords()[STRINGS.X]/E_VARS.CELLWIDTH][player.getCoords()[STRINGS.Y]/E_VARS.CELLHEIGHT] = 0

# Disabled player wrapping code
'''
def edgeCheck(playerCoords):
    if playerCoords[STRINGS.X] <= -1:
        playerCoords[STRINGS.X] = E_VARS.W_WIDTH-E_VARS.CELLWIDTH
    elif playerCoords[STRINGS.X] >= E_VARS.W_WIDTH:
        playerCoords[STRINGS.X] = 0 
    elif playerCoords[STRINGS.Y] <= -E_VARS.CELLHEIGHT/2:
        playerCoords[STRINGS.Y] = E_VARS.W_HEIGHT-E_VARS.CELLHEIGHT
    elif playerCoords[STRINGS.Y] > E_VARS.W_HEIGHT-E_VARS.CELLHEIGHT:
        playerCoords[STRINGS.Y] = 0
'''

def fillTiles(tiles):
    for y in range(0,len(MAP[0])):
        for x in range(0,len(MAP)):
            if(MAP[x][y] >= len(tiles)):    # if out of range, draw nothing
                pass
            else:                            # else get pic and draw
                item = tiles[MAP[x][y]].getPic()
                #XXX TODO why 10? I don't like the magic number here
                item.convert_alpha()
                item.set_alpha(128)
                DISPLAYSURF.blit(item,(x*item.get_width(), y*E_VARS.CELLHEIGHT-10))
            
def drawGrid():
    #draw vertical lines
    for x in range(-E_VARS.W_WIDTH,E_VARS.W_WIDTH*2,E_VARS.CELLWIDTH):
        x2 = x + math.cos(math.radians(26.565)) * math.sqrt(E_VARS.W_HEIGHT**2 + E_VARS.W_WIDTH**2)
        y2 = 0 + math.sin(math.radians(26.565)) * math.sqrt(E_VARS.W_HEIGHT**2 + E_VARS.W_WIDTH**2)
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK, (x+E_VARS.CELLWIDTH/2,0), (0,(E_VARS.CELLHEIGHT/2) + (x/E_VARS.CELLWIDTH*E_VARS.CELLHEIGHT)),1)
        pygame.draw.line(DISPLAYSURF,COLORS.BLACK, (x-E_VARS.CELLWIDTH/2,0), (x2-E_VARS.CELLWIDTH/2,y2),1)
''' for x1 in range(0, E_VARS.W_WIDTH, E_VARS.CELLWIDTH):
        xa = x1 + math.cos(math.radians(26.565)) * math.sqrt(E_VARS.W_HEIGHT**2 + E_VARS.W_WIDTH**2)
        ya = 0 + math.sin(math.radians(26.565)) * math.sqrt(E_VARS.W_HEIGHT**2 + E_VARS.W_WIDTH**2)
        pygame.draw.line(DISPLAYSURF,COLORS.WHITE, (x1,0), (xa,ya),1)
    for y1 in range(E_VARS.CELLHEIGHT, E_VARS.W_HEIGHT, E_VARS.CELLHEIGHT):
        xc = 0 +  math.cos(math.radians(26.565)) * math.sqrt(E_VARS.W_HEIGHT**2 + E_VARS.W_WIDTH**2)
        yc = y1 + math.sin(math.radians(26.565)) * math.sqrt(E_VARS.W_HEIGHT**2 + E_VARS.W_WIDTH**2)
        pygame.draw.line(DISPLAYSURF,COLORS.WHITE, (0,y1), (xc,yc),1)
        pygame.draw.line(DISPLAYSURF,COLORS.WHITE, (0, y1), '''


'''    for x in range(0, E_VARS.W_WIDTH,E_VARS.CELLWIDTH): 
        pygame.draw.line(DISPLAYSURF,COLORS.WHITE, (x,0),(x,E_VARS.W_HEIGHT))
    #draw horizontal lines
    for y in range(0, E_VARS.W_HEIGHT,E_VARS.CELLHEIGHT): 
        pygame.draw.line(DISPLAYSURF,COLORS.WHITE,(0,y),(E_VARS.W_WIDTH,y))  '''
            
def terminate():
    pygame.quit()
    sys.exit()

if __name__ == '__main__':
    main()

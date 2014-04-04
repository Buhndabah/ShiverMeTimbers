import pygame, sys, os, math, copy
from Tkinter import *
from pygame.locals import *
from globals import E_VARS
from globals import STRINGS
from globals import COLORS
from player import Player
from tile import Tile

MAP =[]
TODRAW = 1
COLLIDABLE=True
NONCOLLIDABLE=False
for i in range(0,E_VARS.MAPHEIGHT):
    MAP.append([])
    for j in range(0,E_VARS.MAPWIDTH):
        MAP[i].append({"id": 0, "coord": {STRINGS.X:0, STRINGS.Y:0}, "collidable":NONCOLLIDABLE})


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
    p = Player()
    # main game loop
    while True:
        handleEvents(p,tiles)
       
        # check to see if crossing over an edge (appear at opposite edge), and move
        #edgeCheck(p.getCoords())

        DISPLAYSURF.fill(COLORS.BGCOLOR)
        
        # XXX TODO drawGrid() is broken - doesn't match bases of tiles
        fillTiles(tiles)
        p.draw(DISPLAYSURF)

        # draw currently selected tile at player's loc
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
        elif event.type == KEYUP:
            pygame.key.set_repeat(1,100)
        elif event.type == KEYDOWN:
            if event.key == K_ESCAPE:   # exit program
                terminate()
            elif event.key == K_RETURN: # edit map to new item (currently defaults to starpad
                insertItem(player)
            elif event.key == K_z:
                if TODRAW > 0:
                    TODRAW = TODRAW - 1
            elif event.key == K_x:
                if TODRAW <  len(tiles)-1:
                    TODRAW = TODRAW + 1
            elif event.key == K_BACKSPACE:
                removeItem(player)
            elif event.key == K_m:
                printMap(tiles)
            elif event.key == K_c:
                pygame.key.set_repeat()
                setCollide(player)
            else:                       # tell player to move in dir
                player.setDir(event.key)

def printMap(tiles):

   print("\nPrinting map to file\n")

   file = open('maps/test.xml','w')
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
   file.write("\" weather=\"snow\">\n")

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
   i=0
   for tileList in MAP:
       for tile in tileList:
           file.write("\t\t<element id=\"")
           if(tile is 0):
               file.write(str(0))
               file.write("\" collision=\"false\"/>\n")
           else:
               file.write(str(tile["id"]))
               file.write("\" collision=\"")
               file.write(str(tile["collidable"]).lower())
               file.write("\"/>\n")
           if((i+1)%E_VARS.MAPWIDTH is 0):
               file.write("\n")
           i=i+1
   file.write("\t</layer>\n")
   file.write("</map>")
   file.close()

   print "Finished writing map\n"

def insertItem(player):
    pos = copy.copy(player.getMapPos())
    drawPos = copy.copy(player.getCoords())

    # check boundaries
    if not (pos[0] > E_VARS.MAPWIDTH-1) and not (pos[0] < 0) and not (pos[0] % 1.0 > 0) and not (pos[1] > E_VARS.MAPHEIGHT-1) and not (pos[1] < 0) and not (pos[0] % 1.0 > 0):
        MAP[int(pos[1])][int(pos[0])]["id"] = TODRAW
        MAP[int(pos[1])][int(pos[0])]["coord"] = drawPos

def removeItem(player):
    pos = copy.copy(player.getMapPos())
    drawPos = copy.copy(player.getCoords())
    # check boundaries
    if not (pos[0] > E_VARS.MAPWIDTH-1) and not (pos[0] < 0) and not (pos[0] % 1.0 > 0) and not (pos[1] > E_VARS.MAPHEIGHT-1) and not (pos[1] < 0) and not (pos[0] % 1.0 > 0):
        MAP[int(pos[1])][int(pos[0])]["id"] = TODRAW
        MAP[int(pos[1])][int(pos[0])]["coord"] = drawPos
        MAP[int(pos[1])][int(pos[0])]["collidale"] = NONCOLLIDABLE

def setCollide(player):
    pos = copy.copy(player.getMapPos())
    drawPos = copy.copy(player.getCoords())
    # check boundaries
    if not (pos[0] > E_VARS.MAPWIDTH-1) and not (pos[0] < 0) and not (pos[0] % 1.0 > 0) and not (pos[1] > E_VARS.MAPHEIGHT-1) and not (pos[1] < 0) and not (pos[0] % 1.0 > 0):
        MAP[int(pos[1])][int(pos[0])]["coord"] = drawPos
        MAP[int(pos[1])][int(pos[0])]["collidable"] = not MAP[int(pos[1])][int(pos[0])]["collidable"]
        print "set collidable to ", MAP[int(pos[1])][int(pos[0])]["collidable"]
    

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

    # finding the index for draw order
    num=0
    mapX=0
    mapY=0

    # same as draw order code in mapManager.cpp
    for i in range(E_VARS.MAPWIDTH+E_VARS.MAPHEIGHT-1):
        if(i < E_VARS.MAPWIDTH):
            num=i+1
        elif i is E_VARS.MAPWIDTH:
            num = E_VARS.MAPWIDTH-1
        elif i > E_VARS.MAPWIDTH:
            num = E_VARS.MAPWIDTH - i%E_VARS.MAPWIDTH-1
        for j in range(num):
            if i < E_VARS.MAPWIDTH:
                mapY = (i + (j*(E_VARS.MAPWIDTH-1))) % E_VARS.MAPWIDTH;
                mapX = (i + (j*(E_VARS.MAPWIDTH-1))) / E_VARS.MAPWIDTH;
            else:
                mapY = (i + ((i+1)%E_VARS.MAPWIDTH)*(E_VARS.MAPWIDTH-1) + j*(E_VARS.MAPWIDTH-1)) % E_VARS.MAPWIDTH;
                mapX =(i + ((i+1)%E_VARS.MAPWIDTH)*(E_VARS.MAPWIDTH-1) + j*(E_VARS.MAPWIDTH-1)) / E_VARS.MAPWIDTH;
            #print mapX, mapY
            if MAP[mapY][mapX] is not 0:
                item = tiles[MAP[mapY][mapX]["id"]].getPic()
                item.convert_alpha()
                item.set_alpha(128)
                DISPLAYSURF.blit(item, (MAP[mapY][mapX]["coord"][STRINGS.X], MAP[mapY][mapX]["coord"][STRINGS.Y]))

def drawGrid():

    for x in range(0,E_VARS.MAPWIDTH+1):

        # down and left
        pygame.draw.line(DISPLAYSURF, COLORS.BLACK, (E_VARS.W_WIDTH/2+x*E_VARS.CELLWIDTH/2, E_VARS.W_HEIGHT/2-E_VARS.CELLHEIGHT*E_VARS.MAPHEIGHT/2 + x*E_VARS.CELLHEIGHT/2), (E_VARS.W_WIDTH/2 - E_VARS.MAPWIDTH*E_VARS.CELLWIDTH/2+ E_VARS.CELLWIDTH/2*x, E_VARS.W_HEIGHT/2 + x*E_VARS.CELLHEIGHT/2),1)
        
        #down and right
        pygame.draw.line(DISPLAYSURF, COLORS.BLACK, (E_VARS.W_WIDTH/2 - x*E_VARS.CELLWIDTH/2, E_VARS.W_HEIGHT/2-E_VARS.CELLHEIGHT*E_VARS.MAPHEIGHT/2 + x*E_VARS.CELLHEIGHT/2), (E_VARS.W_WIDTH/2 + E_VARS.MAPWIDTH*E_VARS.CELLWIDTH/2 - E_VARS.CELLWIDTH/2*x, E_VARS.W_HEIGHT/2 + x*E_VARS.CELLHEIGHT/2),1)
            
def terminate():
    pygame.quit()
    sys.exit()

if __name__ == '__main__':
    main()

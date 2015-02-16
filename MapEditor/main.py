import pygame, sys, os, math, Tkinter
import Tkinter as tk
import tkMessageBox 
from pygame.locals import *
from globals import E_VARS
from globals import STRINGS
from globals import COLORS
from player import Player
from tile import Tile
from popupWindow import popupWindow

# index of tile we're currently drawing
TODRAW = 1
COLLIDABLE=True
NONCOLLIDABLE=False

#array storing tile info
MAP =[]

#window for handling pop up dialogs
WINDOW = Tkinter.Tk()
WINDOW.geometry('+'+str(E_VARS.W_WIDTH/2)+'+'+str(E_VARS.W_HEIGHT/2))
WINDOW.wm_withdraw()

# keep running?
QUIT_GAME = False


def main():
    global FPSCLOCK,DISPLAYSURF,BASICFONT,GRID
    GRID=True

    pygame.init()
    pygame.key.set_repeat(1,100)
    pygame.display.set_caption('Editor')

    DISPLAYSURF = pygame.display.set_mode((E_VARS.W_WIDTH,E_VARS.W_HEIGHT))
    FPSCLOCK = pygame.time.Clock()
    BASICFONT = pygame.font.Font("freesansbold.ttf",18)

    MAP.append(createLayer())
    tiles = Tile.loadTiles()

    runGame(tiles)


#----------------------------------


# Main game loop
def runGame(tiles):

    global TODRAW 
    p = Player()

    while True:
    
        # time to shut down
        if QUIT_GAME:
            terminate()

        handleEvents(p,tiles)
      
        #draw background, placed tiles, and static grid 
        DISPLAYSURF.fill(COLORS.BGCOLOR)
        fillTiles(tiles,p)
        drawLowerGrid()

        # draw currently selected tile type at cursor's loc
        pos = (p.getCoords()[STRINGS.X], p.getCoords()[STRINGS.Y]-p.getLevel()*E_VARS.CELLRISE)
        image = tiles[TODRAW].getPic()
        DISPLAYSURF.blit(image,pos)

        #make tile grid
        drawUpperGrid(p)

        pygame.display.update()
        FPSCLOCK.tick(E_VARS.FPS)


#-------------------------------------------


# Handle keyboard input
def handleEvents(player,tiles):
    global TODRAW
    global QUIT_GAME
    global WINDOW

    for event in pygame.event.get():

        #exit editor
        if event.type == QUIT: 
            QUIT_GAME = True
        elif event.type == KEYUP:
            pygame.key.set_repeat(1,100)

        #key press received
        elif event.type == KEYDOWN:

            #exit program
            if event.key == K_ESCAPE:
                if tkMessageBox.askyesno(title="Save?", message="Do you want to save your changes before exiting?"):
                    savePrompt(tiles)
                QUIT_GAME = True

            # Map Editing functions

            # place the selected tile type at the cursor
            elif event.key == K_RETURN:
                insertItem(player)

            # if a tile exists at the cursor, delete it
            elif event.key == K_BACKSPACE:
                removeItem(player)

            # turn on the collision map
            elif event.key == K_c:
                pygame.key.set_repeat()
                setCollide(player)

            # scroll back one tile type
            elif event.key == K_z:
                if TODRAW > 0:
                    TODRAW = TODRAW - 1

            # scroll forward one tile type
            elif event.key == K_x:
                if TODRAW <  len(tiles)-1:
                    TODRAW = TODRAW + 1

            # save the map to file
            elif event.key == K_m:
                savePrompt(tiles)

            # move up one elevation
            elif event.key == K_t:
                player.goUp()

            # move down one elevation
            elif event.key == K_y:
                if not (player.getLevel() == 0):
                    player.goDown()
            
            # move the cursor in direction
            else:                       
                player.setDir(event.key)


#-------------------------------------------

def savePrompt(tiles):

    # open a new window
    new_win = Tkinter.Tk()
    new_win.geometry('+'+str(E_VARS.W_WIDTH/2)+'+'+str(E_VARS.W_HEIGHT/2))

    # create an entry field in it
    nameChoice = tk.Entry(new_win)
    nameChoice.pack()
    nameChoice.insert(0, "myMap")

    # add a confirmation button
    nameButton = Tkinter.Button(new_win, text='Ok', command= lambda :  printMap(tiles, nameChoice.get(), new_win))
    nameButton.pack()
    #nameChoice.bind("<Return>", lambda event : printMap(tiles, nameChoice.get()))

    # wait until the new window closes
    new_win.wait_window(nameChoice)

# Write out to xml
# XXX TODO currently this always print to a file named "test.xml"
def printMap(tiles, fileName, window):

   print("\nPrinting map to file\n")

   file = open('maps/'+fileName+'.xml','w')
   file.write("<?xml version=\"1.0\"?>\n")
   file.write("<map width=\"")
   file.write(str(len(MAP[0][0])))
   file.write("\" height=\"")
   file.write(str(len(MAP[0])))
   file.write("\" tilewidth=\"")
   file.write(str(E_VARS.CELLWIDTH))
   file.write("\" tileheight=\"")
   file.write(str(E_VARS.CELLHEIGHT))
   file.write("\" tileRise=\"")
   file.write(str(E_VARS.CELLRISE))
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
    
   j=0
   for layer in MAP:
       file.write("\t<layer name=\"")
       file.write(str(j))
       file.write("\">\n")
       for tileList in layer:
            i=0
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
       j=j+1
   file.write("</map>")
   file.close()

   print "Finished writing map\n"

   window.destroy()

#-------------------------------------------------------------------


# Add a new layer full of empty tiles to the map
def createLayer():
    newLayer = []
    for i in range(0,E_VARS.MAPHEIGHT):
        newLayer.append([])
        for j in range(0,E_VARS.MAPWIDTH):
            newLayer[i].append({"id": 0, "coord": {STRINGS.X:0, STRINGS.Y:0}, "collidable":NONCOLLIDABLE})
    return newLayer


#-------------------------------------------------------------


# Place tile at cursor position
def insertItem(player):

    pos = player.getMapPos()
    drawPos = player.getCoords()

    nMaxX = not (pos[0] > E_VARS.MAPWIDTH-1)
    nMinX = not (pos[0] < 0)
    nHalfX = not (pos[0] % 1.0 > 0)
    nMaxY = not (pos[1] > E_VARS.MAPHEIGHT-1)
    nMinY = not (pos[1] < 0)
    nHalfY = not (pos[1] % 1.0 > 0)

    # check boundaries
    if nMaxX and nMinX and nHalfX and nMaxY and nMinY and nHalfY:
        if player.getLevel() > len(MAP)-1:
            for i in range(player.getLevel() - len(MAP)+1):
                MAP.append(createLayer())
        MAP[player.getLevel()][int(pos[1])][int(pos[0])]["id"] = TODRAW
        MAP[player.getLevel()][int(pos[1])][int(pos[0])]["coord"] = drawPos



# Remove tile at cursor position
def removeItem(player):

    pos =player.getMapPos()
    drawPos = player.getCoords()

    nMaxX = not (pos[0] > E_VARS.MAPWIDTH-1)
    nMinX = not (pos[0] < 0)
    nHalfX = not (pos[0] % 1.0 > 0)
    nMaxY = not (pos[1] > E_VARS.MAPHEIGHT-1)
    nMinY = not (pos[1] < 0)
    nHalfY = not (pos[1] % 1.0 > 0)
    nTooHigh = not (player.getLevel() > len(MAP) -1)

    # check boundaries
    if nMaxX and nMinX and nHalfX and nMaxY and nMinY and nHalfY and nTooHigh:
        MAP[player.getLevel()][int(pos[1])][int(pos[0])]["id"] = 0
        MAP[player.getLevel()][int(pos[1])][int(pos[0])]["coord"] = drawPos
        MAP[player.getLevel()][int(pos[1])][int(pos[0])]["collidale"] = NONCOLLIDABLE



# Toggle collidable property at cursor position
def setCollide(player):

    pos =player.getMapPos()
    drawPos = player.getCoords()

    nMaxX = not (pos[0] > E_VARS.MAPWIDTH-1)
    nMinX = not (pos[0] < 0)
    nHalfX = not (pos[0] % 1.0 > 0)
    nMaxY = not (pos[1] > E_VARS.MAPHEIGHT-1)
    nMinY = not (pos[1] < 0)
    nHalfY = not (pos[1] % 1.0 > 0)
    nTooHigh = not (player.getLevel() > len(MAP) -1)

    # check boundaries
    if nMaxX and nMinX and nHalfX and nMaxY and nMinY and nHalfY and nTooHigh:
        MAP[player.getLevel()][int(pos[1])][int(pos[0])]["coord"] = drawPos
        MAP[player.getLevel()][int(pos[1])][int(pos[0])]["collidable"] = not MAP[player.getLevel()][int(pos[1])][int(pos[0])]["collidable"]
        print "set collidable to ", MAP[player.getLevel()][int(pos[1])][int(pos[0])]["collidable"]
    

#--------------------------------------------------------------------------------


# Draw the map
def fillTiles(tiles,player):

    # finding the index for draw order
    num=0
    mapX=0
    mapY=0

    # For every row
    for i in range(E_VARS.MAPWIDTH+E_VARS.MAPHEIGHT-1):

        # and every elevation
        for k in range(len(MAP)):

            if(i < E_VARS.MAPWIDTH):    # top half of diamond
                num=i+1

            elif i is E_VARS.MAPWIDTH:  # middle bit
                num = E_VARS.MAPWIDTH-1

            elif i > E_VARS.MAPWIDTH:   # bottom half
                num = E_VARS.MAPWIDTH - i%E_VARS.MAPWIDTH-1
    
            # draw the appropriate number of tiles
            for j in range(num):

                # calculate the coordinates
                if i < E_VARS.MAPWIDTH:
                    mapY = (i + (j*(E_VARS.MAPWIDTH-1))) % E_VARS.MAPWIDTH;
                    mapX = (i + (j*(E_VARS.MAPWIDTH-1))) / E_VARS.MAPWIDTH;
                else:
                    mapY = (i + ((i+1)%E_VARS.MAPWIDTH)*(E_VARS.MAPWIDTH-1) + j*(E_VARS.MAPWIDTH-1)) % E_VARS.MAPWIDTH;
                    mapX =(i + ((i+1)%E_VARS.MAPWIDTH)*(E_VARS.MAPWIDTH-1) + j*(E_VARS.MAPWIDTH-1)) / E_VARS.MAPWIDTH;

                # if there's a tile here (nonblank)
                if MAP[k][mapY][mapX] is not 0:
                    item = tiles[MAP[k][mapY][mapX]["id"]].getPic()
                    item.convert_alpha()
                    colored = item.copy()
                    if MAP[k][mapY][mapX]["collidable"] is True:
                        Tile.color_surface(colored,0,155,155)
                        DISPLAYSURF.blit(colored, (MAP[k][mapY][mapX]["coord"][STRINGS.X], MAP[k][mapY][mapX]["coord"][STRINGS.Y]-k*E_VARS.CELLRISE))
                    else:
                        Tile.color_surface(colored,155,155,0)
                        DISPLAYSURF.blit(colored, (MAP[k][mapY][mapX]["coord"][STRINGS.X], MAP[k][mapY][mapX]["coord"][STRINGS.Y]-k*E_VARS.CELLRISE))


#-------------------------------------------------------------------------------------------------------


# base grid that doesn't move
def drawLowerGrid():
           
    for x in range(0,E_VARS.MAPWIDTH+1):

        # down and left
        startX = E_VARS.W_WIDTH/2+x*E_VARS.CELLWIDTH/2 
        startY = E_VARS.W_HEIGHT/2-E_VARS.CELLHEIGHT*E_VARS.MAPHEIGHT/2+x*E_VARS.CELLHEIGHT/2
        endX = E_VARS.W_WIDTH/2-E_VARS.MAPWIDTH*E_VARS.CELLWIDTH/2+E_VARS.CELLWIDTH/2*x
        endY = E_VARS.W_HEIGHT/2+x*E_VARS.CELLHEIGHT/2

        pygame.draw.line(DISPLAYSURF, 
                         COLORS.BLACK,
                        (startX, startY),
                        (endX, endY),
                         1)
        
        #down and right
        startX = E_VARS.W_WIDTH/2-x*E_VARS.CELLWIDTH/2
        startY = E_VARS.W_HEIGHT/2-E_VARS.CELLHEIGHT*E_VARS.MAPHEIGHT/2+x*E_VARS.CELLHEIGHT/2
        endX = E_VARS.W_WIDTH/2+E_VARS.MAPWIDTH*E_VARS.CELLWIDTH/2-E_VARS.CELLWIDTH/2*x
        endY = E_VARS.W_HEIGHT/2+x*E_VARS.CELLHEIGHT/2

        pygame.draw.line(DISPLAYSURF, 
                         COLORS.BLACK, 
                        (startX, startY),
                        (endX, endY),
                         1)
        

# grid tied to player level
def drawUpperGrid(player):

    for x in range(0,E_VARS.MAPWIDTH+1):

        # down and left
        startX = E_VARS.W_WIDTH/2+x*E_VARS.CELLWIDTH/2
        startY = E_VARS.W_HEIGHT/2-E_VARS.CELLHEIGHT*E_VARS.MAPHEIGHT/2+x*E_VARS.CELLHEIGHT/2-player.getLevel()*E_VARS.CELLRISE
        endX = E_VARS.W_WIDTH/2-E_VARS.MAPWIDTH*E_VARS.CELLWIDTH/2+E_VARS.CELLWIDTH/2*x
        endY = E_VARS.W_HEIGHT/2+x*E_VARS.CELLHEIGHT/2-player.getLevel()*E_VARS.CELLRISE

        pygame.draw.line(DISPLAYSURF, 
                         COLORS.WHITE,
                        (startX, startY), 
                        (endX, endY),
                         1)
        
        #down and right
        startX = E_VARS.W_WIDTH/2-x*E_VARS.CELLWIDTH/2
        startY = E_VARS.W_HEIGHT/2-E_VARS.CELLHEIGHT*E_VARS.MAPHEIGHT/2+x*E_VARS.CELLHEIGHT/2-player.getLevel()*E_VARS.CELLRISE
        endX = E_VARS.W_WIDTH/2+E_VARS.MAPWIDTH*E_VARS.CELLWIDTH/2-E_VARS.CELLWIDTH/2*x
        endY = E_VARS.W_HEIGHT/2+x*E_VARS.CELLHEIGHT/2-player.getLevel()*E_VARS.CELLRISE

        pygame.draw.line(DISPLAYSURF, 
                         COLORS.WHITE, 
                        (startX, startY),
                        (endX, endY),
                        1)


#---------------------------------------------------------------------


def terminate():
    pygame.quit()
    sys.exit()

if __name__ == '__main__':
    main()

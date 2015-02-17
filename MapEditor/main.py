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

OFFSET=[0,0]

#array storing tile info
MAP =[]

VARS=E_VARS()

#window for handling pop up dialogs
WINDOW = Tkinter.Tk()
WINDOW.geometry('+'+str(VARS.getWinWidth()/2)+'+'+str(VARS.getWinHeight()/2))
WINDOW.wm_withdraw()

# keep running?
QUIT_GAME = False



def main():
    global FPSCLOCK,DISPLAYSURF,BASICFONT,GRID,VARS
    GRID=True

    pygame.init()
    pygame.key.set_repeat(2,1000)
    pygame.display.set_caption('Editor')

    DISPLAYSURF = pygame.display.set_mode((VARS.getWinWidth(),VARS.getWinHeight()))
    FPSCLOCK = pygame.time.Clock()
    BASICFONT = pygame.font.Font("freesansbold.ttf",18)

    # prompt for map size
    if not tkMessageBox.askyesno(title="Map Size", message="Use default map size?"):
        new_win = Tkinter.Tk()
        new_win.geometry('+'+str(VARS.getWinWidth()/2)+'+'+str(VARS.getWinHeight()/2))

        sizeChoiceX = tk.Entry(new_win)
        sizeChoiceX.pack()
        sizeChoiceX.insert(0,"0")
        
        sizeChoiceY = tk.Entry(new_win)
        sizeChoiceY.pack()
        sizeChoiceY.insert(0,"0")

        # add a confirmation button
        sizeButton = Tkinter.Button(new_win, text='Ok', command = lambda: setWinSize((int(sizeChoiceX.get()), int(sizeChoiceY.get())),new_win))
        sizeButton.pack()
        
        # wait until the prompt closes
        new_win.wait_window(sizeChoiceX)
        

    MAP.append(createLayer())
    tiles = Tile.loadTiles(VARS)

    runGame(tiles)


#----------------------------------

def setWinSize(size, window):
    global VARS
    VARS.setMapDim(size)
    window.destroy()


# Main game loop
def runGame(tiles):

    global TODRAW,VARS 
    p = Player(VARS)

    #set cursor initial position to middle
    #on initialization the cursor's at map 0,0, the top corne
    for i in range(0, VARS.getMapWidth()-1):
        p.setDir(K_DOWN)

    # figure out where we're anchoring the cursor
    center = (p.getCoords()[STRINGS.X], p.getCoords()[STRINGS.Y])

    

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
        #pos = (p.getCoords()[STRINGS.X], p.getCoords()[STRINGS.Y]-p.getLevel()*VARS.getCellRise())
        image = tiles[TODRAW].getPic()
        drawWithOffset(image,center,(0,0))

        #make tile grid
        drawUpperGrid(p)

        pygame.display.update()
        FPSCLOCK.tick(VARS.getFPS())


#-------------------------------------------

def drawWithOffset(image,pos,offset) :
    DISPLAYSURF.blit(image,(pos[0]-offset[0], pos[1]-offset[1]))


# Handle keyboard input
def handleEvents(player,tiles):
    global TODRAW
    global QUIT_GAME
    global WINDOW

    for event in pygame.event.get():

        #exit editor
        if event.type == QUIT: 
            QUIT_GAME = True

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
                if event.key == K_LEFT or event.key==K_a:
                    OFFSET[0] = OFFSET[0]-VARS.getCellWidth()/2
                elif event.key == K_RIGHT or event.key == K_d:
                    OFFSET[0] = OFFSET[0] + VARS.getCellWidth()/2
                elif event.key == K_UP or event.key == K_w:
                    OFFSET[1] = OFFSET[1] - VARS.getCellHeight()/2
                elif event.key == K_DOWN or event.key == K_s:
                    OFFSET[1] = OFFSET[1] + VARS.getCellHeight()/2
                player.setDir(event.key)


#-------------------------------------------

def savePrompt(tiles):

    global VARS

    # open a new window
    new_win = Tkinter.Tk()
    new_win.geometry('+'+str(VARS.getWinWidth()/2)+'+'+str(VARS.getWinHeight()/2))

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

   global VARS

   print("\nPrinting map to file\n")

   file = open('maps/'+fileName+'.xml','w')
   file.write("<?xml version=\"1.0\"?>\n")
   file.write("<map width=\"")
   file.write(str(len(MAP[0][0])))
   file.write("\" height=\"")
   file.write(str(len(MAP[0])))
   file.write("\" tilewidth=\"")
   file.write(str(VARS.getCellWidth()))
   file.write("\" tileheight=\"")
   file.write(str(VARS.getCellHeight()))
   file.write("\" tileRise=\"")
   file.write(str(VARS.getCellRise()))
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
               if((i+1)%VARS.getMapWidth() is 0):
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
    global VARS
    newLayer = []
    for i in range(0,VARS.getMapHeight()):
        newLayer.append([])
        for j in range(0,VARS.getMapWidth()):
            newLayer[i].append({"id": 0, "coord": {STRINGS.X:0, STRINGS.Y:0}, "collidable":NONCOLLIDABLE})
    return newLayer


#-------------------------------------------------------------


# Place tile at cursor position
def insertItem(player):

    global VARS

    pos = player.getMapPos()
    drawPos = player.getCoords()

    nMaxX = not (pos[0] > VARS.getMapWidth()-1)
    nMinX = not (pos[0] < 0)
    nHalfX = not (pos[0] % 1.0 > 0)
    nMaxY = not (pos[1] > VARS.getMapHeight()-1)
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

    global VARS

    pos =player.getMapPos()
    drawPos = player.getCoords()

    nMaxX = not (pos[0] > VARS.getMapWidth()-1)
    nMinX = not (pos[0] < 0)
    nHalfX = not (pos[0] % 1.0 > 0)
    nMaxY = not (pos[1] > VARS.getMapHeight()-1)
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

    nMaxX = not (pos[0] > VARS.getMapWidth()-1)
    nMinX = not (pos[0] < 0)
    nHalfX = not (pos[0] % 1.0 > 0)
    nMaxY = not (pos[1] > VARS.getMapHeight()-1)
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

    global VARS

    # finding the index for draw order
    num=0
    mapX=0
    mapY=0

    # For every row
    for i in range(VARS.getMapWidth()+VARS.getMapHeight()-1):

        # and every elevation
        for k in range(len(MAP)):

            if(i < VARS.getMapWidth()):    # top half of diamond
                num=i+1

            elif i is VARS.getMapWidth():  # middle bit
                num = VARS.getMapWidth()-1

            elif i > VARS.getMapWidth():   # bottom half
                num = VARS.getMapWidth() - i%VARS.getMapWidth()-1
    
            # draw the appropriate number of tiles
            for j in range(num):

                # calculate the coordinates
                if i < VARS.getMapWidth():
                    mapY = (i + (j*(VARS.getMapWidth()-1))) % VARS.getMapWidth()
                    mapX = (i + (j*(VARS.getMapWidth()-1))) / VARS.getMapWidth()
                else:
                    mapY = (i + ((i+1)%VARS.getMapWidth())*(VARS.getMapWidth()-1) + j*(VARS.getMapWidth()-1)) % VARS.getMapWidth()
                    mapX =(i + ((i+1)%VARS.getMapWidth())*(VARS.getMapWidth()-1) + j*(VARS.getMapWidth()-1)) / VARS.getMapWidth()

                # if there's a tile here (nonblank)
                if MAP[k][mapY][mapX] is not 0:
                    item = tiles[MAP[k][mapY][mapX]["id"]].getPic()
                    item.convert_alpha()
                    colored = item.copy()
                    if MAP[k][mapY][mapX]["collidable"] is True:
                        Tile.color_surface(colored,0,155,155)
                        drawWithOffset(colored, (MAP[k][mapY][mapX]["coord"][STRINGS.X], MAP[k][mapY][mapX]["coord"][STRINGS.Y]-k*VARS.getCellRise()),(OFFSET[0], OFFSET[1]))
                    else:
                        Tile.color_surface(colored,155,155,0)
                        drawWithOffset(colored, (MAP[k][mapY][mapX]["coord"][STRINGS.X], MAP[k][mapY][mapX]["coord"][STRINGS.Y]-k*VARS.getCellRise()),(OFFSET[0], OFFSET[1]))


#-------------------------------------------------------------------------------------------------------


# base grid that doesn't move
def drawLowerGrid():
           
    global OFFSET,VARS

    for x in range(0,VARS.getMapWidth()+1):

        # down and left
        startX = VARS.getWinWidth()/2+x*VARS.getCellWidth()/2-OFFSET[0]
        startY = VARS.getWinHeight()/2-VARS.getCellHeight()*VARS.getMapHeight()/2+x*VARS.getCellHeight()/2-OFFSET[1]
        endX = VARS.getWinWidth()/2-VARS.getMapWidth()*VARS.getCellWidth()/2+VARS.getCellWidth()/2*x-OFFSET[0]
        endY = VARS.getWinHeight()/2+x*VARS.getCellHeight()/2-OFFSET[1]

        pygame.draw.line(DISPLAYSURF, 
                         COLORS.BLACK,
                        (startX, startY),
                        (endX, endY),
                         1)
        
        #down and right
        startX = VARS.getWinWidth()/2-x*VARS.getCellWidth()/2-OFFSET[0]
        startY = VARS.getWinHeight()/2-VARS.getCellHeight()*VARS.getMapHeight()/2+x*VARS.getCellHeight()/2-OFFSET[1]
        endX = VARS.getWinWidth()/2+VARS.getMapHeight()*VARS.getCellWidth()/2-VARS.getCellWidth()/2*x-OFFSET[0]
        endY = VARS.getWinHeight()/2+x*VARS.getCellHeight()/2-OFFSET[1]

        pygame.draw.line(DISPLAYSURF, 
                         COLORS.BLACK, 
                        (startX, startY),
                        (endX, endY),
                         1)
        

# grid tied to player level
def drawUpperGrid(player):

    global OFFSET,VARS

    for x in range(0,VARS.getMapWidth()+1):

        # down and left
        startX = VARS.getWinWidth()/2+x*VARS.getCellWidth()/2-OFFSET[0]
        startY = VARS.getWinHeight()/2-VARS.getCellHeight()*VARS.getMapHeight()/2+x*VARS.getCellHeight()/2-player.getLevel()*VARS.getCellRise()-OFFSET[1]
        endX = VARS.getWinWidth()/2-VARS.getMapWidth()*VARS.getCellWidth()/2+VARS.getCellWidth()/2*x-OFFSET[0]
        endY = VARS.getWinHeight()/2+x*VARS.getCellHeight()/2-player.getLevel()*VARS.getCellRise()-OFFSET[1]

        pygame.draw.line(DISPLAYSURF, 
                         COLORS.WHITE,
                        (startX, startY), 
                        (endX, endY),
                         1)
        
        #down and right
        startX = VARS.getWinWidth()/2-x*VARS.getCellWidth()/2-OFFSET[0]
        startY = VARS.getWinHeight()/2-VARS.getCellHeight()*VARS.getMapHeight()/2+x*VARS.getCellHeight()/2-player.getLevel()*VARS.getCellRise()-OFFSET[1]
        endX = VARS.getWinWidth()/2+VARS.getMapHeight()*VARS.getCellWidth()/2-VARS.getCellWidth()/2*x-OFFSET[0]
        endY = VARS.getWinHeight()/2+x*VARS.getCellHeight()/2-player.getLevel()*VARS.getCellRise()-OFFSET[1]

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

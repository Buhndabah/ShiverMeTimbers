import pygame, sys, os, math, Tkinter
import Tkinter as tk
import tkMessageBox 
from pygame.locals import *
from globals import E_VARS
from globals import STRINGS
from globals import COLORS
from player import Player
from tile import Tile

OFFSET=[0,0]

#array storing tile info
MAP =[]

#window for handling pop up dialogs

def main():
    global FPSCLOCK,BASICFONT,GRID
    GRID=True

    pygame.init()
    pygame.key.set_repeat(250,250)
    pygame.display.set_caption('Editor')

    dispSurf = pygame.display.set_mode([0,0],pygame.RESIZABLE)
    WINDOW = Tkinter.Tk()
    WINDOW.geometry('+'+str(E_VARS().W_WIDTH/2)+'+'+str(E_VARS().W_HEIGHT/2))
    WINDOW.wm_withdraw()
    FPSCLOCK = pygame.time.Clock()
    BASICFONT = pygame.font.Font("freesansbold.ttf",18)

    # prompt for map size
    if not tkMessageBox.askyesno(title="Map Size", message="Use default map size?"):
        new_win = Tkinter.Tk()
        new_win.geometry('+'+str(E_VARS().W_WIDTH/2)+'+'+str(E_VARS().W_HEIGHT/2))

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
    tiles = Tile.loadTiles()

    runGame(tiles,dispSurf)


#----------------------------------

def setWinSize(size, window):
    E_VARS().mapDim = size
    window.destroy()


# Main game loop
def runGame(tiles,dispSurf):

    # index of tile we're currently drawing
    curTile = 1
    p = Player()

    #set cursor initial position to middle
    #on initialization the cursor's at map 0,0, the top corne
    for i in range(0, E_VARS().MAPWIDTH-1):
        p.move(K_DOWN)

    while True:

        curTile = handleEvents(p,tiles,curTile)
      
        #draw background, placed tiles, and static grid 
        dispSurf.fill(COLORS.BGCOLOR)
        fillTiles(tiles,p,dispSurf)
        drawLowerGrid(dispSurf)

        # draw currently selected tile type at cursor's loc
        image = tiles[curTile].pic
        drawWithOffset(image,(p.coords[STRINGS.X], p.coords[STRINGS.Y]),(OFFSET[0],OFFSET[1]+(p.level * E_VARS().CELLRISE)),dispSurf)

        #make tile grid
        drawUpperGrid(p,dispSurf)

        pygame.display.update()
        FPSCLOCK.tick(E_VARS().FPS)
        print "FRAMERATE:",FPSCLOCK.get_fps()


#-------------------------------------------

def drawWithOffset(image,pos,offset,dispSurf) :
    dispSurf.blit(image,(pos[0]-offset[0], pos[1]-offset[1]))

# Handle keyboard input
def handleEvents(player,tiles,curTile):

    keys = []

    for event in pygame.event.get():

        #exit editor
        if event.type == QUIT: 
            terminate()

        #key press received
        elif event.type == KEYDOWN:
            if event.key not in keys:
                keys.append(event.key)

    for key in keys:
        #exit program
        if key == K_ESCAPE:
            if tkMessageBox.askyesno(title="Save?", message="Do you want to save your changes before exiting?"):
                savePrompt(tiles)
            terminate()

            # Map Editing functions
            #----------------------

        # place the selected tile type at the cursor
        elif key == K_RETURN:
            insertItem(player,curTile)

        # if a tile exists at the cursor, delete it
        elif key == K_BACKSPACE:
            removeItem(player)

        # turn on the collision map
        elif key == K_c:
            setCollide(player)

        # scroll back one tile type
        elif key == K_z:
            if curTile > 0:
                curTile = curTile-1

        # scroll forward one tile type
        elif key == K_x:
            if curTile <  len(tiles)-1:
                curTile = curTile+1

        # save the map to file
        elif key == K_m:
            savePrompt(tiles)

        # move up one elevation
        elif key == K_t:
            OFFSET[1] = OFFSET[1] - E_VARS().CELLRISE
            player.level = player.level + 1

        # move down one elevation
        elif key == K_y:
            if not (player.level == 0):
                OFFSET[1] = OFFSET[1] + E_VARS().CELLRISE
                player.level = player.level - 1
            
        # move the cursor in direction
        elif key==K_a:
            player.move(key)
        elif key == K_d:
            player.move(key)
        elif key == K_w:
            player.move(key)
        elif key == K_s:
            player.move(key)

        #shift the screen
        elif key == K_LEFT:
            OFFSET[0] = OFFSET[0]-E_VARS().CELLWIDTH/2
        elif key == K_RIGHT:
            OFFSET[0] = OFFSET[0] + E_VARS().CELLWIDTH/2
        elif key == K_UP:
            OFFSET[1] = OFFSET[1] - E_VARS().CELLHEIGHT/2
        elif key == K_DOWN:
            OFFSET[1] = OFFSET[1] + E_VARS().CELLHEIGHT/2

    return curTile

#-------------------------------------------

def savePrompt(tiles):

    # open a new window
    new_win = Tkinter.Tk()
    new_win.geometry('+'+str(E_VARS().W_WIDTH/2)+'+'+str(E_VARS().W_HEIGHT/2))

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
def printMap(tiles, fileName, window):

   print("\nPrinting map to file\n")

   file = open('maps/'+fileName+'.xml','w')
   file.write("<?xml version=\"1.0\"?>\n")
   file.write("<map width=\"")
   file.write(str(E_VARS().MAPWIDTH))
   file.write("\" height=\"")
   file.write(str(E_VARS().MAPHEIGHT))
   file.write("\" tilewidth=\"")
   file.write(str(E_VARS().CELLWIDTH))
   file.write("\" tileheight=\"")
   file.write(str(E_VARS().CELLHEIGHT))
   file.write("\" tileRise=\"")
   file.write(str(E_VARS().CELLRISE))
   file.write("\" weather=\"snow\">\n")

   i=0
   file.write("\t<tileset>\n")
   for tile in tiles:
       file.write("\t\t<tile id=\"")
       file.write(str(i))
       file.write("\" name=\"")
       file.write(tile.name)
       file.write("\"/>\n")
       i=i+1
   file.write("\t</tileset>\n")
    
   layerNum=0
   for layer in MAP:
       file.write("\t<layer name=\"")
       file.write(str(layerNum))
       file.write("\">\n")
       rowPos=0
       for tile in layer:
            file.write("\t\t<element id=\"")
            if(tile is 0):
                file.write(str(0))
                file.write("\" collision=\"false\"/>\n")
            else:
                file.write(str(tile["id"]))
                file.write("\" collision=\"")
                file.write(str(tile["collidable"]).lower())
                file.write("\"/>\n")
            if((rowPos+1)%E_VARS().MAPWIDTH is 0):
                rowPos=0
                file.write("\n")
            rowPos=rowPos+1
       file.write("\t</layer>\n")
       layerNum=layerNum+1
   file.write("</map>")
   file.close()

   print "Finished writing map\n"

   window.destroy()

#-------------------------------------------------------------------


# Add a new layer full of empty tiles to the map
def createLayer():

    newLayer = []
    for i in range(0,E_VARS().MAPHEIGHT*E_VARS().MAPWIDTH):
        newLayer.append({"id": 0, "coord": {STRINGS.X:0, STRINGS.Y:0}, "collidable":False})
    return newLayer


#-------------------------------------------------------------


# Place tile at cursor position
def insertItem(player,curTile):

    pos = player.getMapPos()
    drawPos = player.coords

    nMaxX = not (pos[0] > E_VARS().MAPWIDTH-1)
    nMinX = not (pos[0] < 0)
    nHalfX = not (pos[0] % 1.0 > 0)
    nMaxY = not (pos[1] > E_VARS().MAPHEIGHT-1)
    nMinY = not (pos[1] < 0)
    nHalfY = not (pos[1] % 1.0 > 0)

    # check boundaries
    if nMaxX and nMinX and nHalfX and nMaxY and nMinY and nHalfY:
        if player.level > len(MAP)-1:
            for i in range(player.level - len(MAP)+1):
                MAP.append(createLayer())
        MAP[player.level][int(pos[1])*E_VARS().MAPWIDTH+int(pos[0])]["id"] = curTile
        MAP[player.level][int(pos[1])*E_VARS().MAPWIDTH+int(pos[0])]["coord"] = drawPos



# Remove tile at cursor position
def removeItem(player):

    pos =player.getMapPos()
    drawPos = player.coords

    nMaxX = not (pos[0] > E_VARS().MAPWIDTH-1)
    nMinX = not (pos[0] < 0)
    nHalfX = not (pos[0] % 1.0 > 0)
    nMaxY = not (pos[1] > E_VARS().MAPHEIGHT-1)
    nMinY = not (pos[1] < 0)
    nHalfY = not (pos[1] % 1.0 > 0)
    nTooHigh = not (player.level > len(MAP) -1)

    # check boundaries
    if nMaxX and nMinX and nHalfX and nMaxY and nMinY and nHalfY and nTooHigh:
        MAP[player.level][int(pos[1])*E_VARS().MAPWIDTH+int(pos[0])]["id"] = 0
        MAP[player.level][int(pos[1])*E_VARS().MAPWIDTH+int(pos[0])]["coord"] = drawPos
        MAP[player.level][int(pos[1])*E_VARS().MAPWIDTH+int(pos[0])]["collidable"] = False



# Toggle collidable property at cursor position
def setCollide(player):

    pos =player.getMapPos()
    drawPos = player.coords

    nMaxX = not (pos[0] > E_VARS().MAPWIDTH-1)
    nMinX = not (pos[0] < 0)
    nHalfX = not (pos[0] % 1.0 > 0)
    nMaxY = not (pos[1] > E_VARS().MAPHEIGHT-1)
    nMinY = not (pos[1] < 0)
    nHalfY = not (pos[1] % 1.0 > 0)
    nTooHigh = not (player.level > len(MAP) -1)

    # check boundaries
    if nMaxX and nMinX and nHalfX and nMaxY and nMinY and nHalfY and nTooHigh:
        MAP[player.level][int(pos[1])*E_VARS().MAPWIDTH+int(pos[0])]["coord"] = drawPos
        MAP[player.level][int(pos[1])*E_VARS().MAPWIDTH+int(pos[0])]["collidable"] = not MAP[player.level][int(pos[1])*E_VARS().MAPWIDTH+int(pos[0])]["collidable"]
        print "set collidable to ", MAP[player.level][int(pos[1])*E_VARS().MAPWIDTH+int(pos[0])]["collidable"]
    

#--------------------------------------------------------------------------------


# Draw the map
def fillTiles(tiles,player,dispSurf):

    # finding the index for draw order
    num=0
    mapX=0
    mapY=0

    mapWidth = E_VARS().MAPWIDTH
    mapHeight = E_VARS().MAPHEIGHT

    for layer in xrange(len(MAP)):
        for tileNum in xrange(mapHeight*mapWidth):
            tile = MAP[layer][tileNum]
            if tile != 0:
                item = tiles[tile["id"]].pic
                drawWithOffset(item, (tile["coord"][STRINGS.X], tile["coord"][STRINGS.Y]-layer*E_VARS().CELLRISE),(OFFSET[0], OFFSET[1]),dispSurf)
                    

#-------------------------------------------------------------------------------------------------------


# base grid that doesn't move
def drawLowerGrid(dispSurf):
           
    global OFFSET

    for x in range(0,E_VARS().MAPWIDTH+1):

        # down and left
        startX = E_VARS().W_WIDTH/2+x*E_VARS().CELLWIDTH/2-OFFSET[0]
        startY = E_VARS().W_HEIGHT/2-E_VARS().CELLHEIGHT*E_VARS().MAPHEIGHT/2+x*E_VARS().CELLHEIGHT/2-OFFSET[1]
        endX = E_VARS().W_WIDTH/2-E_VARS().MAPWIDTH*E_VARS().CELLWIDTH/2+E_VARS().CELLWIDTH/2*x-OFFSET[0]
        endY = E_VARS().W_HEIGHT/2+x*E_VARS().CELLHEIGHT/2-OFFSET[1]

        pygame.draw.line(dispSurf, 
                         COLORS.BLACK,
                        (startX, startY),
                        (endX, endY),
                         1)
        
        #down and right
        startX = E_VARS().W_WIDTH/2-x*E_VARS().CELLWIDTH/2-OFFSET[0]
        startY = E_VARS().W_HEIGHT/2-E_VARS().CELLHEIGHT*E_VARS().MAPHEIGHT/2+x*E_VARS().CELLHEIGHT/2-OFFSET[1]
        endX = E_VARS().W_WIDTH/2+E_VARS().MAPHEIGHT*E_VARS().CELLWIDTH/2-E_VARS().CELLWIDTH/2*x-OFFSET[0]
        endY = E_VARS().W_HEIGHT/2+x*E_VARS().CELLHEIGHT/2-OFFSET[1]

        pygame.draw.line(dispSurf, 
                         COLORS.BLACK, 
                        (startX, startY),
                        (endX, endY),
                         1)
        

# grid tied to player level
def drawUpperGrid(player,dispSurf):

    global OFFSET

    for x in range(0,E_VARS().MAPWIDTH+1):

        # down and left
        startX = E_VARS().W_WIDTH/2+x*E_VARS().CELLWIDTH/2-OFFSET[0]
        startY = E_VARS().W_HEIGHT/2-E_VARS().CELLHEIGHT*E_VARS().MAPHEIGHT/2+x*E_VARS().CELLHEIGHT/2-player.level*E_VARS().CELLRISE-OFFSET[1]
        endX = E_VARS().W_WIDTH/2-E_VARS().MAPWIDTH*E_VARS().CELLWIDTH/2+E_VARS().CELLWIDTH/2*x-OFFSET[0]
        endY = E_VARS().W_HEIGHT/2+x*E_VARS().CELLHEIGHT/2-player.level*E_VARS().CELLRISE-OFFSET[1]

        pygame.draw.line(dispSurf, 
                         COLORS.WHITE,
                        (startX, startY), 
                        (endX, endY),
                         1)
        
        #down and right
        startX = E_VARS().W_WIDTH/2-x*E_VARS().CELLWIDTH/2-OFFSET[0]
        startY = E_VARS().W_HEIGHT/2-E_VARS().CELLHEIGHT*E_VARS().MAPHEIGHT/2+x*E_VARS().CELLHEIGHT/2-player.level*E_VARS().CELLRISE-OFFSET[1]
        endX = E_VARS().W_WIDTH/2+E_VARS().MAPHEIGHT*E_VARS().CELLWIDTH/2-E_VARS().CELLWIDTH/2*x-OFFSET[0]
        endY = E_VARS().W_HEIGHT/2+x*E_VARS().CELLHEIGHT/2-player.level*E_VARS().CELLRISE-OFFSET[1]

        pygame.draw.line(dispSurf, 
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

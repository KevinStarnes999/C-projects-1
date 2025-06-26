from typing import Callable

grid = [
        [0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0]
        ]


def place_oneBlue(grid: list[list[int]], col: int) -> bool:
    """
    Attempts to place a 1 in the lowest available 0 in the given column.
    Returns True if successful, False if the column is full.
    """
    Flag = False
    for i in range(len(grid) - 1, -1, -1):
        if grid[i][col] == 0:
            grid[i][col] = 1
            Flag = True
            for line in grid:
                print(line)
            break
    if Flag == False:
        print(f"Column {col} is full. No placement possible.")
    return Flag

def place_oneRed(grid: list[list[int]], col: int) -> bool:
    """
    Attempts to place a 1 in the lowest available 0 in the given column.
    Returns True if successful, False if the column is full.
    """
    Flag = False
    for i in range(len(grid) - 1, -1, -1):
        if grid[i][col] == 0:
            grid[i][col] = -1
            Flag = True
            for line in grid:
                print(line)
            break
    if Flag == False:
        print(f"Column {col} is full. No placement possible.")
    return Flag

def userChoice(userinput, call:Callable):
    print("Player Blue Turn")
    if userinput == 99:
        exit()
    calling = call(grid, userinput)
    return calling

run = True
while run == True:

    CurPlayer = 1
    if CurPlayer == 1:
        print("Player Blue Turn")
        userinput = int(input("What row are you choosing from? 0 - 7: "))
        worked = userChoice(userinput, place_oneBlue)
        if worked == True:
            CurPlayer = 0
        else:
            CurPlayer = 1

    if CurPlayer == 0:
        print("Player Red Turn")
        userinput = int(input("What row are you choosing from? 0 - 7: "))
        worked = userChoice(userinput, place_oneRed)
        CurPlayer = 1
        if worked == True:
            CurPlayer = 0
        else:
            CurPlayer = 1



from typing import Callable
import csv
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
    with open('Block 3/grid.csv', "r+",newline='') as grids:
        csv_grid = csv.writer(grids)

        Flag = False
        for i in range(len(grid) - 1, -1, -1):
            if grid[i][col] == 0:
                grid[i][col] = 1
                Flag = True
                for line in grid:
                    csv_grid.writerow(line)
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
    with open('Block 3/grid.csv', "r+",newline='') as grids:
        csv_grid = csv.writer(grids)
        Flag = False
        for i in range(len(grid) - 1, -1, -1):
            if grid[i][col] == 0:
                grid[i][col] = -1
                Flag = True
                for line in grid:
                    csv_grid.writerow(line)
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

def checkwinHorizonal():
    with open('Block 3/grid.csv', "r",newline='') as output:
        countBlue = 0
        countRed = 0
        csv_reader = csv.reader(output)
        for line in csv_reader:
            for index, num in enumerate(line):
                if num == '1':
                    countBlue += 1
                    if countBlue == 4:
                        print("Blue winner!")
                        break
                else:
                    countBlue = 0
                if num == '-1':
                    countRed += 1
                    if countRed == 4:
                        print("Red is the winner!")
                        break
                else: 
                    countRed = 0
    
def checkwinVertical(gridT):
    gridT = grid
    transposed_matrix = []
    for row in zip(*gridT):
        transposed_matrix.append(list(row))
    for line in transposed_matrix:
        for index, num in enumerate(line):  #fix
            if num == '1':
                countBlue += 1
                if countBlue == 4:
                    print("Blue winner!")
                    break
            else:
                countBlue = 0
            if num == '-1':
                countRed += 1
                if countRed == 4:
                    print("Red is the winner!")
                    break
            else: 
                countRed = 0

    print(transposed_matrix)

run = True
while run == True:

    CurPlayer = 1
    if CurPlayer == 1:
        print("Player Blue Turn")
        userinput = int(input("What row are you choosing from? 0 - 7: "))
        worked = userChoice(userinput, place_oneBlue)
        checkwinHorizonal()
        checkwinVertical(grid)
        if worked == True:
            CurPlayer = 0
        else:
            CurPlayer = 1

    if CurPlayer == 0:
        print("Player Red Turn")
        userinput = int(input("What row are you choosing from? 0 - 7: "))
        worked = userChoice(userinput, place_oneRed)
        checkwinVertical(grid)
        CurPlayer = 1
        if worked == True:
            CurPlayer = 0
        else:
            CurPlayer = 1



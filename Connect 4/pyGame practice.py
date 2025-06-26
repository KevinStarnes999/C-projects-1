import pygame
from PIL import Image
pygame.init()


class Connect4():
    def __init__(self):
        self.screen = pygame.display.set_mode((1820, 980))
        self.board = pygame.image.load('board.png').convert()
        self.board = pygame.transform.scale(self.board, (1680,780))
        
    def playerBlue(self):
        self.colorBlue = (0,0,255)
        self.player1 = pygame.draw.circle(self.screen, self.colorBlue, (232, 178), 37) 
    def playerRED(self):
        self.colorRed = (255,0,0)
        self.player1 = pygame.draw.circle(self.screen, self.colorRed, (338, 178), 37) 

    def runGame(self):
        self.yellow = (255,255,0)
        self.run = True
        while self.run == True:
            self.screen.fill(self.yellow)
            self.screen.blit(self.board,(70, 120))
            self.playerBlue()
            self.playerRED()

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.run = False

            pygame.display.update()
        pygame.quit()



Connect4().runGame()
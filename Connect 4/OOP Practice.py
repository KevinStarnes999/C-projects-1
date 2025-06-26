# #Define a Dog class
# class Dog():

#     def __init__ (self, name, age): 
#         self.name = name 
#         self.age = age
    
#     def bark(self):
#         return f"{self.name} says Woof!"
#     # Create an object of the Dog class myDog =
# myDog =  Dog("Rex", 3)
# # Use the object print(myDog.bark())
# print(f"My dog's name is {myDog.name} and he is {myDog.age} years old.")
#----------------------------------------------------------------------------------------------

# class Person:
#     def __init__ (self, name, age):
#         self.name = name #private attribute
#         self.age = age #public attribute
#         #Example getter
#     @property
#     def name(self):
#         return self._name
#         #Example setter
#     @name.setter
#     def name(self, newName):
#         self._name = newName

# person = Person("John", 36)
# print(person.name) # Output John
# print(person.age) # Output 36

#----------------------------------------------------------------------------------------------
# class Shape:
#     def area(self):
#         pass
#     # Rectangle class inheriting from Shape
# class Rectangle(Shape):
#     def __init__ (self, width, length):
#         self.width = width
#         self.length = length
#     def area(self):
#         return self.width * self.length
#     # Circle class inheriting from Shape
# class Circle(Shape):
#     def __init__ (self, radius):
#         self.radius = radius
#     def area(self):
#         return 3.14 * self.radius * self.radius
# shapes = [Rectangle(10, 20), Circle(15)]
# for shape in shapes:
#     print(shape.area())
#----------------------------------------------------------------------------------------------
# import pygame

# import sys

# # Initialize Pygame
# pygame.init()

# # Set up screen
# screen_width = 800
# screen_height = 600
# screen = pygame.display.set_mode((screen_width, screen_height))

# class Ball():
#     def __init__(self):
#         # --- Class Attributes ---
#         # Ball position
#         self.x = 0
#         self.y = 0
 
#         # Ball's vector
#         self.change_x = 0
#         self.change_y = 0
 
#         # Ball size
#         self.size = 10
 
#         # Ball color
#         self.color = [255,255,255]
 
#     # --- Class Methods ---
#     def move(self):
#         self.x += self.change_x
#         self.y += self.change_y
 
#     def draw(self, screen):
#         pygame.draw.circle(screen, self.color, [self.x, self.y], self.size )

# theBall = Ball()
# theBall.x = 100
# theBall.y = 100
# theBall.change_x = 2
# theBall.change_y = 1
# theBall.color = [255,0,0]
# while True:
#     theBall.move()
#     theBall.draw(screen)

#----------------------------------------------------------------------------------------------


# class Cat():
#     def __init__(self,name,color,weight):
#         self.name = name
#         self.color = color
#         self.weight = weight
#     def meow(self):
#         print("\x1B[3mmeeeeeeeow\x1B[0m")

# firstCat = Cat('Miles','Orange',400)
# # firstCat.name = 'Miles'
# # firstCat.color = 'Orange'
# # firstCat.weight = 400
# print(f"My cat is named {firstCat.name}, he is a {firstCat.color} cat, and he is {firstCat.weight} pounds")
# firstCat.meow()

# class Monster():
#     def __init__(self,name:str, health:int):
#         self.name = name
#         self.health = health

#     def decreaseHealth(self,amount):
#         self.health -= amount
#         if self.health < 0:
#             print(f"{self.name} has died")
# firstMonster = Monster("Wick", 200)
# firstMonster.decreaseHealth(210)




import time
import sys

for i in 'ragebaiting is the greatest thing ever':
    print(i, end='')  
    sys.stdout.flush()
    time.sleep(.2)
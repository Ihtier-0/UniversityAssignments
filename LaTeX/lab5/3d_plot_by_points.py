import dislin.dislin as dislin
import math

# https://en.wikipedia.org/wiki/Test_functions_for_optimization
# Eggholder function

xbegin = -1.0
xend = 1.0

xn = 25

xstep = (xend - xbegin) / xn

ybegin = -1.0
yend = 1.0

yn = 25

ystep = (yend - ybegin) / yn

a = 1
b = 1

z = list (range((xn + 1) * (yn + 1)))

for i in range (0, xn + 1):
  x = xbegin + i * xstep
  for j in range (0, yn + 1):
    y = ybegin + j * ystep
    z[i * yn + j] = (x / a) ** 2 - (y / b) ** 2

dislin.metafl ("pdf") # устанавливает формат вывода графика: xwin - вывод на экран, pdf - в pdf файл
dislin.setfil ("hyperbolic_paraboloid.pdf") # название файла

dislin.disini () # инициализирует библиотеку DISLIN
dislin.hwfont () # устанавливает стандартный шрифт
dislin.pagera () # рисует границу страницы

dislin.autres (xn, yn) # количество точек данных в направлениях X и Y
dislin.ax3len (2200, 1400, 1400) # определяет длину осей
dislin.axspos (300, 1850) # определяет положение осей

# Имена осей
dislin.name   ('X', 'X')
dislin.name   ('Y', 'Y')
dislin.name   ('Z', 'Z')

dislin.ticks  (2, 'XYZ') # устанавливает количество тиков (отметок на осях X и Y)
dislin.digits (1, 'XYZ') # количество знаков после запятой. 0 после числа будет точка, -1 точки не будет

# заголовок
# 2 аргумент номер заголовка от 1 до 4х
dislin.titlin ('Hyperbolic Paraboloid', 1)
dislin.titlin ('F(X,Y) = (x / a) ^ 2 - (y / b) ^ 2', 2)
dislin.titlin ('a = {0}, b = {1}'.format(a, b), 3)

dislin.graf3d (-1.0, 1.0, # нижний и верхний пределы оси X
               -1.0, 0.2, # первая метка оси X и шаг между метками
               -1.0, 1.0, # нижний и верхний пределы оси Y
               -1.0, 0.2, # первая метка оси Y и шаг между метками
               -1.0, 1.0, # нижний и верхний пределы оси Z
               -1.0, 0.2) # первая метка оси Z и шаг между метками
dislin.title  () # вывод заголовка

# вывод графика
dislin.color ("red")
dislin.surmat (z, xn, yn, 1, 1)

dislin.disfin () # закончить работу с disfin
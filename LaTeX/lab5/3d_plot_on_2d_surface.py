import dislin.dislin as dislin
import math

# https://en.wikipedia.org/wiki/Test_functions_for_optimization
# Eggholder function

xbegin = -1000
xend = 1000

xn = 500

xstep = (xend - xbegin) / xn

ybegin = -1000
yend = 1000

yn = 500

ystep = (yend - ybegin) / yn

z = list (range((xn + 1) * (yn + 1)))

for i in range (0, xn + 1):
  x = xbegin + i * xstep
  for j in range (0, yn + 1):
    y = ybegin + j * ystep
    z[i * yn + j] = - (y + 47) * math.sin(math.sqrt(abs(x / 2.0 + (y + 47)))) - x * math.sin(math.sqrt(abs(x / 2.0 - (y + 47))))

dislin.metafl ("pdf") # устанавливает формат вывода графика: xwin - вывод на экран, pdf - в pdf файл
dislin.setfil ("eggholder.pdf") # название файла

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
dislin.digits (-1, 'XYZ') # количество знаков после запятой. 0 после числа будет точка, -1 точки не будет

# заголовок
# 2 аргумент номер заголовка от 1 до 4х
dislin.titlin ('eggholder', 1)
dislin.titlin ('F(X,Y) = - (y + 47) * sin(sqrt(abs(x / 2.0 + (y + 47)))) - x * sin(sqrt(abs(x / 2.0 - (y + 47))))', 2)

dislin.graf3  (-1000.0, 1000.0, # нижний и верхний пределы оси X
               -1000.0,  200.0, # первая метка оси X и шаг между метками
               -1000.0, 1000.0, # нижний и верхний пределы оси Y
               -1000.0,  200.0, # первая метка оси Y и шаг между метками
               -2000.0, 2000.0, # нижний и верхний пределы оси Z
               -2000.0,  200.0) # первая метка оси Z и шаг между метками
dislin.title  () # вывод заголовка

# вывод графика
dislin.crvmat (z, xn, yn, 1, 1)

dislin.disfin () # закончить работу с disfin
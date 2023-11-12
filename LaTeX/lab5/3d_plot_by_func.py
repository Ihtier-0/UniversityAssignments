import dislin.dislin as dislin
import math

def mobius_strip(u, v, axis):
  if axis == 1: # x
     result = (1 + (v / 2) * math.cos(u / 2)) * math.cos(u)
  elif axis == 2: # y
     result = (1 + (v / 2) * math.cos(u / 2)) * math.sin(u)
  else: # z
     result = (v / 2) * math.sin(u / 2)
  return result

ubegin = 0
uend = 2 * math.pi

vbegin = -1
vend = 1

un = 25
vn = 25

ustep = (uend - ubegin) / un
vstep = (vend - vbegin) / vn

# поменять местами цвет фона и графиков
# стандартно график белый, фон черный
dislin.scrmod ('revers')
dislin.metafl ("pdf") # устанавливает формат вывода графика: xwin - вывод на экран, pdf - в pdf файл
dislin.setfil ("mobius_strip.pdf") # название файла

dislin.disini () # инициализирует библиотеку DISLIN
dislin.hwfont () # устанавливает стандартный шрифт
dislin.pagera () # рисует границу страницы

dislin.axslen (1800, 1800) # определяет длину осей
dislin.axspos (200, 2400) # определяет положение осей

# Имена осей
dislin.name   ('X', 'X')
dislin.name   ('Y', 'Y')
dislin.name   ('Z', 'Z')
dislin.intax  ()

# заголовок
# 2 аргумент номер заголовка от 1 до 4х
dislin.titlin ('Mobius strip', 1)
dislin.titlin ('[(1 + (v / 2) * cos(u / 2)) * cos(u), (1 + (v / 2) * cos(u / 2)) * sin(u), (v / 2) * sin(u / 2)]', 2)

# определяет расстояние между системами осей и заголовками.
# Если аргумент - отрицательное значение, пространство будет уменьшено на координаты графика NV.
# Если аргумент - положительное, пространство будет увеличено на координаты графика NV.
# По умолчанию: 0
dislin.vkytit (-500)

dislin.zscale (-0.5, 0.5) # градиент по оси Z
dislin.surmsh ('on') # рисовать сетку на поверхности

dislin.graf3d (-2.0, 2.0, # нижний и верхний пределы оси X
               -2.0, 0.5, # первая метка оси X и шаг между метками
               -2.0, 2.0, # нижний и верхний пределы оси Y
               -2.0, 0.5, # первая метка оси Y и шаг между метками
               -2.0, 2.0, # нижний и верхний пределы оси Z
               -2.0, 0.5) # первая метка оси Z и шаг между метками
dislin.title  () # вывод заголовка

dislin.surfcp (mobius_strip, # параметрическая функция
               ubegin, uend, # нижний и верхний пределы первого параметра
               ustep,        # шаг первого параметра
               vbegin, vend, # нижний и верхний пределы второго параметра
               vstep)        # шаг второго параметра

dislin.disfin () # закончить работу с disfin
import dislin.dislin as dislin
import math

# https://en.wikipedia.org/wiki/Lima%C3%A7on
# улитка Паскаля

begin = 0
end = 2 * math.pi

n = 100

step = (end - begin) / n

R = 1
h = 2

x = list (range (n + 1))
y = list (range (n + 1))

for i in range (0, n + 1):
  t = begin + i * step
  x[i] = 2 * R * math.cos(t) - h * math.cos(2 * t)
  y[i] = 2 * R * math.sin(t) - h * math.sin(2 * t)

dislin.metafl ("pdf") # устанавливает формат вывода графика: xwin - вывод на экран, pdf - в pdf файл
dislin.setfil ("limacon.pdf") # название файла

dislin.disini () # инициализирует библиотеку DISLIN
dislin.hwfont () # устанавливает стандартный шрифт
dislin.pagera () # рисует границу страницы

dislin.axslen (2200, 1200) # определяет длину осей
dislin.axspos (450, 1800) # определяет положение осей

# Имена осей
dislin.name   ('X', 'X')
dislin.name   ('Y', 'Y')

dislin.ticks  (2, 'XY') # устанавливает количество тиков (отметок на осях X и Y)
dislin.digits (-1, 'XY') # количество знаков после запятой. 0 после числа будет точка, -1 точки не будет

# заголовок
# 2 аргумент номер заголовка от 1 до 4х
dislin.titlin ('limacon', 1)
dislin.titlin ('(2 * R * cos(t) - h * cos(2 * t), 2 * R * sin(t) - h * sin(2 * t))', 2)
dislin.titlin ("R = {0}, h = {1}".format(R, h), 3)
 
dislin.graf   (-5.0, 5.0, # нижний и верхний пределы оси X
               -5.0, 1.0, # первая метка оси X и шаг между метками
               -5.0, 5.0, # нижний и верхний пределы оси Y
               -5.0, 1.0) # первая метка оси Y и шаг между метками
dislin.title  () # вывод заголовка
    
# вывод красного графика
dislin.color  ('red')
dislin.curve  (x, y, n)

# график Y = 0
dislin.color  ('foreground')
dislin.xaxgit () # нарисовать график Y = 0

# график X = 0
dislin.color  ('foreground')
dislin.yaxgit () # нарисовать график X = 0

dislin.disfin () # закончить работу с disfin
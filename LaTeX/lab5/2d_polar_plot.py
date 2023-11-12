import dislin.dislin as dislin
import math

# https://en.wikipedia.org/wiki/Rose_(mathematics)
# роза

begin = 0
end = 2 * math.pi

n = 300

step = (end - begin) / n

a = 1
k = 7

r = list (range (n + 1))
phi = list (range (n + 1))

for i in range (0,n):
  phi[i] = begin + i * step
  r[i] = a * math.sin(k * phi[i])

dislin.metafl ("pdf") # устанавливает формат вывода графика: xwin - вывод на экран, pdf - в pdf файл
dislin.setfil ("rose.pdf") # название файла

dislin.disini () # инициализирует библиотеку DISLIN
dislin.hwfont () # устанавливает стандартный шрифт
dislin.pagera () # рисует границу страницы

dislin.axslen (2200, 1200) # определяет длину осей
dislin.axspos (450, 1800) # определяет положение осей

dislin.ticks  (2, 'XY') # устанавливает количество тиков (отметок на осях X и Y)
dislin.digits (-1, 'Y') # количество знаков после запятой. 0 после числа будет точка, -1 точки не будет
dislin.digits (2, 'X') # количество знаков после запятой. 0 после числа будет точка, -1 точки не будет

# заголовок
# 2 аргумент номер заголовка от 1 до 4х
dislin.titlin ('rose', 1)
dislin.titlin ('p = a * sin(k * phi)', 2)
dislin.titlin ("a = {0}, k = {1}".format(a, k), 3)

dislin.polar  (1.,       # видимый радиус
               0., 0.25, # первая метка оси Х и шаг между метками
               0., 30.)  # первая метка угла и шаг между метками
dislin.title  () # вывод заголовка

# вывод красного графика
dislin.color  ('red')
dislin.curve  (r, phi, n)

dislin.disfin () # закончить работу с disfin
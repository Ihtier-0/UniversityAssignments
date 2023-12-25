import matplotlib.pyplot as plt
from math import sqrt, cos, sin, pi
from numpy import linalg as LA

################################################################################
### bisection_method
################################################################################

# метод деления отрезка пополам
def bisection_method(f,                      # функция корень которой необходимо найти
                     begin:float, end:float, # начало и конец отрезка в котором находится корень
                     steps:int,              # максимальное количество шагов алгоритма
                     eps:float):             # точность приближения корня
    l = begin
    r = end

    step = 0

    while step < steps:
        step += 1
        center = 0.5 * (r + l)

        f_l = f(l)
        f_c = f(center)
        f_r = f(r)

        if abs(f_c) < eps:
            break

        if f_l * f_c < 0:
            r = center
        else: # f_c * f_r < 0
            l = center

    return 0.5 * (r + l)

################################################################################
### quantum_mechanics
################################################################################

def v(x:float) -> float:
    if (x > -half_width and x < (-half_width / 2.0)) or (x > 0 and x < half_width):
        return -1
    elif x >= -half_width / 2.0 and x <= 0:
        return 0
    else:
        return w

#  потенциальная функция
def U(x:float) -> float:
    return v0 * v(x)

def q(e:float,
      x:float) -> float:
    return 2.0 * (e - U(x))

def compute_q(x:list[float],
              energy:float) -> list[float]:
    return [q(energy, xi) for xi in x]

def derivative(function:list[float],
               index:int,
               eps:float) -> float:
    der1 = function[index - 2] - function[index + 2]
    der2 = function[index + 1] - function[index - 1]
    return (
              (der1 + 8.0 * der2)
           / #-------------------
                 (12.0 * eps)
           )

# интегрирование вперёд
def forward_integration(num_intervals:int,
                        penultimate_approximation:float,
                        q:list[float],
                        step:float) -> list[float]:
    num_points = num_intervals + 1
    forward = [float] * (num_points)

    forward[0] = 0.0
    forward[1] = penultimate_approximation

    c = step ** 2 / 12.0

    for i in range(1, num_intervals):
        p1 = 2.0 * (1.0 - 5.0 * c * q[i]) * forward[i]
        p2 = (1.0 + c * q[i - 1]) * forward[i - 1]
        p3 = (1.0 + c * q[i + 1])
        forward[i + 1] = (
                         (p1 - p2)
                     / #-----------
                            p3
                        )

    return forward

# интегрирование назад
def backward_integration(num_intervals:int,
                         first_approximation:float,
                         q:list[float],
                         step:float) -> list[float]:
    num_points = num_intervals + 1
    backward = [float] * num_points

    backward[num_intervals] = 0
    backward[num_intervals - 1] = first_approximation

    c = step ** 2 / 12.0

    for i in range(num_intervals - 1, 0, -1):
        f1 = 2.0 * (1.0 - 5.0 * c * q[i]) * backward[i]
        f2 = (1.0 + c * q[i + 1]) * backward[i + 1]
        backward[i - 1] = (
                            (f1 - f2)
                    / #--------------------
                       (1.0 + c * q[i - 1])
                    )

    return backward

# фукнция для нормировки forward
# и достижения равенства на узле сшивки
def normalization(forward:list[float],
                  backward:list[float],
                  connection:int):
    # нормировка
    norm = abs(max(forward, key = abs))
    forward = list(map(lambda x: x / norm, forward))

    # равенство на узле сшивки - connection
    coef = forward[connection] / backward[connection]
    backward = list(map(lambda x: coef * x, backward))

    return forward, backward

# функция возвращающая разницу производных на узле сшивки
def is_close(forward:list[float],
             backward:list[float],
             connection:int,
             eps:float) -> float:
    return (derivative(forward, connection, eps)
          - derivative(backward, connection, eps))

# функция вычисляет значения волновой функции вперёд и назад по известным данным
# и возвращающая разницу производных на узле сшивки
def is_close_energy(
             energy:float,                             # значение энергии
             x:list[float],                            # сетка
             step:float,                               # шаг сетки
             forward_first_approximation:float,        # первое приближение для интегрирования вперёд
             backward_penultimate_approximation:float, # n - 1 приближение для интегрирования назад
             connection:int) -> float:                 # узел сшивки
    num_intervals = len(x) - 1
    q = compute_q(x, energy)
    forward = forward_integration(num_intervals, forward_first_approximation, q, step)
    backward = backward_integration(num_intervals, backward_penultimate_approximation, q, step)
    forward, backward = normalization(forward, backward, connection)
    return is_close(forward, backward, connection, step)

# возвращет интервалы в которых находятся собственные значения оператора Гамильтона
def eigen_value_intervals(min_energy:float,
                          energy_step:float,
                          max_energy_value:float,
                          max_energy_count:int,
                          x:list[float],
                          step:float,
                          forward_first_approximation:float,        # 1 приближение для интегрирования вперёд
                          backward_penultimate_approximation:float, # n - 1 приближения для интегрирования назад
                          connection:int) -> list[float]:
    level = 0
    sign = (1 if level % 2 == 0 else -1)
    energy = min_energy
    prev_close = is_close_energy(energy,
                                 x,
                                 step,
                                 sign * forward_first_approximation,
                                 backward_penultimate_approximation,
                                 connection)

    intervals = []
    intervals.append(energy)

    while(energy < max_energy_value):
        energy = energy + energy_step
        sign = (1 if level % 2 == 0 else -1)
        close = is_close_energy(energy,
                                x,
                                step,
                                sign * forward_first_approximation,
                                backward_penultimate_approximation,
                                connection)

        if close * prev_close < 0:
            prev_close = close
            intervals.append(energy)
            intervals.append(energy)
            level += 1

            if(len(intervals) >= 2 * max_energy_count):
                break
        else:
            intervals[-1] = energy

    intervals.pop()
    return intervals

def integrate(f:list[float], step:float) -> float:
    size = len(f)

    sum = 0.0
    for i in range(1, size - 1):
        sum += f[i]
    return step * ((f[0] + f[size - 1]) / 2.0 + sum)

def quantum_normalize(psi:list[float], step:float) -> list[float]:
    density = list(map(lambda x: x * x, psi))
    c = integrate(density, step)
    root = 1.0 / sqrt(c)
    normalized = list(map(lambda x: root * x, psi))
    return normalized

def second_derivation(f:list[float], step:float):
    num_points = len(f)
    derivation = [float] * num_points

    coef = 1 / (step * step)

    derivation[0] = coef * (2 * f[0] - 5 * f[1] + 4 * f[2] - f[3])
    derivation[num_points - 1] = (coef *
                                  (2 * f[num_points - 1]
                                 - 5 * f[num_points - 2]
                                 + 4 * f[num_points - 3]
                                     - f[num_points - 4]))

    for i in range(1, num_points - 1):
        derivation[i] = coef * (f[i - 1] - 2 * f[i] + f[i + 1])

    return derivation

w = 10.0

v0 = 15 # Электронвольт
half_width = 2.0 # Ангстрем

print(f"v0 = {v0} Электронвольт")
print(f"l = {half_width} Ангстрем")

# Перевод величин в атомную систему единиц
v0 = v0 / 27.211 # атомных единиц
half_width = half_width / 0.5292 # атомных единиц

print(f"v0 = {v0} атомных единиц")
print(f"l = {half_width} атомных единиц")

def main():
    min_energy = -v0
    print("Минимальное значение потенциальной функции = {}".format(min_energy))
    print()

    forward_first_approximation = 1.e-9
    backward_penultimate_approximation = 1.e-9

    begin = -half_width
    end = +half_width

    num_intervals = 1000
    num_points = num_intervals + 1

    connection = 300

    step = (end - begin) / num_intervals
    x = [begin + step * i for i in range(num_points)]

    energy_step = 0.01
    max_energy_value = 1000
    max_energy_count = 2

    intervals = eigen_value_intervals(min_energy,
                                      energy_step,
                                      max_energy_value,
                                      max_energy_count,
                                      x,
                                      step,
                                      forward_first_approximation,
                                      backward_penultimate_approximation,
                                      connection)
    print("intervals: ", intervals)

    bisection_method_steps = 100
    bisection_method_eps = 0.0001

    energies = []
    level = 0
    f = lambda e : is_close_energy(e,
                                   x,
                                   step,
                                   forward_sign * forward_first_approximation,
                                   backward_penultimate_approximation,
                                   connection)
    for i in range(max_energy_count):
        l = intervals[2 * i]
        r = intervals[2 * i + 1]
        forward_sign = (1 if level % 2 == 0 else -1)
        energy = bisection_method(
            f,
            l, r,
            bisection_method_steps,
            bisection_method_eps)
        energies.append(energy)

        level += 1
    print("energies: ", energies, " a.u.")
    print()

    # значения для построения графиков
    u = [U(xi) for xi in x]

    q_n0 = compute_q(x, energies[0])
    forward_n0 = forward_integration(num_intervals, forward_first_approximation, q_n0, step)
    backward_n0 = backward_integration(num_intervals, backward_penultimate_approximation, q_n0, step)
    forward_n0, backward_n0 = normalization(forward_n0, backward_n0, connection)
    forward_n0 = quantum_normalize(forward_n0, step)
    backward_n0 = quantum_normalize(backward_n0, step)

    def phi(k:int, x:list[float]):
        num_points  = len(x)
        out = [float] * num_points

        denominator = 1.0 / sqrt(half_width)

        for i in range(num_points):
            arg = (pi * (k + 1) * x[i]) / (2.0 * half_width)

            if k % 2:
                out[i] = denominator * sin(arg)
            else:
                out[i] = denominator * cos(arg)

        return out

    def Hphi(k:int, x:list[float]):
        num_points  = len(x)
        out = [float] * num_points

        phi_k = phi(k, x)
        derivation = second_derivation(phi_k, step)

        for i in range(num_points):
            out[i] = -0.5 * derivation[i] + u[i] * phi_k[i]

        return out

    def H(m:int, k:int, x:list[float]):
        phi_m = phi(m, x)
        Hphi_k = Hphi(k, x)

        num_points  = len(x)
        mul = [float] * num_points
        for i in range(num_points):
            mul[i] = phi_m[i] * Hphi_k[i]
        return integrate(mul, step)

    def get_Matrix(size:int, x:list[float]):
        return [[H(m, k, x) for k in range(size)] for m in range(size)]

    size = 6
    matrix = get_Matrix(size, x)
    eigenvalues, eigenvectors = LA.eigh(matrix)
    low = 0
    for i in range(1, len(eigenvalues)):
        if eigenvalues[i] < eigenvalues[low]:
            low = i

    num_points  = len(x)
    wave_func = [0.0] * num_points
    for k in range(size):
        phi_k = phi(k, x)
        for i in range(num_points):
            wave_func[i] += eigenvectors[k][low] * phi_k[i]
    wave_func = quantum_normalize(wave_func, step)

    En = eigenvalues[low]
    print(f"прямой вариационный метод - E = {En}")
    print(f"метод пристрелки - E = {energies[0]}")

    plt.axis([begin, end, min_energy, v0 if v0 > 1 else 1])
    plt.grid(True)
    plt.axhline(0, color='black')
    plt.axvline(0, color='black')
    plt.xlabel("x, a.u.", fontsize = 20, color = "k")
    plt.plot(x, forward_n0, linewidth = 6, color = "orange", label = "$phi(x)$, a.u., метод пристрелки")
    plt.plot(x,  wave_func, linewidth = 2, color = "blue"  , label = "$phi(x)$, a.u., вариационный принцип")
    plt.plot(x,          u, linewidth = 6, color = "green" , label = "$U(x)$, a.u.")
    plt.legend()
    plt.show()

main()

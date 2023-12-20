import matplotlib.pyplot as plt
from math import sqrt

################################################################################
### shooting begin
################################################################################

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

# Потенциальная функция невозмущенной системы
def U(x:float) -> float:
    return v0 * v(x)

# Потенциальная функция возмущенной системы
def U1(x:float) -> float:
    if (x > (-half_width / 2 + half_width / 5) and x < (-half_width / 5)):
        return U(x) + 0.5
    else:
        return U(x)

# возмущение
def V(x:float):
    return U(x) - U1(x)

def U_pr(x:float):
    return U(x) + V(x)

SOLVE_U = U

def q(e:float,
      x:float) -> float:
    return 2.0 * (e - SOLVE_U(x))

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
half_width = half_width / 0.5292

print(f"v0 = {v0} атомных единиц")
print(f"l = {half_width} бор")

def main():
    global SOLVE_U

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

    bisection_method_steps = 100
    bisection_method_eps = 0.0001

################################################################################
### shooting perturbation
################################################################################

    SOLVE_U = U1

    intervals_pr = eigen_value_intervals(min_energy,
                                         energy_step,
                                         max_energy_value,
                                         max_energy_count,
                                         x,
                                         step,
                                         forward_first_approximation,
                                         backward_penultimate_approximation,
                                         connection)
    print("intervals_pr: ", intervals_pr)

    energies_pr = []
    level_pr = 0
    f_pr = lambda e : is_close_energy(e,
                                      x,
                                      step,
                                      forward_sign * forward_first_approximation,
                                      backward_penultimate_approximation,
                                      connection)
    for i in range(max_energy_count):
        l = intervals_pr[2 * i]
        r = intervals_pr[2 * i + 1]
        forward_sign = (1 if level_pr % 2 == 0 else -1)
        energy = bisection_method(
            f_pr,
            l, r,
            bisection_method_steps,
            bisection_method_eps)
        energies_pr.append(energy)

        level_pr += 1
    print("energies_pr: ", energies_pr, " a.u.")
    print()

    q_n0_pr = compute_q(x, energies_pr[0])
    forward_n0_pr = forward_integration(num_intervals, forward_first_approximation, q_n0_pr, step)
    backward_n0_pr = backward_integration(num_intervals, backward_penultimate_approximation, q_n0_pr, step)
    forward_n0_pr, backward_n0_pr = normalization(forward_n0_pr, backward_n0_pr, connection)
    forward_n0_pr = quantum_normalize(forward_n0_pr, step)
    backward_n0_pr = quantum_normalize(backward_n0_pr, step)

    q_n1_pr = compute_q(x, energies_pr[1])
    forward_n1_pr = forward_integration(num_intervals, forward_first_approximation, q_n1_pr, step)
    backward_n1_pr = backward_integration(num_intervals, backward_penultimate_approximation, q_n1_pr, step)
    forward_n1_pr, backward_n1_pr = normalization(forward_n1_pr, backward_n1_pr, connection)
    forward_n1_pr = quantum_normalize(forward_n1_pr, step)
    backward_n1_pr = quantum_normalize(backward_n1_pr, step)

################################################################################
### shooting idealized
################################################################################

    SOLVE_U = U

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

    q_n0 = compute_q(x, energies[0])
    forward_n0 = forward_integration(num_intervals, forward_first_approximation, q_n0, step)
    backward_n0 = backward_integration(num_intervals, backward_penultimate_approximation, q_n0, step)
    forward_n0, backward_n0 = normalization(forward_n0, backward_n0, connection)
    forward_n0 = quantum_normalize(forward_n0, step)
    backward_n0 = quantum_normalize(backward_n0, step)

    q_n1 = compute_q(x, energies[1])
    forward_n1 = forward_integration(num_intervals, forward_first_approximation, q_n1, step)
    backward_n1 = backward_integration(num_intervals, backward_penultimate_approximation, q_n1, step)
    forward_n1, backward_n1 = normalization(forward_n1, backward_n1, connection)
    forward_n1 = quantum_normalize(forward_n1, step)
    backward_n1 = quantum_normalize(backward_n1, step)

################################################################################
### shooting end
################################################################################

################################################################################
### perturbation begin
################################################################################

    # энергии
    # energies

    # волновые функциий
    wave = [forward_n0, forward_n1]

    # Потенциальная функция невозмущенной системы
    U0 = U

    # Потенциальная функция возмущенной системы
    # U1

    # возмущение
    # V

    def matrix_V(n:int, m:int):
        v_n_m = [wave[n][i] * v[i] * wave[m][i] for i in range(num_points)]
        return integrate(v_n_m, step)

    x = [begin + step * i for i in range(num_points)]
    v = [V(xi) for xi in x]
    u0 = [U0(xi) for xi in x]
    u1 = [U1(xi) for xi in x]

    energy_index = 0

    # первая поправка энергии
    first_order_correction_e = matrix_V(energy_index, energy_index)
    print("первая поправка энергии=", first_order_correction_e)

    # вторая поправка энергии
    secont_order_correction_e = 0
    for i in range(len(energies)):
        if(i != energy_index):
            secont_order_correction_e += (
                abs(matrix_V(energy_index, i)) ** 2
            / #--------------------------------------
               (energies[energy_index] - energies[i])
            )
    print("первая поправка энергии=", secont_order_correction_e)

    # первая поправка собственной функции
    const =  matrix_V(0, 1) / (energies[0] - energies[1])
    mul = [const * w for w in wave[1]]
    first_order_correction_w = [wave[0][i] + mul[i] for i in range(len(wave[1]))]

    # вывод
    energy = energies[0]
    print("E_0 = ", energy)
    energy += first_order_correction_e
    print("E_0 = ", energy, " 1-ое приближение, ", "поправка =", first_order_correction_e)
    energy += secont_order_correction_e
    print("E_0 = ", energy, " 2-ое приближение, ", "поправка =", secont_order_correction_e)

    # Графики
    plt.axis([begin, end, min_energy, v0])
    plt.grid(True)
    plt.axhline(0, color='black')
    plt.axvline(0, color='black')
    plt.xlabel("x, a.u.", fontsize = 20, color = "k")
    plt.ylabel("$U^0(x)$, a.u.", fontsize = 20, color = "k")
    plt.plot(x, u0, linewidth = 6, color = "red", label = "$U^0(x)$")
    plt.legend()
    plt.show()

    plt.axis([begin, end, min_energy, v0])
    plt.grid(True)
    plt.axhline(0, color='black')
    plt.axvline(0, color='black')
    plt.xlabel("x, a.u.", fontsize = 20, color = "k")
    plt.ylabel("U(x), a.u.", fontsize = 20, color = "k")
    plt.plot(x, u1, linewidth = 6, color = "red", label = "U(x)")
    plt.legend()
    plt.show()

    plt.axis([begin, end, min_energy, v0 if v0 > 1 else 1])
    plt.grid(True)
    plt.axhline(0, color='black')
    plt.axvline(0, color='black')
    plt.xlabel("x, a.u.", fontsize = 20, color = "k")
    plt.plot(x,            forward_n0_pr, linewidth = 6, color = "orange", label = "$phi(x)$, a.u., метод пристрелки")
    plt.plot(x, first_order_correction_w, linewidth = 3, color = "blue"  , label = "$phi(x)$, a.u., теория возмущений")
    plt.plot(x,                       u1, linewidth = 6, color = "green" , label = "$U(x)$, a.u.")
    plt.legend()
    plt.show()

################################################################################
### perturbation end
################################################################################

main()

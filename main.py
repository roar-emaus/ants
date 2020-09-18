import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

from enum import Enum


patch_size = 1  # mm
time_step = 0.1  # s

n_patches = 50
n_ants = 1


class Status(Enum):
    IN_NEST = 1
    FORAGING = 2
    RETURN = 3


class Ant:
    def __init__(self, sigma1: float = 50, sigma2: float = 15):
        self._x = 0
        self._y = 0
        self.sigma1 = sigma1
        self.sigma2 = sigma2
        self.job = Status.IN_NEST
        self.counter = 0
        self._movement_x = 1
        self._movement_y = 1

    @property
    def x(self):
        return self._x

    @x.setter
    def x(self, new_x):
        self._x = new_x

    @property
    def y(self):
        return self._y

    @y.setter
    def y(self, new_y):
        self._y = new_y

    def _eps1(self):
        return np.random.normal(loc=0, scale=self.sigma1)

    def _eps2(self):
        return np.random.normal(loc=0, scale=self.sigma2)

    def _exit_nest(self):
        return np.random.randint(0, 100) < 1

    def move(self):
        if self.job == Status(1):
            if self._exit_nest():
                self.x = np.random.randint(0, 2) * 2 - 1
                self.y = np.random.randint(0, 2) * 2 - 1
                self.job = Status(2)
        elif self.job == Status(2):
            self.x += self._movement_x
            self.y += self._movement_y
            self.counter += 1
            if self.counter > 4:
                alpha = self._eps2()
                self._movement_x = np.cos(alpha)
                self._movement_y = np.sin(alpha)
                self.counter = 0


if __name__ == "__main__":
    ants = [Ant() for _ in range(10)]
    arena = np.asarray([[-50, 50], [-50, 50]])

    def init():
        ax.set_xlim(arena[0])
        ax.set_ylim(arena[1])
        lines = [ax.plot([], [], "--") for _ in ants]
        return (lines,)

    fig, ax = plt.subplots()
    lines = [ax.plot([], [], "--") for _ in ants]
    xdatas = [[] for _ in ants]
    ydatas = [[] for _ in ants]

    def data_gen():
        [ant.move() for ant in ants]
        yield ants

    def run(ants):
        for i, ant in enumerate(ants):

            xdatas[i].append(ant.x)
            ydatas[i].append(ant.y)
            lines[i][0].set_data(xdatas[i], ydatas[i])

        return (lines,)

    ani = animation.FuncAnimation(fig, run, data_gen, interval=200, init_func=init)
    plt.show()

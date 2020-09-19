import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

from enum import Enum

from typing import List, Tuple, Optional


class Status(Enum):
    IN_NEST = 1
    FORAGING = 2
    RETURN = 3


class Ant:
    def __init__(
            self, start_position: Tuple[int, int] = (0, 0), sigma1: float = 50, sigma2: float = 15, release_fraction: int = 1000
    ):
        self._x = start_position[0]
        self._y = start_position[1]
        self.sigma1 = sigma1
        self.sigma2 = sigma2
        self.job = Status.IN_NEST
        self.counter = 0
        self._movement_x, self._movement_y = self._update_movement()
        self._release_fraction = release_fraction

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

    @property
    def coords(self):
        return np.asarray([self.x, self.y])

    @property
    def _eps1(self):
        return np.random.normal(loc=0, scale=self.sigma1)

    @property
    def _eps2(self):
        return np.random.normal(loc=0, scale=self.sigma2)

    @property
    def _exit_nest(self):
        return np.random.randint(0, self._release_fraction) < 1

    def _update_movement(self):
        alpha = self._eps2
        return np.cos(alpha), np.sin(alpha)

    def move(self):
        if self.job == Status(1):
            if self._exit_nest:
                self.x += np.random.randint(0, 2) * 2 - 1
                self.y += np.random.randint(0, 2) * 2 - 1
                self.job = Status(2)
        elif self.job == Status(2):
            self.x += self._movement_x
            self.y += self._movement_y
            self.counter += 1
            if self.counter > 4:
                self._movement_x, self._movement_y = self._update_movement()
                self.counter = 0

        return np.asarray([self.x, self.y])


class Arena:
    def __init__(
        self,
        patch_size: int,
        grid_size: Tuple[int, int],
        nest_position: Optional[List[Tuple[float, float]]] = None,
        food_positions: Optional[List[Tuple[float, float]]] = None,
    ):
        """
        Args:
            patch_size: an int describing each patch size in mm
            ends: list of 4 ints describing the size of rectangular arena with indexes being left, right, bottom, top
            food_positions: NOT IMPLEMENTED: list of tuples of floats describing coordinates of food.
        """
        self._nest_positions = []
        self._food_positions = food_positions
        self._patch_size = patch_size
        self._grid = np.zeros((grid_size[0], grid_size[1]))

    @property
    def grid_size(self):
        return (len(self._grid[0]), len(self._grid[1]))

    @property
    def left(self):
        return self._left

    @property
    def right(self):
        return self._right

    @property
    def bottom(self):
        return self._bottom

    @property
    def top(self):
        return self._top

    @property
    def patch_size(self):
        return self._patch_size


class AnimatedScatter:
    """Animation function for ants in an arena with scatter plot"""

    def __init__(self, ants: List[Ant], arena: Arena, interval):
        self.ants = ants
        self.arena = arena

        self.xs = np.asarray([])
        self.ys = np.asarray([])

        self.stream = self.data_stream()

        self.fig, self.ax = plt.subplots()
        self.ani = animation.FuncAnimation(
            self.fig,
            self.update,
            interval=interval,
            init_func=self.setup_plot,
            blit=True,
        )

    def setup_plot(self):
        """Initial drawing of the arena."""
        x, y = next(self.stream).T

        self.scat = self.ax.scatter(x, y, cmap="jet", edgecolor="k")
        self.ax.axis(
            [0, self.arena.grid_size[0], 0, self.arena.grid_size[1]]
        )

        return self.scat,

    def data_stream(self):
        """Move ants a time step"""
        while True:
            yield np.asarray([ant.move() for ant in ants])

    def update(self, i):
        """Update the scatter plot."""
        data = next(self.stream)

        # Set x and y data...
        self.scat.set_offsets(data)
        return (self.scat,)


if __name__ == "__main__":
    arena = Arena(patch_size=1, grid_size=(50, 50))
    nest_position = (25, 25)
    ants = [Ant(start_position=nest_position, release_fraction=100) for _ in range(100)]
    ani = AnimatedScatter(ants, arena, interval=100)
    plt.show()

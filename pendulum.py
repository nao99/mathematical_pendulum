from dotenv import load_dotenv
from matplotlib.animation import FuncAnimation
import os
import csv
import matplotlib.pyplot as plt

load_dotenv()

# 0. Define constants
PENDULUM_LOADS_COORDINATES_PATH = 'data/pendulum_loads_coordinates.csv'
LINE_ARGUMENTS = 'o-'
LINE_REDRAW_TIME_MS = 25
TIME_FORMAT = 'time = %.1fs'
TIME_SPEED_COEFFICIENT = 0.05
TIME_POSITION_COEFFICIENT = 0.9

VISUALIZATION_TIME_IN_S = float(os.getenv('VISUALIZATION_TIME_IN_S', 0))
PENDULUM_LOAD_ANGLES_IN_DEG_1 = float(os.getenv('PENDULUM_LOAD_ANGLES_IN_DEG_1', 0))
PENDULUM_LOAD_ANGLES_IN_DEG_2 = float(os.getenv('PENDULUM_LOAD_ANGLES_IN_DEG_2', 0))
PENDULUM_LOAD_ANGLES_VELOCITY_IN_DEG_1 = float(os.getenv('PENDULUM_LOAD_ANGLES_VELOCITY_IN_DEG_1', 0))
PENDULUM_LOAD_ANGLES_VELOCITY_IN_DEG_2 = float(os.getenv('PENDULUM_LOAD_ANGLES_VELOCITY_IN_DEG_2', 0))

# 1. Check if need to recalculate loads coordinates of pendulum
RECALCULATE_PENDULUM_LOADS_COORDINATES = int(os.getenv('RECALCULATE_PENDULUM_LOADS_COORDINATES', 0))
if 1 == RECALCULATE_PENDULUM_LOADS_COORDINATES:
    command = f'./build/mathematical_pendulum ' \
              f'{VISUALIZATION_TIME_IN_S} {PENDULUM_LOAD_ANGLES_IN_DEG_1} {PENDULUM_LOAD_ANGLES_IN_DEG_2} ' \
              f'{PENDULUM_LOAD_ANGLES_VELOCITY_IN_DEG_1} {PENDULUM_LOAD_ANGLES_VELOCITY_IN_DEG_2} ' \
              f'> {PENDULUM_LOADS_COORDINATES_PATH}'

    os.system(command)

# 2. Read calculated loads to a map
pendulum_loads_coordinates_count = 0
pendulum_loads_coordinates_map = {
    'x1': [],
    'x2': [],
    'y1': [],
    'y2': [],
}

with open(PENDULUM_LOADS_COORDINATES_PATH, 'r') as pendulum_loads_coordinates:
    reader = csv.reader(pendulum_loads_coordinates)
    for index, row in enumerate(reader):
        pendulum_loads_coordinates_map['x1'].append(float(row[0]))
        pendulum_loads_coordinates_map['x2'].append(float(row[1]))
        pendulum_loads_coordinates_map['y1'].append(float(row[2]))
        pendulum_loads_coordinates_map['y2'].append(float(row[3]))

        pendulum_loads_coordinates_count += 1

if 0 == pendulum_loads_coordinates_count:
    print('Pendulum loads coordinates not found')
    exit(1)

# 3. Visualize
figure = plt.figure()

'''
1. Draw the main plane: Cartesian plane from -2 to 2 by Oy and from -2 to 2 by Ox

The first 1 - "the first row"
The second 1 - "the first column"
The third 1 - "the first cell"
'''
cartesian_plane_initial_axes = 111
cartesian_plane_initial_x_coordinates = (-2, 2)
cartesian_plane_initial_y_coordinates = (-2, 2)

cartesian_plane = figure.add_subplot(
    cartesian_plane_initial_axes,
    autoscale_on=False,
    xlim=cartesian_plane_initial_x_coordinates,
    ylim=cartesian_plane_initial_y_coordinates,
)

cartesian_plane.grid()

'''
2. Set initial data to graph
    - Draw broken line
    - Draw timer
'''
broken_line, = cartesian_plane.plot([], [], LINE_ARGUMENTS, lw=2)
time_text_block = cartesian_plane.text(
    TIME_SPEED_COEFFICIENT,
    TIME_POSITION_COEFFICIENT,
    None,
    transform=cartesian_plane.transAxes,
)

'''
3. Animate graph
'''


def change_loads_coordinates(i):
    broken_line_coordinates_x = [0, pendulum_loads_coordinates_map['x1'][i], pendulum_loads_coordinates_map['y1'][i]]
    broken_line_coordinates_y = [0, pendulum_loads_coordinates_map['x2'][i], pendulum_loads_coordinates_map['y2'][i]]

    broken_line.set_data(broken_line_coordinates_x, broken_line_coordinates_y)

    time_text = TIME_FORMAT % (i * TIME_SPEED_COEFFICIENT)
    time_text_block.set_text(time_text)

    return broken_line, time_text_block


FuncAnimation(
    figure,
    change_loads_coordinates,
    pendulum_loads_coordinates_count,
    interval=LINE_REDRAW_TIME_MS,
    blit=True,
)

plt.show()

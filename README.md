# Mathematical Pendulum
Simple project for [double mathematical pendulum](https://en.wikipedia.org/wiki/Double_pendulum) visualization
using the Lagrange equations

## Installation
Clone project <br>
```
git clone https://github.com/nao99/mathematical_pendulum
cd mathematical_pendulum
```

Build script for coordinates calculation
```
cmake -D CMAKE_C_COMPILER=/usr/bin/gcc -H. -Bbuild
make -C build
```

Run visualization script and enjoy it
```
python3 pendulum.py
```

## Supportable environment variables
| Variable | Description |
| :------: | :---------: |
| RECALCULATE_PENDULUM_LOADS_COORDINATES | Every time  when application start pendulum coordinates will be recalculated (Optional) |
| VISUALIZATION_TIME_IN_S | Defines visualization time in seconds format (Required)|
| PENDULUM_LOAD_ANGLES_IN_DEG_1 | Start pendulum load 1 deviation in degrees (Required) |
| PENDULUM_LOAD_ANGLES_IN_DEG_2 | Start pendulum load 2 deviation in degrees (Required) |
| PENDULUM_LOAD_ANGLES_VELOCITY_IN_DEG_1 | Start pendulum load 1 angles velocity in degrees (Required) |
| PENDULUM_LOAD_ANGLES_VELOCITY_IN_DEG_2 | Start pendulum load 2 angles velocity in degrees (Required) |

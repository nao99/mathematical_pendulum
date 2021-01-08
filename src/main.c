#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GRAVITY_ACCELERATION 9.81
#define PENDULUM_LENGTH_IN_METERS_1 1.0
#define PENDULUM_LENGTH_IN_METERS_2 1.0
#define PENDULUM_MASS_IN_KILOGRAMS_1 1.0
#define PENDULUM_MASS_IN_KILOGRAMS_2 1.0
#define STEP_LENGTH 0.05

/**
 * Calculates a next angle velocity for upper pendulum load
 *
 * @param m1     pendulum load mass in kilograms 1
 * @param m2     pendulum load mass in kilograms 2
 * @param l1     pendulum length in meters 1
 * @param l2     pendulum length in meters 2
 * @param omega1 pendulum load angle velocity in radians 1
 * @param omega2 pendulum load angle velocity in radians 2
 * @param theta1 pendulum load angle in radians 1
 * @param theta2 pendulum load angle in radians 2
 *
 * @return a next angle velocity for upper pendulum load (.ω1)
 */
extern double calculate_next_angle_velocity_for_upper_pendulum_load(
    double m1,
    double m2,
    double l1,
    double l2,
    double omega1,
    double omega2,
    double theta1,
    double theta2
);

/**
 * Calculates a next angle velocity for lower pendulum load
 *
 * @param m1     pendulum load mass in kilograms 1
 * @param m2     pendulum load mass in kilograms 2
 * @param l1     pendulum length in meters 1
 * @param l2     pendulum length in meters 2
 * @param omega1 pendulum load angle velocity in radians 1
 * @param omega2 pendulum load angle velocity in radians 2
 * @param theta1 pendulum load angle in radians 1
 * @param theta2 pendulum load angle in radians 2
 *
 * @return a next angle velocity for lower pendulum load (.ω2)
 */
extern double calculate_next_angle_velocity_for_lower_pendulum_load(
    double m1,
    double m2,
    double l1,
    double l2,
    double omega1,
    double omega2,
    double theta1,
    double theta2
);

/**
 * Composes motion equations <br>
 *
 * For more understanding how does it works
 * @see https://scienceworld.wolfram.com/physics/DoublePendulum.html
 *
 * @param current_pendulum_position a current pendulum position
 * @param next_pendulum_position    a next pendulum position
 */
extern void compose_motion_equations(double current_pendulum_position[], double next_pendulum_position[]);

/**
 * Solves motion equations <br>
 *
 * This method uses the Runge-Kutta method for each pendulum position parameter
 *
 * For more understanding how does it works
 * @see https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
 *
 * @param current_pendulum_position a current pendulum position
 * @param next_pendulum_position    a next pendulum position
 */
extern void solve_motion_equations(double current_pendulum_position[], double next_pendulum_position[]);

/**
 * Application entrypoint
 *
 * @return an application status code
 */
int main(int argc, char* argv[]) {
    if (5 != argc - 1) {
        printf("Excepted 5 parameters: T, TH1, TH2, W1, W2. But %d given", argc);
        return 1;
    }

    // 1. Initialize input parameters: T, TH1, TH2, W1 and W2
    double pendulum_oscillation_time_in_seconds = strtod(argv[1], NULL);
    double pendulum_load_angle_in_degrees_1 = strtod(argv[2], NULL);
    double pendulum_load_angle_in_degrees_2 = strtod(argv[3], NULL);
    double pendulum_load_angle_velocity_in_degrees_1 = strtod(argv[4], NULL);
    double pendulum_load_angle_velocity_in_degrees_2 = strtod(argv[5], NULL);

    // 2. Calculate a number of steps
    int steps_number = (int) (pendulum_oscillation_time_in_seconds / STEP_LENGTH);

    // 3. Transfer angles from degrees to radians
    double pendulum_load_angle_in_radians_1 = pendulum_load_angle_in_degrees_1 * M_PI / 180.0;
    double pendulum_load_angle_in_radians_2 = pendulum_load_angle_in_degrees_2 * M_PI / 180.0;
    double pendulum_load_angle_velocity_in_radians_1 = pendulum_load_angle_velocity_in_degrees_1 * M_PI / 180.0;
    double pendulum_load_angle_velocity_in_radians_2 = pendulum_load_angle_velocity_in_degrees_2 * M_PI / 180.0;

    double current_pendulum_position[4] = {
        pendulum_load_angle_in_radians_1,
        pendulum_load_angle_velocity_in_radians_1,
        pendulum_load_angle_in_radians_2,
        pendulum_load_angle_velocity_in_radians_2,
    };

    double next_pendulum_position[4];

    // 4. Calculate next pendulum position
    for (int i = 0; i < steps_number; i++) {
        solve_motion_equations(current_pendulum_position, next_pendulum_position);

        double x1 = PENDULUM_LENGTH_IN_METERS_1 * sin(next_pendulum_position[0]);
        double y1 = - PENDULUM_LENGTH_IN_METERS_1 * cos(next_pendulum_position[0]);

        double x2 = x1 + PENDULUM_LENGTH_IN_METERS_2 * sin(next_pendulum_position[2]);
        double y2 = y1 - PENDULUM_LENGTH_IN_METERS_2 * cos(next_pendulum_position[2]);

        printf("%lf,%lf,%lf,%lf\n", x1, y1, x2, y2);

        current_pendulum_position[0] = next_pendulum_position[0];
        current_pendulum_position[1] = next_pendulum_position[1];
        current_pendulum_position[2] = next_pendulum_position[2];
        current_pendulum_position[3] = next_pendulum_position[3];
    }

    return 0;
}

/**
 * {@inheritDoc}
 */
double calculate_next_angle_velocity_for_upper_pendulum_load(
    double m1,
    double m2,
    double l1,
    double l2,
    double omega1,
    double omega2,
    double theta1,
    double theta2
) {
    double delta = theta2 - theta1;
    double sin_delta = sin(delta);
    double cos_delta = cos(delta);

    double numerator_part_1 = m2 * l1 * pow(omega1, 2) * sin_delta * cos_delta;
    double numerator_part_2 = m2 * GRAVITY_ACCELERATION * sin(theta2) * cos_delta;
    double numerator_part_3 = m2 * l2 * pow(omega2, 2) * sin_delta;
    double numerator_part_4 = (m1 + m2) * GRAVITY_ACCELERATION * sin(theta1);

    double denominator = (m1 + m2) * l1 - m2 * l1 * pow(cos_delta, 2);

    return (numerator_part_1 + numerator_part_2 + numerator_part_3 - numerator_part_4) / denominator;
}

/**
 * {@inheritDoc}
 */
double calculate_next_angle_velocity_for_lower_pendulum_load(
    double m1,
    double m2,
    double l1,
    double l2,
    double omega1,
    double omega2,
    double theta1,
    double theta2
) {
    double delta = theta2 - theta1;
    double sin_delta = sin(delta);
    double cos_delta = cos(delta);

    double numerator_part_1 = -m2 * l2 * pow(omega2, 2) * sin_delta * cos_delta;

    double numerator_part_2 = GRAVITY_ACCELERATION * sin(theta1) * cos_delta;
    numerator_part_2 -= l1 * pow(omega1, 2) * sin_delta;
    numerator_part_2 -= GRAVITY_ACCELERATION * sin(theta2);

    double numerator_part_3 = (m1 + m2) * numerator_part_2;
    double denominator = (m1 + m2) * l2 - m2 * l2 * pow(cos_delta, 2);

    return (numerator_part_1 + numerator_part_3) / denominator;
}

/**
 * {@inheritDoc}
 */
void compose_motion_equations(double current_pendulum_position[], double next_pendulum_position[]) {
    next_pendulum_position[0] = current_pendulum_position[1];
    next_pendulum_position[1] = calculate_next_angle_velocity_for_upper_pendulum_load(
        PENDULUM_MASS_IN_KILOGRAMS_1,
        PENDULUM_MASS_IN_KILOGRAMS_2,
        PENDULUM_LENGTH_IN_METERS_1,
        PENDULUM_LENGTH_IN_METERS_2,
        current_pendulum_position[1],
        current_pendulum_position[3],
        current_pendulum_position[0],
        current_pendulum_position[2]
    );

    next_pendulum_position[2] = current_pendulum_position[3];
    next_pendulum_position[3] = calculate_next_angle_velocity_for_lower_pendulum_load(
        PENDULUM_MASS_IN_KILOGRAMS_1,
        PENDULUM_MASS_IN_KILOGRAMS_2,
        PENDULUM_LENGTH_IN_METERS_1,
        PENDULUM_LENGTH_IN_METERS_2,
        current_pendulum_position[1],
        current_pendulum_position[3],
        current_pendulum_position[0],
        current_pendulum_position[2]
    );
}

/**
 * {@inheritDoc}
 */
void solve_motion_equations(double current_pendulum_position[], double next_pendulum_position[]) {
    double dydx[4];
    double dydxt[4];
    double yt[4];

    double F1[4];
    double F2[4];
    double F3[4];
    double F4[4];

    compose_motion_equations(current_pendulum_position, dydx);
    for (int i = 0; i < 4; i++) {
        F1[i] = STEP_LENGTH * dydx[i];
        yt[i] = current_pendulum_position[i] + 0.5 * F1[i];
    }

    compose_motion_equations(yt, dydxt);
    for (int i = 0; i < 4; i++) {
        F2[i] = STEP_LENGTH * dydxt[i];
        yt[i] = current_pendulum_position[i] + 0.5 * F2[i];
    }

    compose_motion_equations(yt, dydxt);
    for (int i = 0; i < 4; i++) {
        F3[i] = STEP_LENGTH * dydxt[i];
        yt[i] = current_pendulum_position[i] + F3[i];
    }

    compose_motion_equations(yt, dydxt);
    for (int i = 0; i < 4; i++) {
        F4[i] = STEP_LENGTH * dydxt[i];
        next_pendulum_position[i] = current_pendulum_position[i] + (F1[i] + 2 * F2[i] + 2 * F3[i] + F4[i]) / 6;
    }
}

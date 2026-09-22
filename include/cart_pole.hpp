#pragma once

struct CartPoleState {
    double x = 0.0;
    double velocity = 0.0;
    double theta = 0.8;
    double omega = 0.0;
};

struct CartPoleParameters {
    double length = 2.5;
    double gravity = 9.81;
    double pendulum_mass = 0.1;
    double cart_mass = 1.0;
    double dt = 0.01;
};

class CartPole {

public:
    CartPole();//constructor. might be useful for the future.

    void step(double force, double torque);
    void reset();

    const CartPoleState& state() const;
    const CartPoleParameters& parameters() const;

private:
    CartPoleState state_;
    CartPoleParameters params_;
};

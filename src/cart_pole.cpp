#include "cart_pole.hpp"
#include <cmath>

CartPole::CartPole() = default;

void CartPole::reset() {
    state_ = CartPoleState{};
}

const CartPoleState& CartPole::state() const {
    return state_;
}

const CartPoleParameters& CartPole::parameters() const {
    return params_;
}

void CartPole::step(double force, double torque) {
    const double L = params_.length;
    const double g = params_.gravity;
    const double m = params_.pendulum_mass;
    const double m_cart = params_.cart_mass;
    const double dt = params_.dt;

    const double theta = state_.theta;
    const double omega = state_.omega;

    // Coupled equations of motion as a 2x2 linear system.
    const double A11 = L;
    const double A12 = std::cos(theta);
    const double A21 = m * L * std::cos(theta);
    const double A22 = m_cart + m;

    const double b1 = -g * std::sin(theta) + torque / (m * L);
    const double b2 = force + m * L * std::sin(theta) * omega * omega;

    const double det = A11 * A22 - A12 * A21;

    const double theta_ddot = (b1 * A22 - b2 * A12) / det;
    const double x_ddot = (A11 * b2 - b1 * A21) / det;

    // Semi-implicit Euler integration.
    state_.omega += theta_ddot * dt;
    state_.theta += state_.omega * dt;

    state_.velocity += x_ddot * dt;
    state_.x += state_.velocity * dt;
}

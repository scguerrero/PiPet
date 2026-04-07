/*
 * Clock is for keeping track of time elapsed in seconds.
 * Author(s): Sasha C. Guerrero
 */

#include "clock.h"

// Default constructor
Clock::Clock() {
    m_elapsed_time = 0;
}

// Return time elapsed in seconds as an integer
int Clock::elapsed_time() {
    return m_elapsed_time;
}

// Set time elapsed in seconds
void Clock::set_elapsed_time(int t) {
    m_elapsed_time = t;
}

// Increment current elapsed time by a given integer
void Clock::increment_elapsed_time(int t) {
    m_elapsed_time += t;
}

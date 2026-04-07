/*
 * Clock is for keeping track of time elapsed in seconds.
 * Author(s): Sasha C. Guerrero
 */

#ifndef CLOCK_H
#define CLOCK_H

class Clock
{
private:
    // Time elapsed in seconds
    int m_elapsed_time;
public:
    // Constructor
    Clock();

    // Getters
    int elapsed_time();

    // Setters
    void set_elapsed_time(int);

    // Incrementers
    void increment_elapsed_time(int);
};

#endif // CLOCK_H

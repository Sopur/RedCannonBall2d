#pragma once

class RedCannonBall {
public:
    RedCannonBall();
    ~RedCannonBall();

    void iteration();      // Preform single iterations
    void iteration(int n); // Preform multiple iterations
};

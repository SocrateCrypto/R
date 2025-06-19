#pragma once

enum class State
{
    Initial,
    Manual, 
    GiroScope,
    Scan,
   
};

class StateMachine
{
public:
    StateMachine();

    State getState() const;
    void setState(State newState);
    bool is(State state) const;

private:
    State currentState;
};
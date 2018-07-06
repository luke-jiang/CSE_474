## Important Notes:
The implementation of traffic light controller FSM involves using one 1Hz timer. The implementation is based on the following preconditions:
1. the switch for on_off and switch for passnger will never be pushed together at any state.
2. A switch push delay of 1 second (at most) is tolerated. This scenario would happen if someone pushes the button just after the clock is cleared and it takes more than 2 seconds to change into the next state. It is possible to increase the speed of the clock for better accuracy.
3. A switch push happened after 3s at each clock cycle is of no effect. See the dev folder for another implementation using two timers that, when the push happend after 3s, would process a 2s timer associated with then pushbuttons first and delays
the 5s timer.

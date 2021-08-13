
/***** INTERNALS *****/

typedef struct {
    int rotation; // 0 stop    1 down    -1 up        (right hand)
    int speed_ref;
    int spindown_time;
    int spinup_time;
} Motor;
Motor _motors = {0, 40, 500, 500};


/***** INTERFACE *****/

// reads from status_buffer

// does not write 



/***** PROTOTYPES *****/

void motors_loop();

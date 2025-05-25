#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

typedef enum {
    BUTTON_NONE,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_SELECT,
    BUTTON_UP_LONG,
    BUTTON_DOWN_LONG,
    BUTTON_SELECT_LONG
} button_event_t;

void input_manager_init(void);
button_event_t input_manager_get_event(void);

#endif // INPUT_MANAGER_H

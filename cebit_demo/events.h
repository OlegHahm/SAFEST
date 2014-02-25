#ifndef EVENTS_H
#define EVENTS_H

typedef enum {
    PARENT_SELECT = 0,
    PARENT_DELETE = 1,
    DIO_RCVD = 2,
    DTA_RCVD = 3,
    ALRM = 4,
    CONFIRM = 5
} evt_t;

#endif /* EVENTS_H */

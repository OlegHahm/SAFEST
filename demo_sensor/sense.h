#ifndef __SENSE_H
#define __SENSE_H

/**
 * @brief		Initialize the sensor functionality
 *
 * Initialize the SMB380 acceleration sensor and start a new thread
 * which will continuously smaple acceleration values.
 */
void sense_init(void);

/**
 * @brief		Enable the sensing event loop
 *
 * Enable the sensing functionality.
 */
void sense_enable(void);

/**
 * @brief		Disable the sensing event loop
 *
 * This will stop the sensing event loop, which means that no events will be detected.
 */
void sense_disable(void);


#endif /* __SENSE_H */
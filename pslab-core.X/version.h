/**
 * @file version.h
 * @brief Version information.
 */

#ifndef VERSION_H
#define	VERSION_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @brief Send hardware version.
 * @return DO_NOT_BOTHER
 */
response_t VERSION_SendHw(void);

#ifdef	__cplusplus
}
#endif

#endif	/* VERSION_H */


#ifndef VERSION_H
#define	VERSION_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @brief Get hardware version.
     * @return DO_NOT_BOTHER
     */
    response_t VERSION_GetVersion(void);

#ifdef	__cplusplus
}
#endif

#endif	/* VERSION_H */

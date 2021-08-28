#ifndef HEADER__MACROS
#define HEADER__MACROS

#define return_error(err, msg) \
    if (err == -1)             \
    {                          \
        perror(msg);           \
        return err;            \
    }

#endif

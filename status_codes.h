#ifndef HEADER__STATUS_CODES
#define HEADER__STATUS_CODES

struct status_code_description
{
    int code;
    const char *description;
};

const struct status_code_description STATUS_CODES_DESCRIPTION[54];

const char *get_status_code_description(int code);

#endif

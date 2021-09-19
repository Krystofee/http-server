#ifndef HEADER__UTILS
#define HEADER__UTILS

// Methods
#define HTTP_METHOD_GET (1)
#define HTTP_METHOD_POST (2)
#define HTTP_METHOD_PUT (3)
#define HTTP_METHOD_PATCH (4)
#define HTTP_METHOD_OPTIONS (5)
#define HTTP_METHOD_HEAD (6)

// Protocols
#define HTTP_PROTOCOL_HTTP_1 (1)

int method_to_code(char *str);
char *code_to_method(int code);

int get_protocol_code(char *str);

char *int_to_string(long int n, char *result, int result_length);

#endif

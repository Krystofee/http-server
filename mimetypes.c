#include <stdlib.h>
#include <memory.h>

#include "mimetypes.h"

char *guess_mimetype(char *filename)
{
    int filename_length = strlen(filename);

    if (filename_length > 5 && strcmp(&filename[filename_length - 5], ".html") == 0)
    {
        return "text/html";
    }
    else if (filename_length > 4 && strcmp(&filename[filename_length - 4], ".css") == 0)
    {
        return "text/css";
    }
    else if (filename_length > 4 && strcmp(&filename[filename_length - 4], ".jpg") == 0)
    {
        return "image/jpg";
    }
    else if (filename_length > 4 && strcmp(&filename[filename_length - 4], ".png") == 0)
    {
        return "image/png";
    }

    return "unknown";
}

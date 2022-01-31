#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

// Get Linux Home Direction
// Attention: this function is not threat-safe
// Refer: https://stackoverflow.com/questions/2910377/get-home-directory-in-linux
char* getLinuxHomeDir()
{
    char *homedir = getenv("HOME");
    if (homedir != NULL)
        return homedir;

    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);

    if (pw == NULL)
        return NULL;

    return pw->pw_dir;
}

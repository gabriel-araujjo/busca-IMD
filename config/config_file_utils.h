//
// Created by Pedro Paulo on 11/06/2016.
//

#ifndef BUSCA_IMD_CONFIG_UTILS_H
#define BUSCA_IMD_CONFIG_UTILS_H

namespace busca_imd_config {
    // Returns the home path,
    // delete home path after use it
    char * getHomeDir();

    //create the hidden config directory on the user folder
    bool createHiddenDir(const char * path);

    //return whether the directory exists
    bool dirExists(const char * path);

    //return whether the file exists
    bool fileExists(const char * path);

    //join a path
    void joinPath(char *parent, const char *file);

    //get the file separator
    const char * getFileSeparator();
}

#endif //BUSCA_IMD_CONFIG_UTILS_H

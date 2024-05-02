#include "messages.h"

#include <stdio.h>
#include <string.h>

Message GLOBAL_MSGS[MESSAGES_COUNT];

/**
 *  \brief Populates the global singleton GLOBAL_MSGS with contents of the provided file.
 *  \param sourceFile Path to the target localization file.
 *
 *  If no matching file was found, perror() is called and the singleton is not populated with anything.
 */

void msgInitFromFile(const char* sourceFile){
    FILE* locFile = fopen(sourceFile, "r");
    if (!locFile){
        perror("Failed to locate the localizations file!");
        return;
    }

    for (int i = 0; i < MESSAGES_COUNT; ++i){
        //Explanation of those funny looking regexp's - https://stackoverflow.com/a/20108623/21342746
        fscanf(locFile, "%30s ", GLOBAL_MSGS[i].key);
        fscanf(locFile, "%120[^\n]\n", GLOBAL_MSGS[i].message);
    }
    fclose(locFile);
}

/**
 *  \brief Retrieves a message with the matching key value.
 *  \param messages An array of Message structs to search through.
 *  \param key The key to look for.
 *  \returns Text of the message with the matching key.
 *  If no key was found, the message will be MESSAGE_NOKEYFOUND_FORMAT macro formatted to contain the invalid key.
 */
char* msgGet(const Message messages[], const char* key){
    for (int i = 0; i < MESSAGES_COUNT; i++){
        if (strcmp(messages[i].key, key) == 0){
            return messages[i].message;
        }
    }
    return "NO MESSAGE FOR THIS KEY FOUND";

    //Excuse the mess
    //Based on this source: NOTES sections of https://devdocs.io/c/io/fprintf
    //int requiredSize = snprintf(NULL, 0, MESSAGES_NOKEYFOUND_FORMAT, key);
    //char buffer[requiredSize + 1]; //remember about null terminating byte
    //snprintf(buffer, sizeof(buffer), MESSAGES_NOKEYFOUND_FORMAT, key);
    //char* finalLiteral = buffer;
    //return finalLiteral;
}

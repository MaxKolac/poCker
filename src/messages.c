#include "messages.h"

#include <stdio.h>
#include <string.h>

void msgLoadFromFile(const char* sourceFile, Message messages[]){
    FILE* locFile = fopen(sourceFile, "r");
    if (!locFile){
        perror("Failed to locate the localizations file!");
        return;
    }

    for (int i = 0; i < MESSAGES_COUNT; ++i){
        //Explanation of those funny looking regexp's - https://stackoverflow.com/a/20108623/21342746
        fscanf(locFile, "%30s ", messages[i].key);
        fscanf(locFile, "%80[^\n]\n", messages[i].message);
    }
    fclose(locFile);
}

char* msgGet(Message messages[], const char* key){
    for (int i = 0; i < MESSAGES_COUNT; i++){
        if (strcmp(messages[i].key, key) == 0){
            return messages[i].message;
        }
    }
    return "NO MESSAGE FOUND";
}

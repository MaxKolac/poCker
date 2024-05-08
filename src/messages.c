#include "messages.h"

#include <stdio.h>
#include <string.h>

/**
 *  \brief A global singleton containing Message structs.
 *  \warning Make sure to populate it with msgInitFromFile() function.
 */
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
        if (feof(locFile)){
            printf("End of file reached when more messages where promised! Some message requests may turn up with errors.\n");
            fclose(locFile);
            return;
        }
        //Explanation of those funny looking regexp's - https://stackoverflow.com/a/20108623/21342746
        //fscanf(locFile, "%30s ", GLOBAL_MSGS[i].key);
        //fscanf(locFile, "%120[^\n]\n", GLOBAL_MSGS[i].message);

        //1. Load an entire line from loc.txt into memory - its maximum length is key + space + message + NULL terminator
        //2. The string format in fscanf means:
        // - load at max 150 chars but stop prematurily if you find a char in the inverted set (just \n) then consume a '\n' char
        //3. strtok replaces the first whitespace character with '/0' char, it always returns a pointer to first char
        //4. string pointed to by key_beginning is now "KEY/000". msg_beginning should point at key_beginning PLUS its string length PLUS 1 null terminator.
        //5. copy everything, dont just assign it. Those char pointers will point at garbage once for loop starts again.
        char buffer[MESSAGES_MAX_KEY_LENGTH + 1 + MESSAGES_MAX_MSG_LENGTH + 1];
        char* key_beginning;
        char* msg_beginning;
        fscanf(locFile, "%150[^\n]\n", buffer);
        key_beginning = strtok(buffer, " ");
        msg_beginning = key_beginning + (sizeof(char) * (strlen(key_beginning) + 1));
        strcpy(GLOBAL_MSGS[i].key, key_beginning);
        strcpy(GLOBAL_MSGS[i].message, msg_beginning);
    }
    fclose(locFile);
}

/**
 *  \brief Retrieves a message with the matching key value.
 *  \param messages An array of Message structs to search through.
 *  \param key The key to look for.
 *  \returns Text of the message with the matching key. If no key was found, the message will be MESSAGE_NOKEYFOUND_FORMAT.
 *
 *  Rules of strcmp() apply when it comes to key strings comparison.
 */
char* msgGet(const Message messages[], const char* key){
    for (int i = 0; i < MESSAGES_COUNT; i++){
        if (strcmp(messages[i].key, key) == 0){
            return messages[i].message;
        }
    }
    return MESSAGES_NOKEYFOUND;
}

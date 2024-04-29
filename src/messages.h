#ifndef MESSAGES_H
#define MESSAGES_H

/**
 *  \file messages.h
 *  \brief Allows to use string literals loaded from a separate text file through a dictionary of KeyValuePairs.
 */

/**
 *  \brief Maximum length of a single key
 *  \warning If you change this value, make sure that the regular expression inside msgInitFromFile matches the new value!
 */
#define MESSAGES_MAX_KEY_LENGTH 30
/**
 *  \brief Maximum length of a single message text
 *  \warning If you change this value, make sure that the regular expression inside msgInitFromFile matches the new value!
 */
#define MESSAGES_MAX_MSG_LENGTH 120
/** \brief Current amount of lines contained within the loc.txt file */
#define MESSAGES_COUNT 15
/** \brief Defines the localization's filename */
#define MESSAGES_FILENAME "loc.txt"
/** \brief String literal format used when no matching key was found in the global singleton. */
#define MESSAGES_NOKEYFOUND_FORMAT "NO MESSAGE FOR KEY '%s' FOUND"

/** \brief A message string with a unique Key. */
typedef struct {
    char key[MESSAGES_MAX_KEY_LENGTH];
    char message[MESSAGES_MAX_MSG_LENGTH];
} Message;

/**
 *  \brief A global singleton that holds all the message strings.
 *  \warning Make sure to populate it with msgInitFromFile(const char*) beforehand!
 */
extern Message GLOBAL_MSGS[MESSAGES_COUNT];

void msgInitFromFile(const char*);
char* msgGet(Message[], const char*);

#endif // MESSAGES_H

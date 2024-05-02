#ifndef MESSAGES_H
#define MESSAGES_H

/**
 *  \file messages.h
 *  \brief Allows to use string literals loaded from a separate text file through a dictionary of key/message pairs.
 */

/**
 *  \brief Maximum length of a single key
 *  \warning If you change this value, make sure that the regular expression inside msgInitFromFile matches the new value!
 */
#define MESSAGES_MAX_KEY_LENGTH 30
/**
 *  \brief Maximum length of a single message text
 *  \warning If you change this value, make sure that the regular expression inside msgInitFromFile matches the new value!
 *
 *  Keep in mind this value only cares about the raw line inside the localization file.
 *  If a really lengthy line includes string formatting, like '%d', replacing it with a great integer might result in a string longer than the value of this macro.
 */
#define MESSAGES_MAX_MSG_LENGTH 120
/** \brief Current amount of lines contained within the loc.txt file */
#define MESSAGES_COUNT 24
/** \brief Defines the localization's filename */
#define MESSAGES_FILENAME "loc.txt"
/** \brief String literal returned when no message with a matching key was found. */
#define MESSAGES_NOKEYFOUND "NO MESSAGE FOUND FOR THIS KEY"

/** \brief Quick macro for showing a message. */
#define MSG_SHOW(dict,key)       printf(msgGet(dict,key))
/** \brief Quick macro for showing a message. Prints a newline character at the end. */
#define MSG_SHOWN(dict,key)      printf(msgGet(dict,key)); printf("\n")
/** \brief Quick macro for showing a message. Prints a space character at the end. */
#define MSG_SHOWS(dict,key)      printf(msgGet(dict,key)); printf(" ")

/** \brief Quick macro for showing a message. Accepts variadic arguments. */
#define MSG_SHOWV(dict,key,...)  printf(msgGet(dict,key),__VA_ARGS__)
/** \brief Quick macro for showing a message. Accepts variadic arguments and prints a newline character at the end. */
#define MSG_SHOWVN(dict,key,...) printf(msgGet(dict,key),__VA_ARGS__); printf("\n")
/** \brief Quick macro for showing a message. Accepts variadic arguments and prints a space character at the end. */
#define MSG_SHOWVS(dict,key,...) printf(msgGet(dict,key),__VA_ARGS__); printf(" ")

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
char* msgGet(const Message[], const char*);

#endif // MESSAGES_H

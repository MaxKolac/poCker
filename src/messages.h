#ifndef MESSAGES_H
#define MESSAGES_H

/** \brief Maximum length of a single key */
#define MESSAGES_MAX_KEY_LENGTH 30
/** \brief Maximum length of a single message text */
#define MESSAGES_MAX_MSG_LENGTH 80
/** \brief Current amount of lines contained within the loc.txt file */
#define MESSAGES_COUNT 3

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

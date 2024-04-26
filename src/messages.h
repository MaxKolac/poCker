#ifndef MESSAGES_H
#define MESSAGES_H

/** \brief Maximum length of a single key */
#define MESSAGES_MAX_KEY_LENGTH 30
/** \brief Maximum length of a single message text */
#define MESSAGES_MAX_MSG_LENGTH 80
/** \brief Predetermined amount of messages contained within the loc.txt file */
#define MESSAGES_COUNT 3

typedef struct {
    char key[MESSAGES_MAX_KEY_LENGTH];
    char message[MESSAGES_MAX_MSG_LENGTH];
} Message;

void msgLoadFromFile(const char*, Message[]);
char* msgGet(Message[], const char*);

#endif // MESSAGES_H

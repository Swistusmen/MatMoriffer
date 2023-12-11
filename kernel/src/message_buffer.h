#include <linux/kthread.h>

struct message
{
    char content[100];
    struct message *next;
} typedef message;

static void initialize_message(message *msg, char *content)
{
    msg->next = NULL;
    strcpy(msg->content, content);
}

struct message_buffer
{
    message *head;
    message *tail;
    int size;
    struct mutex message_buffer_mutex;
} typedef message_buffer;

static void initialize_message_buffer(message_buffer *buffer)
{
    buffer->size = 0;
    buffer->head = NULL;
    buffer->tail = NULL;
}

static void free_message_buffer(message_buffer *buffer)
{
    mutex_lock(&buffer->message_buffer_mutex);
    message *temp;
    while (buffer->head)
    {
        temp = buffer->head;
        buffer->head = buffer->head->next;
        kfree(temp);
    }
    mutex_unlock(&buffer->message_buffer_mutex);
}

static void push_message(message_buffer *buffer, message *msg)
{
    mutex_lock(&buffer->message_buffer_mutex);
    if (buffer->head == NULL)
    {
        buffer->head = msg;
    }
    else
    {
        buffer->tail->next = msg;
    }
    buffer->tail = msg;
    buffer->size++;
    mutex_unlock(&buffer->message_buffer_mutex);
}

static void pop_message(message_buffer *buffer, message **msg)
{
    mutex_lock(&buffer->message_buffer_mutex);
    if (buffer->head == NULL)
    {
        *msg = NULL;
    }
    else
    {
        *msg = buffer->head;
        buffer->head = (*msg)->next;
        (*msg)->next = NULL;
        buffer->size--;
    }
    mutex_unlock(&buffer->message_buffer_mutex);
}

#include "types.h"
#include <time.h>
#include <stdarg.h>
 
char user_message[USER_MSG_LEN];
int user_message_len;
time_t user_message_time;

public void show_message(char *message, ...)
{
	user_message_time = time(NULL);
	va_list ap;
	va_start(ap, message);
	user_message_len = vsnprintf(user_message, USER_MSG_LEN, message, ap);
	va_end(ap);
	update_command_bar();
}

public void clear_message()
{
	user_message[(user_message_len = 0)] = 0;
}

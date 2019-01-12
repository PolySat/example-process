#include <polysat3/cmd.h>
#include "example_pkt.h"

int main(int argc, char **argv)
{
   enum XDR_PRINT_STYLE style;

   IPC_EXAMPLE_forcelink();
   return CMD_send_command_line_command(argc, argv, NULL, NULL,
         CMD_print_response, &style, 3000, "payload", &style);
}

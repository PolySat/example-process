/**
 * @file paylaod.c Example libproc process
 *
 */

#include <polysat/polysat.h>
#include <stdio.h>
#include "example_pkt.h"
#include <polysat/cmd-pkt.h>

static ProcessData *gProc = NULL;

static void print_cmd_handler(struct ProcessData *proc,
      struct IPC_Command *cmd,
      struct sockaddr_in *fromAddr, void *arg, int fd);

struct XDR_CommandHandlers handlers[] = {
   { IPC_EXAMPLE_CMDS_PRINT, &print_cmd_handler, NULL},
};

static void print_cmd_handler(struct ProcessData *proc,
      struct IPC_Command *cmd,
      struct sockaddr_in *fromAddr, void *arg, int fd)
{
   struct IPC_EXAMPLE_PrintParams *params =
      (struct IPC_EXAMPLE_PrintParams*)cmd->parameters.data;

   if (params->number % 2)
      IPC_error(proc, cmd, IPC_EXAMPLE_ERRS_BAD_NUM, fromAddr);
   else {
      printf("command data: %u %s\n", params->number, params->str);
      IPC_response(proc, cmd, IPC_TYPES_VOID, NULL, fromAddr);
   }
}

// Function called to generate a status structure
static void status_data_populator(void *arg, XDR_tx_struct cb,
      void *cb_args)
{
   struct IPC_EXAMPLE_Status resp;
   static int count = 0;

   resp.count = ++count;

   cb(&resp, cb_args);
}

// Simple SIGINT handler for cleanup
static int sigint_handler(int signum, void *arg)
{
   EVT_exit_loop(arg);
   return EVENT_KEEP;
}

int usage(const char *name)
{
   printf("Usage: %s\n"
          ""
          , name);

   return 0;
}

// Entry point
int main(int argc, char *argv[])
{
   XDR_register_populator(&status_data_populator, NULL,
         IPC_EXAMPLE_TYPES_STATUS);
   // CMD_set_xdr_cmd_handler(IPC_EXAMPLE_CMDS_PRINT, &print_cmd_handler, NULL);

   // Initialize the process
   gProc = PROC_init_xdr("payload", WD_DISABLED, handlers);
   DBG_setLevel(DBG_LEVEL_ALL);

   // Add a signal handler call back for SIGINT signal
   PROC_signal(gProc, SIGINT, &sigint_handler, PROC_evt(gProc));

   // Enter the main event loop
   EVT_start_loop(PROC_evt(gProc));

   // Clean up, whenever we exit event loop
   DBG_print(DBG_LEVEL_INFO, "Cleaning up\n");

   PROC_cleanup(gProc);

   return 0;
}


/**************************************************************************//**
 * Program Title: squirm - mouse jiggler
 * Date: 2024-12-21
 * Author: Ethan Tauriainen
 * Tested on: OpenSUSE Leap, 6.4.0-150600.23.30-default, x86_64 GNU/Linux 
 ******************************************************************************/

#include <argp.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

bool volatile run = 1;
const char *argp_program_version = "squirm 1.0";
const char *argp_program_bug_address = "<taurethan@gmail.com>";
static char doc[] = "squirm -- a program that periodically jiggles the mouse cursor";

static struct argp_option options[] = {
  {"interval", 'i', "TIME", 0,
   "Jiggle mouse left and right every TIME seconds"},
  { 0 }
};

struct arguments
{
  int interval;
};

void
handler (int signo)
{
  /* Currently only handles SIGINT as the user's only means of
     stopping the program; will potentially handle other signals
     down the road */
  run = 0;
}

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'i':
      arguments->interval = atoi(arg);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

void
emit (int fd, int type, int code, int val)
{
  struct input_event ie;

  ie.type = type;
  ie.code = code;
  ie.value = val;
  /* timestamp values below are ignored */
  ie.time.tv_sec = 0;
  ie.time.tv_usec = 0;

  write (fd, &ie, sizeof (ie));
}

void
jiggle (int x, int y, int distance, int fd)
{
  while (distance--)
    {
      emit (fd, EV_REL, REL_X, x);
      emit (fd, EV_REL, REL_Y, y);
      emit (fd, EV_SYN, SYN_REPORT, 0);
      usleep (15000);
    }
}

static struct argp argp = { options, parse_opt, 0, doc };

int
main (int argc, char **argv)
{
  struct uinput_setup usetup;
  struct arguments arguments;

  int fd = open ("/dev/uinput", O_WRONLY | O_NONBLOCK);

  if (fd < 0)
    {
      fprintf (stderr, RED "Root privileges are required to run this command.\n" RESET);
      exit (EXIT_FAILURE);
    }

  /* Defaults */
  arguments.interval = 5;

  ioctl (fd, UI_SET_EVBIT, EV_KEY);
  ioctl (fd, UI_SET_KEYBIT, BTN_LEFT);
  ioctl (fd, UI_SET_EVBIT, EV_REL);
  ioctl (fd, UI_SET_RELBIT, REL_X);
  ioctl (fd, UI_SET_RELBIT, REL_Y);

  memset (&usetup, 0, sizeof (usetup));
  strcpy (usetup.name, "Squirm device");

  ioctl (fd, UI_DEV_SETUP, &usetup);
  ioctl (fd, UI_DEV_CREATE);

  signal (SIGINT, handler);
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  if (arguments.interval < 1)
    {
      fprintf (stderr, RED "Invalid interval.\n" RESET);
      exit (EXIT_FAILURE);
    }

  while (run)
    {
      /* Jiggle upwards to the left */
      jiggle (-5, -5, 50, fd);
      sleep (arguments.interval);

      /* Jiggle downwards to the right */
      jiggle (5, 5, 50, fd);
      sleep (arguments.interval);
    }

  sleep (1);

  ioctl (fd, UI_DEV_DESTROY);
  close (fd);

  printf (GREEN "\nJiggle success.\n" RESET);
  exit (EXIT_SUCCESS);
}

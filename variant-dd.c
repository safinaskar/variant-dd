#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>

#include <libsh.h>

int
main (int argc, char *argv[])
{
  sh_init (argv[0]);

  if (argc != 3)
    {
      sh_x_fprintf (stderr, "Usage: %s [splice|sendfile|pipe|read-write] BLOCK-SIZE < IN > OUT\n", argv[0]);
      exit (EXIT_FAILURE);
    }

  size_t block_size = sh_umax2size_t (sh_xx_strtoumax (argv[2], 0));

  int p[2];
  char *buf;

  if (strcmp (argv[1], "pipe") == 0)
    {
      sh_x_pipe (p);
    }
  else if (strcmp (argv[1], "read-write") == 0)
    {
      buf = (char *) sh_x_malloc (block_size);
    }

  for (;;)
    {
      if (strcmp (argv[1], "splice") == 0)
        {
          if (sh_x_splice (0, NULL, 1, NULL, block_size, SPLICE_F_MOVE | SPLICE_F_MORE) == 0)
            {
              break;
            }
        }
      else if (strcmp (argv[1], "sendfile") == 0)
        {
          if (sh_x_sendfile (1, 0, NULL, block_size) == 0)
            {
              break;
            }
        }
      else if (strcmp (argv[1], "pipe") == 0)
        {
          size_t written = sh_x_splice (0, NULL, p[1], NULL, block_size, SPLICE_F_MOVE | SPLICE_F_MORE);

          if (written == 0)
            {
              break;
            }

          if (sh_x_splice (p[0], NULL, 1, NULL, written, SPLICE_F_MOVE | SPLICE_F_MORE) != written)
            {
              sh_throwx ("second splice wrote partially, I don't want to handle this case");
            }
        }
      else if (strcmp (argv[1], "read-write") == 0)
        {
          size_t got = sh_x_read (0, buf, block_size);

          if (got == 0)
            {
              break;
            }

          sh_repeat_write (1, buf, got);
        }
      else
        {
          sh_throwx ("wrong variant");
        }
    }
}

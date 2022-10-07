#include <stdio.h>
#include <string.h>

void rvl_test_info ();
void rvl_test_data ();
void rvl_test_write_TEXT ();
void rvl_test_read_TEXT ();

int
main (int argc, char *argv[])
{
  if (strcmp (argv[1], "1") == 0)
    {
      rvl_test_info ();
    }
  else if (strcmp (argv[1], "2") == 0)
    {
      rvl_test_data ();
    }
  else if (strcmp (argv[1], "3") == 0)
    {
      rvl_test_write_TEXT ();
    }
  else if (strcmp (argv[1], "4") == 0)
    {
      rvl_test_read_TEXT ();
    }

  return 0;
}

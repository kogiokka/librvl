#include <stdio.h>
#include <string.h>

void rvl_test_write_info();

int main(int argc, char* argv[])
{
    printf("Hello?");
    if (strcmp(argv[1], "1") == 0) {
        rvl_test_write_info();
    }

    return 0;
}

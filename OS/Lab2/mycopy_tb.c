#include <unistd.h>
#include <sys/syscall.h>

int main(void) {
    syscall(548, "src", "dst");
    return 0;
}


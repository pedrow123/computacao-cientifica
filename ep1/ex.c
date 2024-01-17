#include <fenv.h>
#include <math.h>
#include <stdio.h>

int main() {
    float num = 0.001;
    fesetround(FE_DOWNWARD);
    printf("%1.10e\n", roundf(num));
}

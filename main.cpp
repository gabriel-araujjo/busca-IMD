// trata os comandos do usuário

#include <cpuid.h>
#include <stdio.h>

int main(int argc, char ** argv) {
    unsigned int eax, ebx, ecx, edx;
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);
    if (ecx & bit_SSE4_2)
        printf ("SSE4.2 is supported\n");
    return 0;
}
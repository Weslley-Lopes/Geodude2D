#include "src/SolidDomain.h"
#include "src/Gnuplot.h"

int main(int argc, char **args)
{
    PetscInitialize(&argc, &args, (char *)0, NULL);

    #include "examples/generalized-alfa-solid/cantilever_beam_dissertacao.h"

    PetscFinalize();
    return 0;
}
// teste brache
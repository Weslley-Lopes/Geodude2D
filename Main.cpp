#include "src/SolidDomain.h"
#include "src/Gnuplot.h"

int main(int argc, char **args)
{
    PetscInitialize(&argc, &args, (char *)0, NULL);

    #include "examples/cantilever_beam.h"

    PetscFinalize();
    return 0;
}
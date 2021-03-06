/** This file exists solely to prevent code coverage analysis from including the test code.  */
#include <CodeCoverage\CodeCoverage.h>
// Exclusions must be compiled as unmanaged (native):
#pragma managed(push, off)

ExcludeFromCodeCoverage(Exclusion1, L"testing::*");
ExcludeFromCodeCoverage(Exclusion2, L"testing_internal::*");
ExcludeSourceFromCodeCoverage(Exclusion3, L"*\\unittest.*.cc");
ExcludeSourceFromCodeCoverage(Exclusion4, L"*\\systemtest.*.cc");
// Exclude Lua from coverage
ExcludeSourceFromCodeCoverage(Exclusion5, L"*\\l*.c");

// After setting exclusions, restore the previous managed/unmanaged state:
#pragma managed(pop)

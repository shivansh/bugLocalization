#include <iostream>
#include "utils.h"

int
main()
{
    std::vector<std::string> input_vars = {"a", "b", "c"};
    utils::GeneratePredicates(input_vars, "test_predicates.py");
    return 0;
}

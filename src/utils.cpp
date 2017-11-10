#include <fstream>
#include <string>
#include "utils.h"

void
utils::GeneratePredicates(std::vector<std::string> input_vars,
                          std::string predicate_file)
{
    std::ofstream test_fstream;
    int counter = 0;
    test_fstream.open(predicate_file, std::ios::out);

    test_fstream << "true_predicates = [0] * "
                  + std::to_string(input_vars.size()) + "\n"
                 << "false_predicates = [0] * "
                  + std::to_string(input_vars.size()) + "\n\n";

    for (int i = 0; i < input_vars.size(); i++) {
        for (int j = i+1; j < input_vars.size(); j++) {
            test_fstream << "if "
                          + input_vars[i] + " < " + input_vars[j] + ":\n"
                          + "\ttrue_predicates[" + std::to_string(counter) + "] += 1\n"
                          + "else:\n"
                          + "\tfalse_predicates[" + std::to_string(counter) + "] += 1\n";
            counter++;
        }
    }

    test_fstream.close();
}

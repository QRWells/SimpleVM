#include <vector>

#include "Instruction.h"

using namespace svm;

const std::vector<svm::Instruction> FACTORIAL = {
    {Operator::JMP, 11},
    // Fac
    {Operator::DUP},
    {Operator::PUSH, 1},
    {Operator::OPERATION, Func::SUB},
    {Operator::DUP},
    {Operator::BRANCH, Func::BNZ, 3},
    {Operator::POP},
    {Operator::RET},
    {Operator::CALL, 1},
    {Operator::OPERATION, Func::MUL},
    {Operator::RET},
    // Main
    {Operator::PUSH, 10},
    {Operator::CALL, 1},
    {Operator::OUT},
    {Operator::HALT},
};
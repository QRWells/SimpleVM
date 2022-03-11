
#include "Instruction.h"
#include "Machine.h"
#include "TestCase.h"
namespace svm::test {
const ::svm::TestCase FAC{

    //
    .Name = "Factorial",
    //
    .AsmCode =
        R"(
    jmp     main // jump to main
Fac:
    dup
    push    1
    sub
    dup
    bnz     next
    pop
    ret
next:
    call    Fac
    mul
    ret
main:
    push    5
    call    Fac
    out
    halt
)",
    .MachineCode = {
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
        {Operator::PUSH, 5},
        {Operator::CALL, 1},
        {Operator::OUT},
        {Operator::HALT},
    }};
} // namespace svm::test
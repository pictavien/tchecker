/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include <limits>
#include <iomanip>
#include <vector>

#include "tchecker/vm/vm.hh"

namespace tchecker {
  
  /* bytecode_t */
  
  
  std::ostream & output(std::ostream & os, tchecker::bytecode_t const * bytecode)
  {
    tchecker::bytecode_t const * base = bytecode;
    bool stop = false;
    while (!stop) {
      stop = (*bytecode == VM_RET);
      auto fl = os.flags();
      os.setf(std::ios::right);
      os << std::setw(4) << std::setfill('0') << (bytecode - base) << "\t";
      os.flags(fl);
      os << std::setw(0);
      bytecode += output_instruction (os, bytecode);

    }
    return os;
  }
  
  size_t output_instruction(std::ostream & os, tchecker::bytecode_t const * bytecode)
  {
    size_t res = 1;

    switch (*bytecode) {
      case VM_RET:
        os << "RET";
        break;
      case VM_RETZ:
        os << "RETZ";
        break;
      case VM_FAILNOTIN: os << "FAILNOTIN";
        res += 2;
        break;

      case VM_JMP:
        os << "JMP";
        res++;
        break;

      case VM_JMPZ:
        os << "JMPZ";
        res++;
        break;

      case VM_PUSH:
        os << "PUSH";
        res++;
        break;

      case VM_VALUEAT:
        os << "VALUEAT";
        break;

      case VM_ASSIGN:
        os << "ASSIGN";
        break;

      case VM_LOR:
        os << "LAND";
        break;

      case VM_LAND:
        os << "LAND";
        break;

      case VM_MINUS:
        os << "MINUS";
        break;

      case VM_DIV:
        os << "DIV";
        break;

      case VM_EQ:
        os << "EQ";
        break;

      case VM_GE:
        os << "GE";
        break;

      case VM_GT:
        os << "GT";
        break;

      case VM_LT:
        os << "LT";
        break;

      case VM_LE:
        os << "LE";
        break;

      case VM_MUL:
        os << "MUL";
        break;

      case VM_MOD:
        os << "MOD";
        break;

      case VM_NE:
        os << "NE";
        break;

      case VM_SUM:
        os << "SUM";
        break;

      case VM_NEG:
        os << "NEG";
        break;

      case VM_LNOT:
        os << "LNOT";
        break;

      case VM_CLKCONSTR:
        os << "CLKCONSTR";
        res++;
        break;

      case VM_CLKRESET:
        os << "CLKRESET";
        break;

      case VM_NOP:
        os << "NOP";
        break;

      case VM_PUSH_FRAME:
        os << "PUSH_FRAME";
        break;

      case VM_POP_FRAME:
        os << "POP_FRAME";
        break;

      case VM_VALUEAT_FRAME:
        os << "VALUEAT_FRAME";
        break;

      case VM_INIT_FRAME:
        os << "INIT_FRAME";
        break;

      case VM_ASSIGN_FRAME:
        os << "ASSIGN_FRAME";
        break;

      case VM_LOCATION_ID:
        os << "LOCATION_ID";
        res += 2;
        break;

      case VM_LOCATION_LABEL:
        os << "LOCATION_LABEL";
        res += 2;
        break;

      case VM_EVENT:
        os << "EVENT";
        res += 2;
        break;

      case VM_INC:
        os << "INC";
        break;

      case VM_DUP:
        os << "DUP";
        break;

      case VM_DUP2:
        os << "DUP2";
        break;

      case VM_ASSIGN_Q:
        os << "ASSIGN_Q";
        res++;
        break;

      case VM_VALUEAT_Q:
        os << "VALUEAT_Q";
        res++;
        break;

      default:
        throw std::runtime_error("bytecode output: incomplete switch statement");
    }

    for(int i = 1; i < res; i++)
      os << " " << bytecode[i];
    os << std::endl;

    return res;
  }

} // end of namespace tchecker

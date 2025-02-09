// frontend_flag_spec_leaky.h

#ifndef FRONTEND_FLAG_SPEC_H
#define FRONTEND_FLAG_SPEC_H

#include "_build/cpp/id_kind_asdl.h"
#include "_build/cpp/runtime_asdl.h"
#include "mycpp/mylib_leaky.h"

// Forward declarations (can't include osh_eval.h)
namespace args {
class _Action;
class _Attributes;
class Reader;
};  // namespace args

//
// Types for compile-time FlagSpec
//

union Val_c {
  bool b;
  int i;
  float f;
  const char* s;
};

struct DefaultPair_c {
  const char* name;
  runtime_asdl::flag_type_t typ;
  Val_c val;
};

// all concrete subtypes of args::_Action
enum class ActionType_c {
  SetToString,    // name, valid
  SetToString_q,  // hack for quit_parsing_flags

  SetToInt,         // name
  SetToFloat,       // name
  SetToTrue,        // name
  SetAttachedBool,  // name, for OilFlags

  SetOption,             // name
  SetNamedOption,        // no args, valid
  SetNamedOption_shopt,  // no args, valid
  SetAction,             // name
  SetNamedAction,        // no args, valid
};

// TODO: Figure out the difference between name and key
// key = '--ast-format'
// name = 'ast-format'
// out.Set('ast-format', ...)
// So I want to compress these two

struct Action_c {
  const char* key;
  ActionType_c type;
  const char* name;
  // for --ast-format, SetNamedAction(), SetNamedOption()
  const char** strs;
};

struct FlagSpec_c {
  const char* name;         // e.g. 'wait'
  const char** arity0;      // NULL terminated array
  Action_c* arity1;         // NULL terminated array
  Action_c* actions_long;   // NULL terminated array
  const char** plus_flags;  // NULL terminated array
  DefaultPair_c* defaults;
};

struct FlagSpecAndMore_c {
  const char* name;  // e.g. 'osh'
  // These are Dict[str, _Action]
  Action_c* actions_short;
  Action_c* actions_long;
  const char** plus_flags;  // NULL terminated array
  DefaultPair_c* defaults;
};

namespace flag_spec {

class _FlagSpec {
 public:
  List<Str*>* arity0;
  Dict<Str*, args::_Action*>* arity1;
  List<Str*>* plus_flags;
  Dict<Str*, args::_Action*>* actions_long;
  Dict<Str*, runtime_asdl::value_t*>* defaults;
};

class _FlagSpecAndMore {
 public:
  Dict<Str*, args::_Action*>* actions_long;
  Dict<Str*, args::_Action*>* actions_short;
  List<Str*>* plus_flags;
  Dict<Str*, runtime_asdl::value_t*>* defaults;
};

class _OilFlagSpec {
 public:
  Dict<Str*, args::_Action*>* arity1;
  Dict<Str*, runtime_asdl::value_t*>* defaults;
};

// for testing only
flag_spec::_FlagSpec* LookupFlagSpec(Str* spec_name);

args::_Attributes* Parse(Str* spec_name, args::Reader* arg_r);

Tuple2<args::_Attributes*, args::Reader*> ParseCmdVal(
    Str* spec_name, runtime_asdl::cmd_value__Argv* cmd_val);

// With optional arg
Tuple2<args::_Attributes*, args::Reader*> ParseCmdVal(
    Str* spec_name, runtime_asdl::cmd_value__Argv* cmd_val,
    bool accept_typed_args);

Tuple2<args::_Attributes*, args::Reader*> ParseLikeEcho(
    Str* spec_name, runtime_asdl::cmd_value__Argv* cmd_val);

args::_Attributes* ParseMore(Str* spec_name, args::Reader* arg_r);

Tuple2<args::_Attributes*, args::Reader*> ParseOilCmdVal(
    Str* spec_name, runtime_asdl::cmd_value__Argv* cmd_val);

}  // namespace flag_spec

#endif  // FRONTEND_FLAG_SPEC_H

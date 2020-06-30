// Mini C code generator for the JVM by Robert van Engelen
// A simple one-pass, syntax-directed translation of mini C to JVM bytecode

#ifndef MINIC_HPP
#define MINIC_HPP

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <string>
#include <set>
#include <vector>

// JVM computational types

typedef uint8_t     U1; // JVM byte
typedef uint16_t    U2; // JVM short
typedef uint32_t    U4; // JVM int
typedef uint64_t    U8; // JVM long
typedef float       F4; // JVM float
typedef double      F8; // JVM double
typedef const char *CS; // JVM string

// JVM opcodes

const U1 nop             = 0x00;  
const U1 aconst_null     = 0x01;  
const U1 iconst_m1       = 0x02;  
const U1 iconst_0        = 0x03;  
const U1 iconst_1        = 0x04;  
const U1 iconst_2        = 0x05;  
const U1 iconst_3        = 0x06;  
const U1 iconst_4        = 0x07;  
const U1 iconst_5        = 0x08;  
const U1 lconst_0        = 0x09;  
const U1 lconst_1        = 0x0a;  
const U1 fconst_0        = 0x0b;  
const U1 fconst_1        = 0x0c;  
const U1 fconst_2        = 0x0d;  
const U1 dconst_0        = 0x0e;  
const U1 dconst_1        = 0x0f;  
const U1 bipush          = 0x10;  
const U1 sipush          = 0x11;  
const U1 ldc             = 0x12;  
const U1 ldc_w           = 0x13;  
const U1 ldc2_w          = 0x14;  
const U1 iload           = 0x15;  
const U1 lload           = 0x16;  
const U1 fload           = 0x17;  
const U1 dload           = 0x18;  
const U1 aload           = 0x19;  
const U1 iload_0         = 0x1a;  
const U1 iload_1         = 0x1b;  
const U1 iload_2         = 0x1c;  
const U1 iload_3         = 0x1d;  
const U1 lload_0         = 0x1e;  
const U1 lload_1         = 0x1f;  
const U1 lload_2         = 0x20;  
const U1 lload_3         = 0x21;  
const U1 fload_0         = 0x22;  
const U1 fload_1         = 0x23;  
const U1 fload_2         = 0x24;  
const U1 fload_3         = 0x25;  
const U1 dload_0         = 0x26;  
const U1 dload_1         = 0x27;  
const U1 dload_2         = 0x28;  
const U1 dload_3         = 0x29;  
const U1 aload_0         = 0x2a;  
const U1 aload_1         = 0x2b;  
const U1 aload_2         = 0x2c;  
const U1 aload_3         = 0x2d;  
const U1 iaload          = 0x2e;  
const U1 laload          = 0x2f;  
const U1 faload          = 0x30;  
const U1 daload          = 0x31;  
const U1 aaload          = 0x32;  
const U1 baload          = 0x33;  
const U1 caload          = 0x34;  
const U1 saload          = 0x35;  
const U1 istore          = 0x36;  
const U1 lstore          = 0x37;  
const U1 fstore          = 0x38;  
const U1 dstore          = 0x39;  
const U1 astore          = 0x3a;  
const U1 istore_0        = 0x3b;  
const U1 istore_1        = 0x3c;  
const U1 istore_2        = 0x3d;  
const U1 istore_3        = 0x3e;  
const U1 lstore_0        = 0x3f;  
const U1 lstore_1        = 0x40;  
const U1 lstore_2        = 0x41;  
const U1 lstore_3        = 0x42;  
const U1 fstore_0        = 0x43;  
const U1 fstore_1        = 0x44;  
const U1 fstore_2        = 0x45;  
const U1 fstore_3        = 0x46;  
const U1 dstore_0        = 0x47;  
const U1 dstore_1        = 0x48;  
const U1 dstore_2        = 0x49;  
const U1 dstore_3        = 0x4a;  
const U1 astore_0        = 0x4b;  
const U1 astore_1        = 0x4c;  
const U1 astore_2        = 0x4d;  
const U1 astore_3        = 0x4e;  
const U1 iastore         = 0x4f;  
const U1 lastore         = 0x50;  
const U1 fastore         = 0x51;  
const U1 dastore         = 0x52;  
const U1 aastore         = 0x53;  
const U1 bastore         = 0x54;  
const U1 castore         = 0x55;  
const U1 sastore         = 0x56;  
const U1 pop             = 0x57;  
const U1 pop2            = 0x58;  
const U1 dup             = 0x59;  
const U1 dup_x1          = 0x5a;  
const U1 dup_x2          = 0x5b;  
const U1 dup2            = 0x5c;  
const U1 dup2_x1         = 0x5d;  
const U1 dup2_x2         = 0x5e;  
const U1 swap            = 0x5f;  
const U1 iadd            = 0x60;  
const U1 ladd            = 0x61;  
const U1 fadd            = 0x62;  
const U1 dadd            = 0x63;  
const U1 isub            = 0x64;  
const U1 lsub            = 0x65;  
const U1 fsub            = 0x66;  
const U1 dsub            = 0x67;  
const U1 imul            = 0x68;  
const U1 lmul            = 0x69;  
const U1 fmul            = 0x6a;  
const U1 dmul            = 0x6b;  
const U1 idiv            = 0x6c;  
const U1 ldiv_           = 0x6d;  
const U1 fdiv            = 0x6e;  
const U1 ddiv            = 0x6f;  
const U1 irem            = 0x70;  
const U1 lrem            = 0x71;  
const U1 frem            = 0x72;  
const U1 drem            = 0x73;  
const U1 ineg            = 0x74;
const U1 lneg            = 0x75;  
const U1 fneg            = 0x76;  
const U1 dneg            = 0x77;  
const U1 ishl            = 0x78;  
const U1 lshl            = 0x79;  
const U1 ishr            = 0x7a;  
const U1 lshr            = 0x7b;  
const U1 iushr           = 0x7c;  
const U1 lushr           = 0x7d;  
const U1 iand            = 0x7e;  
const U1 land            = 0x7f;  
const U1 ior             = 0x80;  
const U1 lor             = 0x81;  
const U1 ixor            = 0x82;  
const U1 lxor            = 0x83;  
const U1 iinc            = 0x84;  
const U1 i2l             = 0x85;  
const U1 i2f             = 0x86;  
const U1 i2d             = 0x87;  
const U1 l2i             = 0x88;  
const U1 l2f             = 0x89;  
const U1 l2d             = 0x8a;  
const U1 f2i             = 0x8b;  
const U1 f2l             = 0x8c;  
const U1 f2d             = 0x8d;  
const U1 d2i             = 0x8e;  
const U1 d2l             = 0x8f;  
const U1 d2f             = 0x90;  
const U1 i2b             = 0x91;  
const U1 i2c             = 0x92;  
const U1 i2s             = 0x93;  
const U1 lcmp            = 0x94;  
const U1 fcmpl           = 0x95;  
const U1 fcmpg           = 0x96;  
const U1 dcmpl           = 0x97;  
const U1 dcmpg           = 0x98;  
const U1 ifeq            = 0x99;  
const U1 ifne            = 0x9a;  
const U1 iflt            = 0x9b;  
const U1 ifge            = 0x9c;  
const U1 ifgt            = 0x9d;  
const U1 ifle            = 0x9e;  
const U1 if_icmpeq       = 0x9f;  
const U1 if_icmpne       = 0xa0;  
const U1 if_icmplt       = 0xa1;  
const U1 if_icmpge       = 0xa2;  
const U1 if_icmpgt       = 0xa3;  
const U1 if_icmple       = 0xa4;  
const U1 if_acmpeq       = 0xa5;  
const U1 if_acmpne       = 0xa6;  
const U1 goto_           = 0xa7;  
const U1 jsr             = 0xa8;  
const U1 ret             = 0xa9;  
const U1 tableswitch     = 0xaa;  
const U1 lookupswitch    = 0xab;  
const U1 ireturn         = 0xac;  
const U1 lreturn         = 0xad;  
const U1 freturn         = 0xae;  
const U1 dreturn         = 0xaf;  
const U1 areturn         = 0xb0;  
const U1 return_         = 0xb1;  
const U1 getstatic       = 0xb2;  
const U1 putstatic       = 0xb3;  
const U1 getfield        = 0xb4;  
const U1 putfield        = 0xb5;  
const U1 invokevirtual   = 0xb6;  
const U1 invokespecial   = 0xb7;  
const U1 invokestatic    = 0xb8;  
const U1 invokeinterface = 0xb9;  
const U1 xxxunusedxxx1   = 0xba;  
const U1 new_            = 0xbb;  
const U1 newarray        = 0xbc;  
const U1 anewarray       = 0xbd;  
const U1 arraylength     = 0xbe;  
const U1 athrow          = 0xbf;  
const U1 checkcast       = 0xc0;  
const U1 instanceof      = 0xc1;  
const U1 monitorenter    = 0xc2;  
const U1 monitorexit     = 0xc3;  
const U1 wide            = 0xc4;  
const U1 multianewarray  = 0xc5;  
const U1 ifnull          = 0xc6;  
const U1 ifnonnull       = 0xc7;  
const U1 goto_w          = 0xc8;  
const U1 jsr_w           = 0xc9;  

// JVM atype operands

const U1 T_BOOLEAN       = 4;
const U1 T_CHAR          = 5;
const U1 T_FLOAT         = 6;
const U1 T_DOUBLE        = 7;
const U1 T_BYTE          = 8;
const U1 T_SHORT         = 9;
const U1 T_INT           = 10;
const U1 T_LONG          = 11;

// JVM access flags
typedef U2 AF;

const AF ACC_PUBLIC    = 0x0001; // declared public; may be accessed from outside its package
const AF ACC_PRIVATE   = 0x0002; // declared private; usable only within the defining class
const AF ACC_PROTECTED = 0x0004; // declared protected; may be accessed within subclasses
const AF ACC_STATIC    = 0x0008; // declared static
const AF ACC_FINAL     = 0x0010; // declared final; no subclasses allowed or no further assignment after initialization
const AF ACC_SUPER     = 0x0020; // treat superclass methods specially when invoked by the invokespecial instruction
const AF ACC_VOLATILE  = 0x0040; // declared volatile; cannot be cached
const AF ACC_TRANSIENT = 0x0080; // declared transient; not written or read by a persistent object manager
const AF ACC_INTERFACE = 0x0200; // is an interface, not a class
const AF ACC_ABSTRACT  = 0x0400; // declared abstract; may not be instantiated
const AF ACC_STRICT    = 0x0800; // declared strictfp; floating-point mode is FP-strict
const AF ACC_SYNTHETIC = 0x1000; // declared synthetic; not present in the source code

// identifiers are efficienty compared by their pointer to a string in the Scanner::symbols set
typedef const std::string *ID;

// JVM type descriptor, as a special case we use NULL to denote short-circuit boolean
typedef const char *TD;

class Table;

// entry in a Table
class Entry {

 public:

  Entry(ID id, TD type, U2 place, Table *table, bool proto = false)
    :
      id(id),
      type(type),
      place(place),
      table(table),
      proto(proto)
  { }

  ID     id;    // identifier
  TD     type;  // JVM type descriptor
  U2     place; // variables have a place that is the index of a local or a constant pool index
  Table *table; // the table to which this entry belongs
  bool   proto; // prototype of a function

};

// table hierarchy to map identifiers to their properties
class Table {

 public:

  Table(Table *parent = NULL)
    :
      parent(parent),
      scope(parent != NULL ? parent->scope + 1 : 0),
      locals(0)
      { }

  // enter a variable in the table with its type and local index or pool index
  bool enter(ID id, TD type, U2 place)
  {
    for (std::vector<Entry>::iterator i = entries.begin(); i != entries.end(); ++i)
      if (i->id == id)
        return false;

    entries.push_back(Entry(id, type, place, this));

    return true;
  }

  // enter a function in the table
  bool enter_func(ID id, TD type, bool proto)
  {
    for (std::vector<Entry>::iterator i = entries.begin(); i != entries.end(); ++i)
    {
      if (i->id == id)
      {
        if (strcmp(i->type, type) != 0)
          return false;

        if (proto)
          return true;

        if (!i->proto)
          return false;

        i->proto = false;
        return true;
      }
    }

    entries.push_back(Entry(id, type, 0, this, proto));

    return true;
  }

  // lookup an identifier in the table hierarchy
  Entry *lookup(ID id)
  {
    for (Table *table = this; table != NULL; table = table->parent)
      for (std::vector<Entry>::iterator i = table->entries.begin(); i != table->entries.end(); ++i)
        if (i->id == id)
          return &*i;

    return NULL;
  }

  Table             *parent;  // parent table defines the outer scope
  std::vector<Entry> entries; // entries in the table
  int                scope;   // scope depth, zero for outermost scope (no parent)
  U2                 locals;  // number of arguments and locals declared

};

// backpatch list to resolve jump instruction targets
// concept from "Compilers: Principles, Techniques, and Tools" by Aho, Sethi, and Ullman
class BackpatchList {

 public:

  BackpatchList(U4 addr)
    :
      addr(addr),
      next(NULL)
  { }

  U4             addr;
  BackpatchList *next;

};

// semantic value for types, see minic.y %type <Type>
class Type {

 public:

  Type(TD type = NULL)
    :
      type(type)
  { }

  TD type;
};

// semantic value for expressions, see minic.y %type <Expr>
class Expr : public Type {

 public:

  Expr(TD type = NULL)
    :
      Type(type),
      truelist(NULL),
      falselist(NULL)
  { }

  BackpatchList *truelist;
  BackpatchList *falselist;

};

// a library entry to store Java packages to invoke built-in methods
class Library {

 public:

  const char *name;    // library function name
  const char *package; // JVM package
  const char *method;  // JVM method name
  const char *virtype; // type to match if method is virtual otherwise NULL when static
  const char *type;    // JVM method type descriptor

};

// the compiler
class Compiler {

 public:

  Compiler(const std::string& name)
    :
      main(false),
      type(NULL),
      return_type(NULL),
      scope(-1),
      loop_break_nest(0),
      loop_continue_nest(0),
      cf(name)
  { }

  // library functions we can call, translated to JVM virtual and static method invocations
  const Library *library(ID id, TD types)
  {
    // static and virtual (virtype) methods may use types I, D, Ljava/lang/String, return also C and Z that are implicitly converted to I
    static const Library builtins[] = {
      // System
      { "exit",    "java/lang/System",  "exit",           NULL, "(I)V" },
      { "getenv",  "java/lang/System",  "getenv",         NULL, "(Ljava/lang/String;)Ljava/lang/String;" },
      // Integer
      { "bin",     "java/lang/Integer", "toBinaryString", NULL, "(I)Ljava/lang/String;"  },
      { "hex",     "java/lang/Integer", "toHexString",    NULL, "(I)Ljava/lang/String;"  },
      { "oct",     "java/lang/Integer", "toOctalString",  NULL, "(I)Ljava/lang/String;"  },
      { "strtoi",  "java/lang/Integer", "parseInt",       NULL, "(Ljava/lang/String;)I"  },
      { "strtoi",  "java/lang/Integer", "parseInt",       NULL, "(Ljava/lang/String;I)I" },
      // Double
      { "strtof",  "java/lang/Double",  "parseDouble",    NULL, "(Ljava/lang/String;)D" },
      // String
      { "at",      "java/lang/String",  "charAt",         "(Ljava/lang/String;I)C", "(I)C" },
      { "compare", "java/lang/String",  "compareTo",      "(Ljava/lang/String;Ljava/lang/String;)I", "(Ljava/lang/String;)I" },
      { "concat",  "java/lang/String",  "concat",         "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", "(Ljava/lang/String;)Ljava/lang/String;" },
      { "empty" ,  "java/lang/String",  "isEmpty",        "(Ljava/lang/String;)Z", "()Z" },
      { "find",    "java/lang/String",  "indexOf",        "(Ljava/lang/String;I)I", "(I)I" },
      { "find",    "java/lang/String",  "indexOf",        "(Ljava/lang/String;II)I", "(II)I" },
      { "find",    "java/lang/String",  "indexOf",        "(Ljava/lang/String;Ljava/lang/String;)I", "(Ljava/lang/String;)I" },
      { "find",    "java/lang/String",  "indexOf",        "(Ljava/lang/String;Ljava/lang/String;I)I", "(Ljava/lang/String;I)I" },
      { "length",  "java/lang/String",  "length",         "(Ljava/lang/String;)I", "()I" },
      { "lower",   "java/lang/String",  "toLowerCase",    "(Ljava/lang/String;)Ljava/lang/String;", "()Ljava/lang/String;" },
      { "matches", "java/lang/String",  "matches",        "(Ljava/lang/String;Ljava/lang/String;)Z", "(Ljava/lang/String;)Z" },
      { "rfind",   "java/lang/String",  "lastIndexOf",    "(Ljava/lang/String;I)I", "(I)I" },
      { "rfind",   "java/lang/String",  "lastIndexOf",    "(Ljava/lang/String;II)I", "(II)I" },
      { "rfind",   "java/lang/String",  "lastIndexOf",    "(Ljava/lang/String;Ljava/lang/String;)I", "(Ljava/lang/String;)I" },
      { "rfind",   "java/lang/String",  "lastIndexOf",    "(Ljava/lang/String;Ljava/lang/String;I)I", "(Ljava/lang/String;I)I" },
      { "substr",  "java/lang/String",  "substring",      "(Ljava/lang/String;I)Ljava/lang/String;", "(I)Ljava/lang/String;" },
      { "substr",  "java/lang/String",  "substring",      "(Ljava/lang/String;II)Ljava/lang/String;", "(II)Ljava/lang/String;" },
      { "upper",   "java/lang/String",  "toUpperCase",    "(Ljava/lang/String;)Ljava/lang/String;", "()Ljava/lang/String;" },
      { "trim",    "java/lang/String",  "trim",           "(Ljava/lang/String;)Ljava/lang/String;", "()Ljava/lang/String;" },
      { "str",     "java/lang/String",  "valueOf",        NULL, "(I)Ljava/lang/String;" },
      { "str",     "java/lang/String",  "valueOf",        NULL, "(D)Ljava/lang/String;" },
      // Math
      { "abs",     "java/lang/Math",    "abs",            NULL, "(I)I"  },
      { "abs",     "java/lang/Math",    "abs",            NULL, "(D)D"  },
      { "acos",    "java/lang/Math",    "acos",           NULL, "(D)D"  },
      { "asin",    "java/lang/Math",    "asin",           NULL, "(D)D"  },
      { "atan",    "java/lang/Math",    "atan",           NULL, "(D)D"  },
      { "atan",    "java/lang/Math",    "atan2",          NULL, "(DD)D" },
      { "ceil",    "java/lang/Math",    "ceil",           NULL, "(D)D"  },
      { "cos",     "java/lang/Math",    "cos",            NULL, "(D)D"  },
      { "cosh",    "java/lang/Math",    "cosh",           NULL, "(D)D"  },
      { "deg",     "java/lang/Math",    "toDegrees",      NULL, "(D)D"  },
      { "exp",     "java/lang/Math",    "exp",            NULL, "(D)D"  },
      { "floor",   "java/lang/Math",    "floor",          NULL, "(D)D"  },
      { "log",     "java/lang/Math",    "log",            NULL, "(D)D"  },
      { "log10",   "java/lang/Math",    "log10",          NULL, "(D)D"  },
      { "max",     "java/lang/Math",    "max",            NULL, "(DD)D" },
      { "max",     "java/lang/Math",    "max",            NULL, "(II)I" },
      { "min",     "java/lang/Math",    "min",            NULL, "(DD)D" },
      { "min",     "java/lang/Math",    "min",            NULL, "(II)I" },
      { "pow",     "java/lang/Math",    "pow",            NULL, "(DD)D" },
      { "rad",     "java/lang/Math",    "toRadians",      NULL, "(D)D"  },
      { "rnd",     "java/lang/Math",    "random",         NULL, "()D"   },
      { "sin",     "java/lang/Math",    "sin",            NULL, "(D)D"  },
      { "sinh",    "java/lang/Math",    "sinh",           NULL, "(D)D"  },
      { "sgn",     "java/lang/Math",    "signum",         NULL, "(D)D"  },
      { "sqrt",    "java/lang/Math",    "sqrt",           NULL, "(D)D"  },
      { "tan",     "java/lang/Math",    "tan",            NULL, "(D)D"  },
      { "tanh",    "java/lang/Math",    "tanh",           NULL, "(D)D"  },
      { NULL, NULL, NULL, NULL, NULL }
    };

    for (const Library *lib = builtins; lib->name != NULL; ++lib)
      if (strcmp(lib->name, id->c_str()) == 0 && type_check_args(types, lib->virtype ? lib->virtype : lib->type))
        return lib;

    return NULL;
  }

  // return a function type
  TD type_function(TD args, TD result)
  {
    return types.insert(std::string("(").append(args).append(")").append(result)).first->c_str();
  }

  // return two types (to construct a function type)
  TD type_concat(TD type1, TD type2)
  {
    return types.insert(std::string(type1).append(type2)).first->c_str();
  }

  // return an array type
  TD type_array(TD type)
  {
    return types.insert(std::string("[").append(type)).first->c_str();
  }

  // return a "no type" (e.g. function without arguments)
  TD type_none()
  {
    return "";
  }

  // return a void type
  TD type_void()
  {
    return "V";
  }

  // return a boolean type (computationally the same as "I")
  TD type_boolean()
  {
    return "Z";
  }

  // return a char type (computationally the same as "I")
  TD type_char()
  {
    return "C";
  }

  // return a byte type (computationally the same as "I")
  TD type_byte()
  {
    return "B";
  }

  // return a short type (computationally the same as "I")
  TD type_short()
  {
    return "S";
  }

  // return a int type
  TD type_int()
  {
    return "I";
  }

  // return a long type
  TD type_long()
  {
    return "J";
  }

  // return a float type
  TD type_float()
  {
    return "F";
  }

  // return a double type
  TD type_double()
  {
    return "D";
  }

  // return a string object type
  TD type_string()
  {
    return "Ljava/lang/String;";
  }

  // return the return type of a function or NULL
  TD type_return(TD type_func)
  {
    if (type_func != NULL)
    {
      const char *s = strrchr(type_func, ')');
      if (s != NULL)
        return s + 1;
    }
    return NULL;
  }

  // check types of the function arguments, which must match exactly
  bool type_check_args(TD type_actuals, TD type_func)
  {
    if (type_actuals == NULL || type_func == NULL)
      return false;

    const char *s = strrchr(type_func, ')');
    return s != NULL && s == type_func + strlen(type_actuals) + 1 && strncmp(type_actuals, type_func + 1, s - type_func - 1) == 0;
  }

  // return the element type of an array
  TD type_element(TD array_type)
  {
    if (array_type != NULL && *array_type == '[')
      return array_type + 1;

    return NULL;
  }

  // true if the type of an expression is short-circuit (i.e. logic, not a value on the stack)
  bool type_is_circuit(TD type)
  {
    return type == NULL;
  }

  // true if the type of an expression is void
  bool type_is_void(TD type)
  {
    return type != NULL && strcmp(type, "V") == 0;
  }

  // true if the type of an expression is boolean
  bool type_is_boolean(TD type)
  {
    return type != NULL && strcmp(type, "Z") == 0;
  }

  // true if the type of an expression is char
  bool type_is_char(TD type)
  {
    return type != NULL && strcmp(type, "C") == 0;
  }

  // true if the type of an expression is byte
  bool type_is_byte(TD type)
  {
    return type != NULL && strcmp(type, "B") == 0;
  }

  // true if the type of an expression is short
  bool type_is_short(TD type)
  {
    return type != NULL && strcmp(type, "S") == 0;
  }

  // true if the type of an expression is int
  bool type_is_int(TD type)
  {
    return type != NULL && strcmp(type, "I") == 0;
  }

  // true if the type of an expression is long
  bool type_is_long(TD type)
  {
    return type != NULL && strcmp(type, "J") == 0;
  }

  // true if the type of an expression is float
  bool type_is_float(TD type)
  {
    return type != NULL && strcmp(type, "F") == 0;
  }

  // true if the type of an expression is double
  bool type_is_double(TD type)
  {
    return type != NULL && strcmp(type, "D") == 0;
  }

  // true if the type of an expression is a string object
  bool type_is_string(TD type)
  {
    return type != NULL && strcmp(type, "Ljava/lang/String;") == 0;
  }

  // true if two types are equal
  bool type_equal(TD type1, TD type2)
  {
    return type1 == type2 || (type1 != NULL && type2 != NULL && strcmp(type1, type2) == 0);
  }

  // reset the emitter to generate code for a method, stored in a buffer
  void new_method_code()
  {
    code.clear();
  }

  // address of the next instruction emitted
  U4 addr()
  {
    return static_cast<U4>(code.size());
  }

  // add a UTF-8 formatted string to the ClassFile constant pool
  U2 pool_add_Utf8(CS s)
  {
    for (std::vector<ConstantPool>::iterator p = cf.pool.begin(); p != cf.pool.end(); ++p)
      if (p->t == CONSTANT_Utf8 && p->s == s)
        return static_cast<U2>(std::distance(cf.pool.begin(), p) + 1);

    cf.pool.push_back(ConstantPool(CONSTANT_Utf8, s));

    return static_cast<U2>(cf.pool.size());
  }

  // add a 32-bit unsigned integer to the ClassFile constant pool
  U2 pool_add_Integer(U4 i)
  {
    for (std::vector<ConstantPool>::iterator p = cf.pool.begin(); p != cf.pool.end(); ++p)
      if (p->t == CONSTANT_Integer && p->i == i)
        return static_cast<U2>(std::distance(cf.pool.begin(), p) + 1);

    cf.pool.push_back(ConstantPool(CONSTANT_Integer, i));

    return static_cast<U2>(cf.pool.size());
  }

  // add a 32-bit float to the ClassFile constant pool
  U2 pool_add_Float(F4 f)
  {
    for (std::vector<ConstantPool>::iterator p = cf.pool.begin(); p != cf.pool.end(); ++p)
      if (p->t == CONSTANT_Float && p->f == f)
        return static_cast<U2>(std::distance(cf.pool.begin(), p) + 1);

    cf.pool.push_back(ConstantPool(CONSTANT_Float, f));

    return static_cast<U2>(cf.pool.size());
  }

  // add a 64-bit unsigned integer to the ClassFile constant pool
  U2 pool_add_Long(U8 l)
  {
    for (std::vector<ConstantPool>::iterator p = cf.pool.begin(); p != cf.pool.end(); ++p)
      if (p->t == CONSTANT_Long && p->l == l)
        return static_cast<U2>(std::distance(cf.pool.begin(), p) + 1);

    cf.pool.push_back(ConstantPool(CONSTANT_Long, l));

    // Double requires one extra entry in the constant pool; use non-existent CONSTANT_Padding
    cf.pool.push_back(ConstantPool());

    return static_cast<U2>(cf.pool.size() - 1);
  }

  // add a 64-bit float to the ClassFile constant pool
  U2 pool_add_Double(F8 d)
  {
    for (std::vector<ConstantPool>::iterator p = cf.pool.begin(); p != cf.pool.end(); ++p)
      if (p->t == CONSTANT_Double && p->d == d)
        return static_cast<U2>(std::distance(cf.pool.begin(), p) + 1);

    cf.pool.push_back(ConstantPool(CONSTANT_Double, d));

    // Double requires one extra entry in the constant pool; use non-existent CONSTANT_Padding
    cf.pool.push_back(ConstantPool());

    return static_cast<U2>(cf.pool.size() - 1);
  }

  // Add a string to the ClassFile constant pool
  U2 pool_add_String(CS s)
  {
    U2 r = pool_add_Utf8(s);

    for (std::vector<ConstantPool>::iterator p = cf.pool.begin(); p != cf.pool.end(); ++p)
      if (p->t == CONSTANT_String && p->r == r)
        return static_cast<U2>(std::distance(cf.pool.begin(), p) + 1);

    cf.pool.push_back(ConstantPool(CONSTANT_String, r));

    return static_cast<U2>(cf.pool.size());
  }

  // add a class name to the ClassFile constant pool
  U2 pool_add_Class(CS name)
  {
    U2 r = pool_add_Utf8(name);

    for (std::vector<ConstantPool>::iterator p = cf.pool.begin(); p != cf.pool.end(); ++p)
      if (p->t == CONSTANT_Class && p->r == r)
        return static_cast<U2>(std::distance(cf.pool.begin(), p) + 1);

    cf.pool.push_back(ConstantPool(CONSTANT_Class, r));

    return static_cast<U2>(cf.pool.size());
  }

  // add a field for the current ClassFile to the ClassFile constant pool
  U2 pool_add_Field(CS field_name, CS type)
  {
    return pool_add_Field(cf.name.c_str(), field_name, type);
  }

  // add a field to the ClassFile constant pool
  U2 pool_add_Field(CS class_name, CS field_name, CS type)
  {
    U2 r1 = pool_add_Class(class_name);
    U2 r2 = pool_add_NameAndType(field_name, type);

    for (std::vector<ConstantPool>::iterator p = cf.pool.begin(); p != cf.pool.end(); ++p)
      if (p->t == CONSTANT_Field && p->p.first == r1 && p->p.second == r2)
        return static_cast<U2>(std::distance(cf.pool.begin(), p) + 1);

    cf.pool.push_back(ConstantPool(CONSTANT_Field, r1, r2));

    return static_cast<U2>(cf.pool.size());
  }

  // add a method for the current ClassFile to the ClassFile constant pool
  U2 pool_add_Method(CS method_name, CS type)
  {
    return pool_add_Method(cf.name.c_str(), method_name, type);
  }

  // add a method to the ClassFile constant pool
  U2 pool_add_Method(CS class_name, CS method_name, CS type)
  {
    U2 r1 = pool_add_Class(class_name);
    U2 r2 = pool_add_NameAndType(method_name, type);

    for (std::vector<ConstantPool>::iterator p = cf.pool.begin(); p != cf.pool.end(); ++p)
      if (p->t == CONSTANT_Method && p->p.first == r1 && p->p.second == r2)
        return static_cast<U2>(std::distance(cf.pool.begin(), p) + 1);

    cf.pool.push_back(ConstantPool(CONSTANT_Method, r1, r2));

    return static_cast<U2>(cf.pool.size());
  }

  // add a (name,type) pair to the ClassFile constant pool
  U2 pool_add_NameAndType(CS name, CS type)
  {
    U2 r1 = pool_add_Utf8(name);
    U2 r2 = pool_add_Utf8(type);

    for (std::vector<ConstantPool>::iterator p = cf.pool.begin(); p != cf.pool.end(); ++p)
      if (p->t == CONSTANT_NameAndType && p->p.first == r1 && p->p.second == r2)
        return static_cast<U2>(std::distance(cf.pool.begin(), p) + 1);

    cf.pool.push_back(ConstantPool(CONSTANT_NameAndType, r1, r2));

    return static_cast<U2>(cf.pool.size());
  }

  // add a new method to the ClassFile
  void new_method(AF access, ID name, TD type, U2 max_locals, U2 max_stack)
  {
    U2 r1 = pool_add_Utf8(name->c_str());
    U2 r2 = pool_add_Utf8(type);

    cf.methods.push_back(MethodInfo(access, r1, r2, max_locals, max_stack, code));
  }

  // add a new field to the ClassFile
  U2 new_field(AF access, ID name, TD type)
  {
    U2 r1 = pool_add_Utf8(name->c_str());
    U2 r2 = pool_add_Utf8(type);

    cf.fields.push_back(FieldInfo(access, r1, r2));

    return pool_add_Field(name->c_str(), type);
  }

  // append the second list to the first list, either one can be NULL
  BackpatchList *merge(BackpatchList *list1, BackpatchList *list2)
  {
    BackpatchList *end = NULL;

    for (BackpatchList *p = list1; p != NULL; p = p->next)
      end = p;

    if (end != NULL)
    {
      end->next = list2;
      return list1;
    }

    return list2;
  }

  // return new backpatch list for the next instruction address
  BackpatchList *backpatch_addr()
  {
    return new BackpatchList(addr());
  }

  // backpatch opcodes in the backpatch list
  void backpatch(BackpatchList *list, U4 addr)
  {
    while (list != NULL)
    {
      backpatch(list->addr, addr);
      BackpatchList *next = list->next;
      delete list;
      list = next;
    }
  }

  // backpatch opcodes in the backpatch list
  void backpatch(U4 inst, U4 addr)
  {
    U2 offset = static_cast<U2>(addr - inst); // should error if out of range?
    code[inst + 1] = H1(offset);
    code[inst + 2] = L1(offset);
  }

  // open loop scope
  void loop_break_init()
  {
    ++loop_break_nest;
    if (breaks.size() < loop_break_nest)
      breaks.push_back(NULL);
    else
      breaks[loop_break_nest - 1] = NULL;
  }

  // break statement in a loop
  bool loop_break()
  {
    if (loop_break_nest == 0)
      return false;
    breaks[loop_break_nest - 1] = merge(breaks[loop_break_nest - 1], backpatch_addr());
    emit3(goto_, 0);
    return true;
  }

  // open loop scope
  void loop_continue_init()
  {
    ++loop_continue_nest;
    if (continues.size() < loop_continue_nest)
      continues.push_back(NULL);
    else
      continues[loop_continue_nest - 1] = NULL;
  }

  // continue statement in a loop
  bool loop_continue()
  {
    if (loop_continue_nest == 0)
      return false;
    continues[loop_continue_nest - 1] = merge(continues[loop_continue_nest - 1], backpatch_addr());
    emit3(goto_, 0);
    return true;
  }

  // close loop scope and backpatch the break and continue statements in the loop if any
  void loop_done()
  {
    backpatch(breaks[--loop_break_nest], addr());
    backpatch(continues[--loop_continue_nest], addr());
  }

  // coerce value on top of JVM stack
  bool coerce(Expr& expr, TD type)
  {
    if (type_equal(expr.type, type))
      return true;

    TD conv = decircuit(expr);

    if (type_is_int(conv) && type_is_double(type))
      emit(i2d);
    else if (type_is_double(conv) && type_is_int(type))
      emit(d2i);
    else if (!type_equal(conv, type))
      return false;

    return true;;
  }

  // Coerce value under stack top value
  bool coerce_x1(Expr& expr, TD type)
  {
    if (type_equal(expr.type, type))
      return true;

    if (!type_is_circuit(expr.type))
    {
      if (type_is_double(expr.type))
      {
        emit(dup_x2); // v3 v2 v1 -> v1 v3 v2 v1
        emit(pop);    // v1 v3 v2 v1 -> v1 v3 v2
      }
      else
      {
        emit(swap);
      }
    }

    if (coerce(expr, type))
    {
      if (type_is_double(type))
      {
        emit(dup2_x1); // v3 v2 v1 -> v2 v1 v3 v2 v1
        emit(pop2);    // v2 v1 v3 v2 v1 -> v2 v1 v3
      }
      else
      {
        emit(swap);
      }

      return true;
    }

    return false;
  }

  // Coerce value under top two stack values
  bool coerce_x2(Expr& expr, TD type)
  {
    if (type_equal(expr.type, type))
      return true;

    if (!type_is_circuit(expr.type))
    {
      if (type_is_double(expr.type))
      {
        emit(dup2_x2); // v4 v3 v2 v1 -> v2 v1 v4 v3 v2 v1
        emit(pop2);    // v2 v1 v4 v3 v2 v1 -> v2 v1 v4 v3
      }
      else
      {
        emit(dup2_x1); // v3 v2 v1 -> v2 v1 v3 v2 v1
        emit(pop2);    // v2 v1 v3 v2 v1 -> v2 v1 v3
      }
    }

    if (coerce(expr, type))
    {
      if (type_is_double(type))
      {
        emit(dup2_x2); // v4 v3 v2 v1 -> v2 v1 v4 v3 v2 v1
        emit(pop2);    // v2 v1 v4 v3 v2 v1 -> v2 v1 v4 v3
      }
      else
      {
        emit(dup_x2); // v3 v2 v1 -> v1 v3 v2 v1
        emit(pop);    // v1 v3 v2 v1 -> v1 v3 v2
      }

      return true;
    }

    return false;
  }

  // convert any type to short-circuit logic, no change when already short circuit
  Expr circuit(Expr& expr, bool& ok)
  {
    ok = true;

    if (type_is_circuit(expr.type))
      return expr;

    Expr result;

    if (type_is_int(expr.type))
    {
      result.falselist = backpatch_addr();
      emit3(ifeq, 0);
      result.truelist = backpatch_addr();
      emit3(goto_, 0);
    }
    else if (type_is_double(expr.type))
    {
      emit(dconst_0);
      emit(dcmpl);
      result.falselist = backpatch_addr();
      emit3(ifeq, 0);
      result.truelist = backpatch_addr();
      emit3(goto_, 0);
    }
    else if (type_is_string(expr.type))
    {
      result.falselist = backpatch_addr();
      emit3(ifnull, 0);
      result.truelist = backpatch_addr();
      emit3(goto_, 0);
    }
    else
    {
      ok = false;
    }

    return result;
  }

  // convert short-circuit logic to push 0 or 1, no change when not short-circuit
  TD decircuit(Expr& expr)
  {
    if (!type_is_circuit(expr.type))
      return expr.type;

    if (expr.falselist != NULL)
    {
      backpatch(expr.falselist, addr());
      emit(iconst_0);
      expr.falselist = NULL;
      if (expr.truelist != NULL)
        emit3(goto_, 4);
    }
    if (expr.truelist != NULL)
    {
      backpatch(expr.truelist, addr());
      emit(iconst_1);
      expr.truelist = NULL;
    }

    return type_int();
  }

  // coerce and return the wider type (int or double) of two types
  TD widen(Expr& expr1, Expr& expr2, bool& ok)
  {
    TD type;

    if (type_is_double(expr1.type))
      type = expr1.type;
    else if (type_is_double(expr2.type))
      type = expr2.type;
    else
      type = type_int();

    ok = coerce(expr2, type);

    if (ok)
    {
      if (type_is_double(type))
        ok = coerce_x2(expr1, type);
      else
        ok = coerce_x1(expr1, type);
    }

    return type;
  }

  // emit ldc or ldc_w
  void emit_ldc(U2 index)
  {
    if (index <= 0xff)
      emit2(ldc, static_cast<U1>(index));
    else
      emit3(ldc_w, index);
  }

  // emit load
  void emit_load(TD type, U2 local, bool& ok)
  {
    ok = true;

    if (type_is_int(type))
    {
      switch (local)
      {
        case 0:  emit(iload_0); break;
        case 1:  emit(iload_1); break;
        case 2:  emit(iload_2); break;
        case 3:  emit(iload_3); break;
        default: emit2(iload, local);
      }
    }
    else if (type_is_double(type))
    {
      switch (local)
      {
        case 0:  emit(dload_0); break;
        case 1:  emit(dload_1); break;
        case 2:  emit(dload_2); break;
        case 3:  emit(dload_3); break;
        default: emit2(dload, local);
      }
    }
    else if (type_is_string(type))
    {
      switch (local)
      {
        case 0:  emit(aload_0); break;
        case 1:  emit(aload_1); break;
        case 2:  emit(aload_2); break;
        case 3:  emit(aload_3); break;
        default: emit2(aload, local);
      }
    }
    else
    {
      ok = false;
    }
  }

  // emit store
  void emit_store(TD type, U2 local, bool& ok)
  {
    if (type_is_int(type))
    {
      switch (local)
      {
        case 0:  emit(istore_0); break;
        case 1:  emit(istore_1); break;
        case 2:  emit(istore_2); break;
        case 3:  emit(istore_3); break;
        default: emit2(istore, local);
      }
    }
    else if (type_is_double(type))
    {
      switch (local)
      {
        case 0:  emit(dstore_0); break;
        case 1:  emit(dstore_1); break;
        case 2:  emit(dstore_2); break;
        case 3:  emit(dstore_3); break;
        default: emit2(dstore, local);
      }
    }
    else if (type_is_string(type))
    {
      switch (local)
      {
        case 0:  emit(astore_0); break;
        case 1:  emit(astore_1); break;
        case 2:  emit(astore_2); break;
        case 3:  emit(astore_3); break;
        default: emit2(astore, local);
      }
    }
    else
    {
      ok = false;
    }
  }

  // emit integer/double/string comparison relation
  Expr emit_rel(Expr& expr1, Expr& expr2, int if_op, int if_icmp_op, int d_op, bool eq, bool& ok)
  {
    Expr result;

    if (type_is_string(expr1.type) && type_is_string(expr2.type))
    {
      emit3(invokevirtual, pool_add_Method("java/lang/String", "compareTo", "(Ljava/lang/String;)I"));
      result.truelist = backpatch_addr();
      emit3(if_op, 0);
      result.falselist = backpatch_addr();
      emit3(goto_, 0);
    }
    else
    {
      TD type = widen(expr1, expr2, ok);

      if (type_is_int(type))
      {
        result.truelist = backpatch_addr();
        emit3(if_icmp_op, 0);
      }
      else if (type_is_double(type))
      {
        emit(d_op);
        if (d_op == dsub)
          emit(f2i);
        result.truelist = backpatch_addr();
        emit3(eq ? ifeq : ifne, 0);
      }

      result.falselist = backpatch_addr();
      emit3(goto_, 0);
    }

    return result;
  }

  // emit integer/double/string dyadic operation
  Expr emit_op(Expr& expr1, Expr& expr2, int i_op, int d_op, bool& ok)
  {
    ok = true;

    Expr result;

    if (type_is_string(expr1.type) && type_is_string(expr2.type) && i_op == iadd)
    {
      emit3(invokevirtual, pool_add_Method("java/lang/String", "concat", "(Ljava/lang/String;)Ljava/lang/String;"));

      result.type = type_string();
    }
    else
    {
      result.type = widen(expr1, expr2, ok);

      if (type_is_int(result.type))
        emit(i_op);
      else if (type_is_double(result.type) && d_op != nop)
        emit(d_op);
      else
        ok = false;
    }

    return result;
  }

  // emit integer/double/string (combined) assignment operation
  Expr emit_asg(Entry *entry, Expr& expr, int i_op, int d_op, bool& ok)
  {
    ok = true;

    Expr result;

    if (entry != NULL)
    {
      if (type_is_string(entry->type) && type_is_string(expr.type) && i_op == iadd)
      {
        emit3(invokevirtual, pool_add_Method("java/lang/String", "concat", "(Ljava/lang/String;)Ljava/lang/String;"));
        emit(dup);
        emit_store(entry->type, entry->place, ok);

        result.type = type_string();
      }
      else if (coerce(expr, entry->type))
      {
        result.type = entry->type;

        if (entry->table->scope == 0)
        {
          if (i_op != nop)
          {
            if (type_is_int(result.type))
              emit(i_op);
            else if (type_is_double(result.type) && d_op != nop)
              emit(d_op);
            else
              ok = false;
          }
          if (type_is_double(result.type))
            emit(dup2);
          else
            emit(dup);
          emit3(putstatic, entry->place);
        }
        else if (type_is_int(result.type))
        {
          if (i_op != nop)
            emit(i_op);
          emit(dup);
          emit_store(entry->type, entry->place, ok);
        }
        else if (type_is_double(result.type) && (i_op == nop || d_op != nop))
        {
          if (d_op != nop)
            emit(d_op);
          emit(dup2);
          emit_store(entry->type, entry->place, ok);
        }
        else if (type_is_string(result.type) && i_op == nop)
        {
          emit(dup);
          emit_store(entry->type, entry->place, ok);
        }
        else
        {
          ok = false;
        }
      }
      else
      {
        ok = false;
      }
    }

    return result;
  }
  
  // emit opcode
  void emit(U1 opcode)
  {
    code.push_back(opcode);
  }

  // emit opcode with single-byte operand
  void emit2(U1 opcode, U1 operand)
  {
    code.push_back(opcode);
    code.push_back(operand);
  }

  // emit opcode with double-byte operand
  void emit3(U1 opcode, U2 operand)
  {
    code.push_back(opcode);
    code.push_back(H1(operand));
    code.push_back(L1(operand));
  }

  // emit opcode with two single-byte operands
  void emit3(U1 opcode, U1 operand1, U1 operand2)
  {
    code.push_back(opcode);
    code.push_back(operand1);
    code.push_back(operand2);
  }

  // save the class file to file "name.class", "name" is the name of the source code file without extension suffix
  bool save()
  {
    FILE *fd = fopen(std::string(cf.name).append(".class").c_str(), "w");

    if (fd == NULL)
      return false;

    U2 attribute_code_ref = pool_add_Utf8("Code");
    U2 this_class_ref = pool_add_Class(cf.name.c_str());
    U2 super_class_ref = pool_add_Class("java/lang/Object");

    save_U4(fd, 0xcafebabe);      // magic number
    save_U2(fd, 0x0000);          // minor_version
    save_U2(fd, 0x002e);          // major_version
    save_pool(fd);
    save_U2(fd, cf.access);       // access_flags
    save_U2(fd, this_class_ref);  // this_class (index in pool refers to CONSTANT_Class)
    save_U2(fd, super_class_ref); // super_class (index in pool refers to CONSTANT_Class)
    save_interfaces(fd);
    save_fields(fd);
    save_methods(fd, attribute_code_ref);
    save_attributes(fd);

    int err = ferror(fd);

    fclose(fd);

    return err == 0;
  }

  bool                        main;        // if we are compiling the body of main()
  TD                          type;        // the last type declared
  TD                          return_type; // the return type of the function we are compiling
  Table                      *table[2];    // table[0] holds statics, table[1] holds locals
  int                         scope;       // 0 for static scope, 1 for local scope
  size_t                      loop_break_nest;    // depth of the loop
  size_t                      loop_continue_nest; // depth of the loop
  std::vector<BackpatchList*> breaks;      // break jumps to the instructions after the loop
  std::vector<BackpatchList*> continues;   // continue jumps to the loop condition expression
  std::set<std::string>       types;       // collection of TD strings

 private:

  // JVM bytecode is a sequence of unsigned bytes
  typedef std::basic_string<U1> Code;

  // each item in the ConstantPool must begin with a 1-byte tag ConstantType indicating the kind of entry
  enum ConstantType {
    CONSTANT_Padding         = 0, // a non-existent value to pad Long and Double entries
    CONSTANT_Utf8            = 1,
    CONSTANT_Integer         = 3,
    CONSTANT_Float           = 4,
    CONSTANT_Long            = 5,
    CONSTANT_Double          = 6,
    CONSTANT_Class           = 7,
    CONSTANT_String          = 8,
    CONSTANT_Field           = 9,
    CONSTANT_Method          = 10,
    CONSTANT_InterfaceMethod = 11,
    CONSTANT_NameAndType     = 12,
    CONSTANT_MethodHandle    = 15, // since Java SE 7; not used
    CONSTANT_MethodType      = 16, // since Java SE 7; not used
    CONSTANT_Dynamic         = 17, // since Java SE 11; not used
    CONSTANT_InvokeDynamic   = 18, // since Java SE 7; not used
    CONSTANT_Module          = 19, // since Java SE 9; not used
    CONSTANT_Package         = 20, // since Java SE 9; not used
  };

  // the constant pool may contain pairs of references
  typedef std::pair<U2,U2> Pair;

  // a class file constant pool element in simplified form
  struct ConstantPool {

    ConstantPool() : t(CONSTANT_Padding) { }

    ConstantPool(ConstantType t, CS s)         : t(t), s(s)      { }
    ConstantPool(ConstantType t, U4 i)         : t(t), i(i)      { }
    ConstantPool(ConstantType t, F4 f)         : t(t), f(f)      { }
    ConstantPool(ConstantType t, U8 l)         : t(t), l(l)      { }
    ConstantPool(ConstantType t, F8 d)         : t(t), d(d)      { }
    ConstantPool(ConstantType t, U2 r)         : t(t), r(r)      { }
    ConstantPool(ConstantType t, U2 r1, U2 r2) : t(t), p(r1, r2) { }

    ConstantType t; // tag

    U4   i; // unsigned int
    F4   f; // float
    U8   l; // long unsigned int
    F8   d; // double float
    CS   s; // string in UTF-8 format
    U2   r; // reference (constant pool index)
    Pair p; // pair of references

  };

  // class file field information
  struct FieldInfo {

    FieldInfo(AF access, U2 name, U2 type)
      :
        access(access),
        name(name),
        type(type)
    { }

    AF access;
    U2 name;
    U2 type;

  };

  // class file method information
  struct MethodInfo {

    MethodInfo(AF access, U2 name, U2 type, U2 max_locals, U2 max_stack, const Code& code)
      :
        access(access),
        name(name),
        type(type),
        max_locals(max_locals),
        max_stack(max_stack),
        code(code)
    { }

    AF   access;
    U2   name;
    U2   type;
    U2   max_locals;
    U2   max_stack;
    Code code;

  };

  // the class file in simplified form
  struct ClassFile {

    // set up a class file structure
    ClassFile(const std::string& name)
      :
        access(ACC_PUBLIC),
        name(name)
    { }

    AF                        access;
    std::string               name;
    std::vector<ConstantPool> pool;
    std::vector<FieldInfo>    fields;
    std::vector<MethodInfo>   methods;

  };

  // save the ConstantPool contents to the class file
  void save_pool(FILE *fd)
  {
    save_U2(fd, static_cast<U2>(cf.pool.size() + 1));

    for (std::vector<ConstantPool>::iterator p = cf.pool.begin(); p != cf.pool.end(); ++p)
    {
      switch (p->t)
      {
        case CONSTANT_Padding:
          break;
        case CONSTANT_Utf8:
          save_Utf8(fd, p->s);
          break;
        case CONSTANT_Integer:
          save_Integer(fd, p->i);
          break;
        case CONSTANT_Float:
          save_Float(fd, p->f);
          break;
        case CONSTANT_Long:
          save_Long(fd, p->l);
          break;
        case CONSTANT_Double:
          save_Double(fd, p->d);
          break;
        case CONSTANT_String:
          save_String(fd, p->r);
          break;
        case CONSTANT_Class:
          save_Class(fd, p->r);
          break;
        case CONSTANT_Field:
          save_Field(fd, p->p);
          break;
        case CONSTANT_Method:
          save_Method(fd, p->p);
          break;
        case CONSTANT_InterfaceMethod:
          save_InterfaceMethod(fd, p->p);
          break;
        case CONSTANT_NameAndType:
          save_NameAndType(fd, p->p);
          break;
        case CONSTANT_MethodHandle:
        case CONSTANT_MethodType:
        case CONSTANT_Dynamic:
        case CONSTANT_InvokeDynamic:
        case CONSTANT_Module:
        case CONSTANT_Package:
          // skipped; not used
          break;
      }
    }
  }

  void save_interfaces(FILE *fd)
  {
    save_U2(fd, 0); // interfaces_count
    // each (u2) value in the interfaces array must be a valid index into the pool table
  }

  void save_fields(FILE *fd)
  {
    U2 n = static_cast<U2>(cf.fields.size());

    save_U2(fd, n);

    for (U2 i = 0; i < n; i++)
    {
      save_U2(fd, cf.fields[i].access);
      save_U2(fd, cf.fields[i].name);
      save_U2(fd, cf.fields[i].type);
      save_U2(fd, 0); // no attributes
    }
  }

  void save_methods(FILE *fd, U2 attribute_code_ref)
  {
    U2 n = static_cast<U2>(cf.methods.size());

    save_U2(fd, n);

    for (U2 i = 0; i < n; i++)
    {
      save_U2(fd, cf.methods[i].access);
      save_U2(fd, cf.methods[i].name);
      save_U2(fd, cf.methods[i].type);

      if (cf.methods[i].code.size() > 0)
      {
        save_U2(fd, 1); // one attribute
        save_U2(fd, attribute_code_ref); // attribute "Code"
        save_U4(fd, static_cast<U4>(cf.methods[i].code.size() + 12)); // attribute length
        save_U2(fd, cf.methods[i].max_stack);
        save_U2(fd, cf.methods[i].max_locals);
        save_Code(fd, cf.methods[i].code);
        save_U2(fd, 0); // no exceptions
        save_U2(fd, 0); // no attributes
      }
      else
      {
        save_U2(fd, 0);
      }
    }
  }

  void save_attributes(FILE *fd)
  {
    save_U2(fd, 0); // attributes_count
    // each value of the attributes table must be an attribute structure
  }

  void save_Utf8(FILE *fd, CS s)
  {
    U2 n = static_cast<U2>(strlen(s));
    save_U1(fd, CONSTANT_Utf8);
    save_U2(fd, n);
    fwrite(s, 1, n, fd);
  }

  void save_Integer(FILE *fd, U4 i)
  {
    save_U1(fd, CONSTANT_Integer);
    save_U4(fd, i);
  }

  void save_Float(FILE *fd, F4 f)
  {
    save_U1(fd, CONSTANT_Float);
    save_U4(fd, *reinterpret_cast<U4*>(&f)); // warning: this is valid assuming this machine uses IEEE 754
  }

  void save_Long(FILE *fd, U8 l)
  {
    save_U1(fd, CONSTANT_Long);
    save_U8(fd, l);
  }

  void save_Double(FILE *fd, F8 d)
  {
    save_U1(fd, CONSTANT_Double);
    save_U8(fd, *reinterpret_cast<U8*>(&d)); // warning: this is valid assuming this machine uses IEEE 754
  }

  void save_String(FILE *fd, U2 r)
  {
    save_U1(fd, CONSTANT_String);
    save_U2(fd, r);
  }

  void save_Class(FILE *fd, U2 r)
  {
    save_U1(fd, CONSTANT_Class);
    save_U2(fd, r);
  }

  void save_Field(FILE *fd, Pair& p)
  {
    save_U1(fd, CONSTANT_Field);
    save_U2(fd, p.first);
    save_U2(fd, p.second);
  }

  void save_Method(FILE *fd, Pair& p)
  {
    save_U1(fd, CONSTANT_Method);
    save_U2(fd, p.first);
    save_U2(fd, p.second);
  }

  void save_InterfaceMethod(FILE *fd, Pair& p)
  {
    save_U1(fd, CONSTANT_InterfaceMethod);
    save_U2(fd, p.first);
    save_U2(fd, p.second);
  }

  void save_NameAndType(FILE *fd, Pair& p)
  {
    save_U1(fd, CONSTANT_NameAndType);
    save_U2(fd, p.first);
    save_U2(fd, p.second);
  }

  void save_Code(FILE *fd, const Code& code)
  {
    save_U4(fd, static_cast<U4>(code.size()));
    fwrite(code.c_str(), 1, code.size(), fd);
  }

  void save_U1(FILE *fd, U1 n)
  {
    fputc(n, fd);
  }

  void save_U2(FILE *fd, U2 n)
  {
    save_U1(fd, H1(n));
    save_U1(fd, L1(n));
  }

  void save_U4(FILE *fd, U4 n)
  {
    save_U2(fd, H2(n));
    save_U2(fd, L2(n));
  }

  void save_U8(FILE *fd, U8 n)
  {
    save_U4(fd, H4(n));
    save_U4(fd, L4(n));
  }

  static U1 H1(U2 n)
  {
    return static_cast<U1>(n >> 8);
  }

  static U1 L1(U2 n)
  {
    return static_cast<U1>(n);
  }

  static U2 H2(U4 n)
  {
    return static_cast<U2>(n >> 16);
  }

  static U2 L2(U4 n)
  {
    return static_cast<U2>(n);
  }

  static U4 H4(U8 n)
  {
    return static_cast<U4>(n >> 32);
  }

  static U4 L4(U8 n)
  {
    return static_cast<U4>(n);
  }

  ClassFile cf;   // the class file
  Code      code; // the current code being emitted

};

#endif
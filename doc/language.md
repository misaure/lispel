Language Reference
==================

> This document describes the core language implemented by the default
> interpreter implementation. When the first finalized version of Lispel
> is available, there should also be some extension packages ready for
> download. These extensions won't be described here, as they aren't part
> of the core language. The core language roughly resembles the Schame language
> as described in the R4RS.

Design Principles
-----------------

The general idea behind Lispel is to provide a Scheme interpreter which
can easily be embedded into your own C++ programs. But it is
also possible (just like it is in any other scripting language) to use
Lispel as a stand-alone interpreter with some user-supplied extensions.

This basically requires that you can on the one hand add new functions
and on the other hand this idea is moved much ahead when the type
system itself is extensible. Lispel provides both possibilities.
The strict object-oriented implementation of the interpreter revealed,
ehm, design flaws in the Scheme language. In R4RS, the types required
for a standards compliant implementation are mostly unrelated and hence,
most functions can only applied to one specific type.

**META:** this paragraph is obsolete!

While implementing types based on C++ classes it became obvious that types
can be divided into three classes: atomic types (or scalar types),
unidirectional sequences (e.g. consed lists) and random access sequences
(e.g. vectors). This is no genious idea, it is simply the result of basing
the implementation on STL container types and iterators. As both concepts,
containers and iterators, are of great practical use and should be familiar
to OO-Programmers, I decided to add the concept of iterators to the core
language. All builtins that worked on containers in original Lisp or
Scheme will operate on iterators in Lispel. Here is a short example:

```
> (define aVector #(1 2 3 4))
> (map display (forward-iterator aVector))
1 2 3
```

In R4RS-Scheme map is the only function which can be used to apply a
function to a sequence and, even worse, map only works for lists. If
you wanted to use similar functionality with vectors, you would have to
define a function 'vector-map' on your own. From an OOP-view, this isn't
acceptable at all.


Language Reference
------------------

In this chapter you will find a quick reference of the Lispel functions
provided by the core interpreter distribution. The functions are grouped
into command subsets, each of which has to be explicitly added to each
interpreter instance. This allows your applications some control over what
functionality is available on the scripting language level. If you for
example don't want to allow or simply don't need file accesses, then you
can omit the call to the addPortCommands
API function during the interpreter initialization sequence.

### Syntax

**META:** MISSING

### Data Types

**META:** most types are still MISSING

This section describes the various data types supported by Lispel.
You should keep in mind that whenever the lexical form of data types is
described that the scanner's behaviour can be changed by manipulating the
character class table. The following descriptions always refers to the
scanner's default behaviour as it is provided after calling the API function
initLexerModule().

#### Character classes

This are  the default character classes the scanner uses to recognize
tokens in Lispel programs:
 
```
ALPHA   ::= [a-zA-Z]
DECIMAL ::= [0-9]
```

#### Booleans

In contrast to Lisp and just like Scheme Lispel supports a boolean type.
They are written as #t for true and #f for false.
In boolean tests, all values except #f are regarded as true,
#t mainly exists for clarity of notation. For example, all
builtin predicates will return #t instead of some other arbitrary
value to return true values.

```
boolean ::= #t | #f
```


#### Numbers

Lispel recognizes integer and double values as supported by the underlying
system (ie. the C++ compiler used to compile Lispel). It is not planned to
support big integers (found in many lisp implementations), rationals or
complex numbers. Integer number are simple a sequence of decimal digits,
double values are a sequence of decimal digits, a dot and then again a
sequence of digits. To make this more clear:

```
integer ::= {DECIMAL}+
double  ::= {DECIMAL}+'.'{DECIMAL}+
```

**NOTE** The feature described here currently isn't implemented!

Numbers with a radix other than 10 can be read by prefixing them with one
of the following:
 <ul>
* ???
 </ul></p>

#### Keywords

Keywords are similar to symbols in that they are identifiers which are
used give things a name. The evaluator treats keywords specially as they
always evaluate to themselves (so you don't have to quote them). This makes
them quite handy in many situations, for example as keys for hash tables.
Keywords tokens are (by default) sequences of characters introduced by a
colon:

```
keyword ::= ':'{ALPHA}+
```


### Core Commands

The core commands are made available by calling the addCoreCommands API
function for each Interpreter instance.

#### Special forms

As the current implementation doesn't support macros, all special forms
are implemented as builtins. Whenever you instantiate an interpreter you
should at least add the functions described in this section.

* (define <symbol> <expr>)<br>
  Binds the value of <expr> to <symbol>
  in the toplevel binding environment. If the symbol is already defined it's
  value will be overwritten.<br>
  <strong>Returns:</strong>The symbol which was assigned a value.
* (quote <expr>)<br>
  Avoid evaluation of an expression.<br>
  <strong>Returns:</strong>The expression passed as a parameter without
  evaluating it.
* (if <expr1> <expr2> <expr3>)<br>
  Conditionally evaluates either expr2 or expr3, depending on the boolean
  value of expr1. expr3 will only be evaluated if expr1 evaluates to
  #f.<br>
  <strong>Returns:</strong>The result of evaluating either expr2 or expr3.
* (and <expr1> ... <exprn>)<br>
  Evaluates the expressions passed as arguments from left to right until an
  expression evaluates to #f.<br>
  <strong>Returns:</strong>#f if any of the expressions evaluated   to false or the value of the expression last evaluated, else.
* (or <expr1> ... <exprn>)<br>
* (cond <clause> ... <clause>)<br>
* (let <bindings> <expr1> ... <exprn>)<br>
* (let* <bindings> <expr1> ... <exprn>)<br>


#### Basic Functions

Some of the functions described here might only be available after
loading the file lispellib.scm into the interpreter.

* (display <expr>)
* (begin <expr1> ... < exprn>)
* (car <pair>)
* (cdr <pair&gt)
* (eq? <expr1> <expr2>)
* (eqv? <expr1> <expr2>)
* (cons <expr1> <expr2>)
* (boolean? <expr>)
* (symbol? <expr>)
* (char? <expr>)
* (pair? <expr>)
* (double? <expr>)
* (integer? <expr>)
* (string? <expr>)
* (keyword? <expr>)
* (hashtable? <expr>)
* (set? <expr>)
* (not <expr>)
* (memstat)
* (gc)


### List Processing Commands

 <p>The functions described in this section provide basic features for
 manipulating lists, the most important data structure in all Lisp dialects.
 [META: should cons and pair? be moved here?]</p>

* (list <expr1> ... <exprn>)
* (length <list>)
* (null? <expr>)
* (set-car! <pair> <obj>)
* (set-cdr! <pair> <obj>)


### Math Commands

The math commands support the most common mathematical operations on
integers (referred to as exact numbers) and doubles (referred to as
inexact numbers). As an extension to Scheme, Lispel also supports
arithmetic operations on vectors (of doubles). Numeric vectors are handled
transparently as a datatype of their own to enable efficient computations.

* (+)
* (+ <m_op>)
* (+  <m_op1> ... <m_opn>)
* (*)
* (* <m_op>)
* (*  <m_op1> ... <m_opn>)
* (- <m_op>)
* (-  <m_op1> ... <m_opn>)
* (/ <m_op>)
* (/  <m_op1> ... <m_opn>)
* (= <m_op1> <m_op2>)
* (< <m_op1> <m_op2>)
* (> <m_op1> <m_op2>)


### Hashtable commands

* (make-hash)
* (hash-get <table> <key>)
* (hash-set <table> <key> <value>)
* (hash-clear <table>)
* (hash-size <table>)
* (hash-remove <table> <key>)

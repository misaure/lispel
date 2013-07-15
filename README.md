lispel
======

A simple interpreter for a Scheme-like scripting language

History
=======

Lispel started (and still is) a pet project that started after I read the GoF
book about design patterns, "An Introduction to Scheme and its Implementation"
and later "Uniprocessor Garbage Collection Techniques" in sequence. This lead
to the idea of trying out how configurable a virtual machine based interpreter
for a Scheme-based language could be if it was implemented with a strong 
emphasis on object-oriented patterns.

The work lead to an interpreter that was able to execute subsets of R5RS 
Scheme and a subset of JavaScript (and a platform abstract library called
"MiscClass"). Sadly, the final version of the interpreter and all of the
MiscClass library was lost when my hard disk was stolen and I lost interest.

Some weeks ago I found an old backup of parts of the Lispel implementation and
it got me excited again to build an interpreter based on a virtual machine.
The main idea still is to try to avoid the usual C-hackery that is required to
build an efficient interpreter for almost any language. Instead, the goal is
that the code is portable and that the basic ideas and the common structure
of interpreted languages become clear.


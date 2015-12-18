# Context
Depending on how the core library is setup and the capabilities of the platform, this may be a dedicated thread or process.

# Cassowary
Refers to the Cassowary Linear Arithmetic Constraint Solving Algorithm. This is used as the basis of the constraint solver used for determining linear equality and inequality constraints between the elements in the layout. The details of this algorithm are described in this [paper](https://constraints.cs.washington.edu/solvers/cassowary-tochi.pdf).

# Display Link
A channel provided by the core library that is worken up using a trivial write when the display is ready to be updated. Pulses on this channel arrive at a rate equivalent to the screen refresh rate.

# Indeterminate
An indeterminate is a symbol that is treated as a variable. These are placeholders in polynomial equations.

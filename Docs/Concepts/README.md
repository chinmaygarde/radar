# Core Concepts

##### *Absolutely all APIs in Radar are declarative. No exceptions.*

All subsystems in Radar can be controlled by declaring intent. The exact nature of the interaction depends on the subsystem. But generally, mathematical equations are preferred. You will observe that almost all aspects of an entity in the user interface can be used as an indeterminate in a polynomial (or more commonly linear) equation.

There are five major subsystems in Radar. Activities like layout, event handling and recognition, composition, lifecycle management, etc. are all handled by these subsystems.

Though these subsystems need to interact with one another for their operation, data flow throughout the system is highly directed. These subsystems communicate with one another over the channel abstraction provided by the core library. Channels provide a means of communications between components that are in different contexts, i.e, over a process or thread boundary.

# Major Subsystems
There are 5 major subsystems in Radar:
## Interface
* The entire surface area of the API available to application code is present in this subsystem.
* The interface is in its own isolated context. Though other subsystems may share the same context, they are never in the same context as an interface.
* There may be multiple active interfaces in multiple contexts at any given time.
* Is responsible for the setup of the entity hierarchy that forms the visual structure of the user interface.
* Layout are expressed declaratively using mathematical equations.
  * Constraints are expressed as linear equations.
  * Layout resolution does not happen on the interface.
* Gesture recognizer are declaratively setup using polynomial equations.
  * User code is afforded no opportunity to receive raw touch input events.
  * Event handling and gesture recognizer resolution does not happen on the interface.
* All declarative intent is communicated to an external entity by writing to a channel the interface received during its construction.


## Coordinator
* Owns a channel for each active interface. Declarative intent from these interfaces is received by this subsystem.
* Owns a per interface presentation graph.
  * This is a directed acyclic graph of user interface entities that are part of the visual structure described by that interface.
* Owns the channels on which raw input events are received.
  * Works in conjunction with the gesture recognition subsystem to resolve updates to presentation graphs due to user interaction.
* Owns a per interface layout engine that is responsible for resolving layout constraints and updating properties of entities in the user interface.
  * The coordinator is also responsible for deciding when each layout engine need to resolve its constraints.
* Owns a per interface animation director.
  * Each animation director maintains an interpolator for an entities-property pair that is currently under the effect of an animation.
* Owns the primary channel on which display link pulses are received.
  * This is used to decide how to update the animation directors, relayout and eventually swap a frame.
* The collated state of the presentation graph after all updates is communicated to the compositor for display onto the screen.
* Typically, there is only one coordinator on a system.


## Layout Engine
* The position and layout constraints of all entities in the interface are determined by linear quality and inequality constraints given the layout engine
* Constraints are in the form of linear equations. The indeterminates of the the linear equations are properties of entities within the visual hierarchy.
* During the time they are active, touches are represented as entities that are part of the interface. Since they are first class citizens of the interface, layout constraints can be setup between touches and visual entities using a uniform API.
* The layout engine is an implementation of the Cassowary Linear Constraint Solver.


## Animation Director
* This subsystem maintains the set of interpolators for active animations within the interface
* Animations are configured in a declarative manner on the interface.
  * There is no provision for interfaces to imperetively control the behavior of an active animation.
  * Certain interfaces (for example, games) may register a display link callback and update entity properties as they see fit.
* Most properties on entites are animatable. This includes, position, bounds, transformation, opacity, background colors, etc..


## Gesture Recognition Engine
* A gesture recognizer is declaratively defined as a set of polynomials.
* The subsystem provides a set of predefined indeterminates that can be used in place of input events that have not occurred at the time of gesture recognizer setup.
  * For example, one of the recognizer inderminates can the touch entity that will be created when the user taps the screen. Obviously, when the recognizer is initially setup, this entity is not present, but the interface can use proxy indeterminates that will be replaced by the actual entity representing the touch when the recognizer is active.
* Boundary conditions on gesture recognizers can be expressed in the form of polynomial equations as well.
* Extremely complex gestures can be expressed purely mathematically using multiple polynomials.
* The interface has no say in the operation of the gesture recognition engine.
  * The interface context may be idle or processing application business logic when the gesture recognition is under effect.
  * The interface may register certain interest points within recognition. Callbacks will be delivered to the interface when the recognotition engine receives these events.


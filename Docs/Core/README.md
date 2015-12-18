# The Core Library

The Core Library provides the basic primitives used by all subsystems in Radar. This includes:

## Channel
* The basic means of communication between two entities that may (but dont have to) be across thread or process boundaries.
* The core library has multiple implementations of channels and picks the one best suited for the platform.
  * **Mac/iOS**: `Mach` ports
  * **Linux/Android**: `Unix` Domain Sockets
  * **Native Client**: No `IMC` available in the public SDK. Uses in-process queues.
* Messages over channels are be sent atomically.
* Channels may be used as sources to event loops, waking them up as messages arrive on them and giving custom handlers an opportunity to read and interpret these pending messages.
* The channel implemnentations try to prevent the number of buffer copies when sending and receiving messages.
  * On Mac and Native Client, there are zero copies (there is still the allocation). The Linux implementation has one copy. Point being, these are fast and getting faster as I tinker on them. There is still lots of room for improvement.

## Event Loops
* The core library has multiple implementations of event loops and picks the one best suited for the platform. The results in decisions based on the available kernel event multiplexing APIs.
  * **Mac/iOS**: `kqueue`
  * **Linux/Android**: `epoll`
  * **Native Client**: No available multiplexing API. But no `IMC` either. So uses thread condition variables.
* Any native descriptor that can be used by the underlying kernel event multiplexing API can be wrapped to form an event loop source.
  * All aspects of the wrapped (or preconfigured) event loop source are configurble. This includes:
    * How data is read from the underlying descriptor.
    * How data is written to the underlying descriptor.
    * How the descriptor is added and removed from the set of descriptors multiplexed on.
    * How the lifecycle of the underlying descriptor is managed.
  * This allows for descriptors that were vended from outside the core library to be first class citizens on the event loop. For example, an `ALooper` file descriptor obtained from Android may be wrapped with custom handlers to form an event loop source.
* The core library already provides a set of preconfigured event loops sources. These include:
  * **High Resolution Timers**
  * **Trivial Sources**: Their sole purpose is to wake up a loop but they don't actually deliver valuable information.
    * Implemented using `Mach` ports with with unit queue limit caps with zero timeouts on Mac. Uses event file descriptors on Linux. Where `IMC` is unavailable, uses thread condition variable.
    * Multiple unservices wakes on trivial sources are coalesced.
  * **Display Link Sources**: These sources are triggered at a rate equivalent to (and synchronized with) the screen refresh rate.
    * These are used by the compositor mostly. Application that do their own interpolations or rendering may register for these as well.

## High Resolution Timers
Allows creation of event loop sources that wake up at a set interval. The implementation varies per platform. Implemented using `EVFILT_TIMER` on Mac, timer file descriptors on Linux, and, per wait-set max heaps managing timeouts on condition variables where `IMC` is unavailable.

## Thread Synchronization Primtives
Implements the sundry thread synchonization primitives like countdown latches, semaphores and condition variables.

## Message Serialization & Deserialization
Messages sent across channels must be marshalled and unmarshalled. Utilities in the core library try to make this as frictionless as possible. These utilities work with the channel implementations to ensure that the number of buffer copies while sending and receiving messages as minimal as possible.

## Shared Memory Primitives
Allows for the setup of shared memory arenas.

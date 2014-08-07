G.O.D: Grand Object-bound Dispatcher
==========

G.O.D is a high performance non-blocking task dispatcher which guarantees class member functions' execution sequence 

## FEATURES
* Designed for multi-platform by using C++11 standard
 * Visual Studio Solutions for now, but you can easily use in *NIX environment (Just #include relevant files)
* std::bind and std::shared_ptr are used instead of a custom Job class interface
* Lock-free (non-blocking) algorithms
* Using custom STL allocator
* Deferred task execution using Timer
* Simple code-base for easy to understand (easy to adapt to other projects)





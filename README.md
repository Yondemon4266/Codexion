*This project has been created as part of the 42 curriculum by aluslu.*

## Description

This project is a multi-threaded simulation in C that models a scenario where multiple "coders" (threads) compete for a limited number of shared resources, called "dongles," to perform a task (compiling). The goal is to successfully complete a set number of compilations before a deadline, known as "burnout."

Each coder needs two dongles to start compiling. If the required dongles are in use, the coder must wait. The simulation is overseen by a monitor thread that checks for success (all coders complete their compilations) or failure (a coder "burns out" by waiting too long). The project demonstrates key concepts in concurrent programming, including thread synchronization, deadlock prevention, and resource management.

## Instructions

To compile the project, run `make`.

**Usage:**
```shell
./coders nb_coders t_burnout t_compile t_debug t_refactor nb_comp t_cooldown scheduler
```

**Parameters: (all mandatory)**
*   `nb_coders`: The number of coder threads to create.
*   `t_burnout`: The maximum time (in ms) a coder can go without starting a compilation before they "burn out."
*   `t_compile`: The time (in ms) it takes for a coder to complete one compilation.
*   `t_debug`: The time (in ms) a coder spends debugging after compiling.
*   `t_refactor`: The time (in ms) a coder spends refactoring after compiling.
*   `nb_comp`: The number of times each coder must compile.
*   `t_cooldown`: The time (in ms) a dongle must remain inactive after being used.
*   `scheduler`: The algorithm to manage dongle allocation:
    *   `fifo`: Grants the dongle to the first coder who requested it.
    *   `edf`: (Earliest Deadline First) Grants the dongle to the coder with the most urgent deadline to prevent burnout.

## Resources

The following resources were helpful for understanding the concepts used in this project:

*   **Threads and Mutexes in C:** [Threads, Mutexes, and Concurrent Programming in C](https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/)
*   **General Concurrency Concepts:** [YouTube Playlist on Concurrency](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)

**AI Usage:**
AI (Gemini) was used to clarify complex topics such as the implementation of condition variables (`pthread_cond_t`) and to help optimize the Earliest Deadline First (EDF) scheduling algorithm and the queue management system. It also assisted in analyzing the codebase to generate detailed explanations for this README.

## Blocking Cases Handled

This project addresses several critical concurrency issues to ensure the simulation runs correctly and fairly.

*   **Deadlock Prevention:** Deadlock is avoided by enforcing a strict locking order. Coders always attempt to lock the two required dongles in a consistent sequence (based on their memory address). This prevents a circular wait scenario, which is one of the four Coffman conditions for deadlock.
*   **Starvation Prevention:** The choice of scheduler helps prevent starvation (where a thread is perpetually denied a resource).
    *   The **FIFO** scheduler ensures basic fairness by serving coders in the order they arrive.
    *   The **EDF** scheduler provides more advanced starvation prevention by prioritizing coders whose deadlines are approaching, ensuring they get a chance to compile before burning out.
*   **Cooldown Handling:** After a dongle is used, it enters a "cooldown" period. The simulation tracks the release time of each dongle. If a coder tries to acquire a dongle that is still in cooldown, the coder's thread will sleep until the cooldown period is over.
*   **Precise Burnout Detection:** A dedicated monitor thread continuously tracks each coder's status. It calculates the time elapsed since a coder's last compilation. If this time exceeds the `t_burnout` limit for any coder, the monitor flags a burnout and safely terminates the entire simulation.
*   **Log Serialization:** To prevent jumbled output from multiple threads printing at the same time, a global mutex (`print_lock`) is used. Any thread that needs to write to the console must first acquire this lock, ensuring that log messages are printed one at a time in a clear, serialized manner.

## Thread Synchronization Mechanisms

The simulation relies on several `pthreads` primitives to coordinate actions between threads and protect shared data.

**Primitives Used:**
*   `pthread_mutex_t`: Mutexes are used as locks to protect shared resources and ensure that only one thread can access critical data at a time.
    *   **Dongle Mutex (`t_dongle.mutex`):** Each dongle has its own mutex to protect its data, such as its user queue and cooldown status.
    *   **Coder Mutex (`t_coder.coder_lock`):** Each coder has a mutex to protect its personal state, like its compilation count and last compilation time.
    *   **Global Mutexes (`stop_lock`, `print_lock`, `start_lock`):** These are used to protect global resources, including the simulation's stop flag and the standard output for printing.
*   `pthread_cond_t`: Condition variables are used to signal threads to wake up when a specific condition has been met, avoiding inefficient busy-waiting.
    *   **Start Condition (`start_cond`):** Used by the main thread to signal all coder threads to begin their routines simultaneously, ensuring a fair start.
    *   **Compilation Wait Condition (`wait_compil_cond`):** Allows a coder to sleep efficiently while waiting for the necessary dongles to become available. When a dongle is released, the releasing thread broadcasts on this condition variable to wake up waiting threads.

**How They Work Together:**

1.  **Initialization:** All coder threads are created but are immediately put to sleep using `start_cond`.
2.  **Starting the Simulation:** The main thread broadcasts on `start_cond`, waking up all coders at once.
3.  **Requesting Dongles:** A coder wanting to compile first locks the mutexes of its two required dongles and adds itself to their respective queues.
4.  **Waiting for Dongles:** The coder then waits on its `wait_compil_cond`. This puts the thread to sleep, releasing the lock it was holding, which allows other threads to run.
5.  **Acquiring Dongles & Compiling:** When another coder releases a dongle, it broadcasts a signal. The waiting coder wakes up, re-acquires its lock, and checks if it's now at the front of both dongle queues. If so, it proceeds to compile.
6.  **Releasing Dongles:** After compiling, the coder locks the dongles' mutexes again, removes itself from their queues, and broadcasts on the condition variable to wake up the next waiting coder.

This combination of mutexes and condition variables prevents race conditions and ensures that coders can communicate and access shared resources (dongles, logs, and monitor state) in a safe and organized way.

## Some tests for the code


### Invalid tests:

Scheduler not fifo or edf.
```shell
./codexion 5 800 200 200 200 7 10 abc
```
Negative numbers
```shell
./codexion -5 800 200 200 200 7 10 edf
```
Integers over INT_MAX (2147483647)
```shell
./codexion 2 800 200 200 2147483648 7 10 edf
```
Missing arguments (all arguments are mandatory).
```shell
./codexion 5 800 200 200
```

### Valid tests:

Unique coder (burnout unavoidable, can't compile with only 1 dongle).
```shell
./codexion 1 800 200 200 200 7 10 edf
```

Burnout with many coders.
```shell
./codexion 4 310 200 200 200 7 10 fifo
```

Can survive with these parameters:
```shell
./codexion 5 800 200 200 200 7 10 edf
```

0 compilation test:
```shell
./codexion 5 800 200 200 200 0 10 edf
```
Cooldown high:
```shell
./codexion 4 400 200 200 200 5 150 edf
```

Pressure tests on EDF with 200 coders in a tight timing:
```shell
./codexion 200 400 60 60 60 5 10 edf
```
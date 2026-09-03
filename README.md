*This project has been created as part of the 42 curriculum by sboudcha.*

# Codexion

## Description

**Codexion** is a concurrent programming project in C designed to explore multi-threading, resource contention, deadlock prevention, and scheduling algorithms under strict real-time constraints.

The simulation models a team of **Coders** (threads) competing for a limited set of **Dongles** (shared resources) needed to compile their code. Each coder must acquire two neighboring dongles, compile, debug, and release the dongles, which then enter a cooling-down period before becoming available again. If a coder fails to compile within a specific timeframe (`time_burnout`), they suffer a **Burnout**, terminating the simulation. A dedicated **Monitor thread** continuously supervises the system state to ensure real-time safety, log actions, and detect burnout events or completion criteria.

---

## Instructions

### Compilation

Build the executable using the provided `Makefile`:

```bash
# Compile the executable
make

# Clean object files
make clean

# Clean object files and executable
make fclean

# Recompile everything
make re

```

### Execution Syntax

```bash
./codexion <n_coders> <time_burnout> <time_compile> <time_debug> <time_cooldown> <compiles_required> <scheduler>

```

| Parameter | Type | Description |
| --- | --- | --- |
| `n_coders` | Integer | Total number of coders (and dongles) |
| `time_burnout` | Milliseconds | Maximum allowed time without compiling before burnout |
| `time_compile` | Milliseconds | Time taken to perform a compilation |
| `time_debug` | Milliseconds | Time taken to debug after compilation |
| `time_cooldown` | Milliseconds | Cooldown duration for a dongle after release |
| `compiles_required` | Integer | Number of successful compilations required per coder |
| `scheduler` | String | Resource scheduling policy: `fifo`, `edf`, or `lst` |

### Usage Example

```bash
./codexion 4 3100 1000 500 800 3 fifo

```

---

## Blocking Cases Handled

### 1. Deadlock Prevention & Coffman's Conditions

Deadlocks occur when all four Coffman conditions are met (*Mutual Exclusion*, *Hold and Wait*, *No Preemption*, and *Circular Wait*). Codexion prevents deadlocks by eliminating **Hold and Wait** and **Circular Wait**:

* **Atomic Acquisition:** Coders never hold one dongle while waiting for another. A coder acquires both required dongles simultaneously or releases any claim and waits.
* **Resource Ordering:** When evaluating requests, dongle indices are acquired in a strict, uniform order (e.g., lower index first), breaking symmetry and preventing circular lock dependency chains.

### 2. Starvation Prevention

Under high contention, naive locking strategies can cause threads with high deadlines to starve threads facing immediate burnout.

* Codexion implements thread scheduling mechanisms (**FIFO**, **EDF** - *Earliest Deadline First*, and **LST** - *Least Slack Time*).
* Coders request resources through a centralized priority-aware queue system so that coders closest to burnout get priority allocation over newly finished coders.

### 3. Cooldown Handling

When a coder releases a dongle, it cannot be reused immediately during its `time_cooldown`.

* Instead of keeping the releasing thread sleeping and blocking other execution, dongle availability timestamps (`available_at`) are tracked asynchronously.
* Coders requesting a cooling-down dongle are safely suspended on condition variables without blocking unrelated coders from acquiring free dongles.

### 4. Precise Burnout Detection

* The **Monitor thread** runs an asynchronous loop checking coder timestamps.
* Burnout checks calculate `current_time - last_compilation_time`. To prevent false burnout detection due to thread scheduling latency, state checks are protected by fine-grained time mutexes (`time_mutex`), ensuring atomic reads of timestamp structures (`struct timeval`).

### 5. Log Serialization

Multiple threads outputting log messages simultaneously can lead to garbled, interleaved terminal output.

* A dedicated logging mutex (`log_mutex`) serializes all stdout operations.
* Once the simulation reaches a terminal state (burnout or full completion), logging is atomically disabled via a global `game_over` flag to prevent stale logs after termination.

---

## Thread Synchronization Mechanisms

### Threading Primitives

* **`pthread_mutex_t`**:
* `log_mutex`: Protects terminal output printing.
* `time_mutex`: Protects access to individual coder compilation timestamps (`lst_compile_tv`) and compilation counts.
* `state_mutex`: Protects shared state data, priority queues, and dongle status arrays (`is_free`, `available_at`).


* **`pthread_cond_t`**:
* Used to suspend coders when dongles are unavailable or cooling down. Suspended threads consume zero CPU cycles while waiting, avoiding busy-wait CPU saturation (`while` loop spinning).
* Broadcast signals (`pthread_cond_broadcast`) wake up sleeping threads when dongles become free or when the simulation is forced to terminate.



### Shared Resource Coordination

```
[ Coder Thread ] ----(Locks time_mutex)----> Updates lst_compile_tv
       |                                              |
(Locks state_mutex)                                  v
       |                                     [ Monitor Thread ]
       +---------> Checks/Modifies Dongles <---------+

```

* **Race Condition Prevention:** Any read/write operation on shared variables (`compiles_count`, `game_over`, `finished_coders`, queue structures) is wrapped strictly inside mutex lock/unlock pairs.
* **Thread-Safe Monitor Communication:** The Monitor thread never writes directly to coder execution loops. Instead, it observes state under `time_mutex` and `state_mutex`. If a burnout condition is met, it sets `game_over = 1` under lock and broadcasts condition variables to unblock all coders for a clean join and shutdown sequence.

---

## Resources

### References & Documentation

* [POSIX Threads Programming (LLNL Tutorial)](https://www.google.com/search?q=https://hpc-tutorials.llnl.gov/posix/)
* [The Dining Philosophers Problem & Concurrency Control](https://www.google.com/search?q=https://en.wikipedia.org/wiki/Dining_philosophers_problem)
* `man pthread_mutex_init`, `man pthread_cond_wait`, `man gettimeofday`

### AI Assistance Declaration

In accordance with the 42 curriculum guidelines, Artificial Intelligence (Large Language Models) was used during this project for the following tasks:

* **Debugging Concurrency Logic:** Identifying potential edge-case race conditions in priority queue operations and condition variable signaling.
* **Code Refactoring & Norminette Alignment:** Structuring helper functions to respect function line limits and improve code readability.
* **Documentation:** Drafting structural sections and formatting the `README.md` file according to project requirements.
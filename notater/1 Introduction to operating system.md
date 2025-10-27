## 1) What is an operating system?

**Operating system** = the software layer that manages a computer’s hardware resources **for users and applications**.
- It decides **who gets what, when**, and **keeps everyone from stepping on each other**.
---
## 2) The three roles of an operating system

1. **Referee (isolation + controlled sharing)**
    - Allocates CPU time, memory, disk, and network.
    - Prevents one program/user from corrupting another’s data.
    - Provides safe communication channels between programs.
2. **Illusionist (nice abstractions)**
    - Makes a single machine look **bigger/simpler** than it is: each app feels like it has the whole machine, “infinite” memory, reliable storage, reliable network.
    - Hides messy hardware details behind clean interfaces.
3. **Glue (developer conveniences)**
    - Provides libraries, system calls, user interface widgets, and naming (files, directories) so programs can do useful work without re‑inventing the wheel.
---
## 3) Example: file systems (how the three roles show up)

- **Referee:** Enforces permissions so other users cannot read your files, even after deletion and space reuse.
- **Illusionist:** Files can grow as needed; data appears durable even if a crash happens mid‑save (journaling, write ordering, recovery).
- **Glue:** Provides directories, file names, and utilities (for example, `printf`, standard libraries) so programs can read/write easily.
---

## 4) Core challenges an operating system must handle

### Reliability and availability
- **Reliability:** The system does **what it is designed to do**. Bugs and crashes break reliability.
- **Availability:** **How much of the time** the system is up and serving. Usually measured with **Mean Time To Failure (MTTF)** and **Mean Time To Repair (MTTR)**.
### Security and privacy
- **Security:** Can an attacker compromise the system? (Privilege escalation, code injection, malware.)
- **Privacy:** Only **authorized** users can access data.
### Portability (run on different hardware)
- For applications: expose a stable **Application Programming Interface (API)** and an **Abstract Virtual Machine (AVM)** view of the computer.
- For the operating system itself: use a **Hardware Abstraction Layer (HAL)** to hide device specifics.
### Performance (fast and fair)
- **Latency / response time:** How long a single operation takes.
- **Throughput:** How many operations per unit time.
- **Overhead:** Extra work the operating system adds beyond the useful work.
- **Fairness:** Users/programs get comparable service.
- **Predictability:** Performance is consistent over time (no nasty surprises).
---
## 6) Case study: a web service

- **Concurrency:** Handle many client requests at once (threads, processes, event loops, async I/O).
- **Safety:** Keep the client safe from untrusted scripts (browser sandboxes, same‑origin policy) and keep the server safe from users (input validation, isolation).
- **Consistency:** Rolling updates without serving inconsistent pages (transactions, versioned deployments, blue/green, atomic swaps).
---
## 7) A 60‑second history of operating systems
### Early days: computers were expensive
- One application ran at a time with full hardware control; the operating system was basically a **runtime library**.
- **Batch systems:** Keep the CPU busy with a **queue of jobs**; users submitted jobs and waited.
### Time‑sharing: people and computers both expensive
- **Multiprogramming:** Run multiple programs at once.
- **Interactive performance:** Minimize user wait time. As hardware got cheaper, it became smarter to **optimize for human time** rather than machine time.
### Today: computers are cheap and everywhere
- Operating systems power **smartphones**, **embedded systems**, **laptops**, **tablets**, **virtual machines**, and **data‑center servers**.
### Tomorrow: scale and parallelism
- **Giant data centers**, many processors per machine, many machines per user, and **very large‑scale storage**.
---
## 8) Quick reference (pin this)

- **Operating system roles:** Referee, Illusionist, Glue.
- **Reliability vs Availability:** Correctness vs Uptime.
- **Security vs Privacy:** Resist attacks vs restrict access to data.
- **Performance knobs:** Latency, Throughput, Overhead, Fairness, Predictability.
- **Portability levers:** API/AVM for apps, HAL for the operating system.
- **Scheduling reality:** Mixed strategies, tuned to goals.
---
## 10) Summary

An operating system **allocates resources**, **isolates and connects programs/users**, and **hides hardware complexity** behind helpful abstractions. To be useful, it must be **reliable, available, secure, private, portable, and fast enough** - and it evolves as hardware and usage patterns change.
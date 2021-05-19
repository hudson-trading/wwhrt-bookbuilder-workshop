# The Hudson River Trading Bookbuilder Workshop

This workshop will give you a taste of what it's like to work as a Core
Developer at HRT. There are many teams at HRT that write code, but the Core team
works on all of the low-level infrastructure that supports our trading. One
aspect of this is writing code that allows us to interface with various
financial exchanges.

When HRT wants to trade any given product on an exchange, the first step is
getting market data for the exchange order book. The order book is a list of
orders representing all the outstanding interest of buyers and sellers,
organized by price level. Whenever a new order arrives that would satisfy an
existing order on the book, the exchange will inform the two parties that a
trade took place and modify the existing order as necessary (either removing it
or reducing its quantity).

In many types of trading that HRT does, it is useful to keep track of the
individual orders resting on the exchange's book. So in this workshop exercise,
we will optimize a data structure that maintains the state of the book as it
processes order updates. These order updates can include add, deletes, and
modifies that apply to specific orders.

# The Bookbuilder

We've provided some code that implements a very naive bookbuilder structure as
well as the supporting framework to read our sample data.

You can find the naive code in [`BookBuilder.cc`](./BookBuilder.cc).

Your bookbuilder will receive events for individual orders by via callbacks.
Your implementation must keep track of all the open orders and be able to
respond to queries about the current best orders to buy and sell.

# The Data

We have provided sample data from a real cryptocurrency exchange.  These files
record events sent from the exchange.  There are three types of events that you
will need to handle: when orders are **added** to the book, **deleted** from
the book, or **updated** while remaining on the book. You can find more
information about these events in the [`Events.h`](./Events.h) header.

# Running the Code

We've provided a makefile that shoud make it simple to run the code on a x86_64
Linux environment. You will need a C++17 compiler as well as several packages.

To install the necessary prerequisites on Ubuntu you can run:
```
sudo apt install git wget tar gzip cmake build-essential binutils gdb libunwind8
```

Once you're all set up, you can run any of the commands below
| command | purpose |
|----|---|
| `make all` |  build the optimized and unoptimized versions of the code |
| `make measure` | runs the optimized code and outputs the time in second to fully process the data. |
| `make check` | runs the optimized code and verifies that after every event your bookbuilder gives the correct result.
| `make profile` | alias to `make profile_functions` |
| `make profile_functions` | runs the optimized code and measures which functions are taking the most time  |
| `make profile_lines` | like `make profile_functions` but instead shows which lines are taking the most time |
| `make gdb` |  alias to `make gdb_unoptimized` |
| `make gdb_unoptimized` | runs the unoptimzed code under GDB so you can step through and find bugs |
| `make gdb_optimized` | like `make gdb_unoptimzed` except uses the optimized version of your code in case your bug depends on optimizations being enabled |

Parameters
| parameter | rules | purpose |
|---|---|---|
| `SEQNUM` | `gdb`<br>`gdb_optimized`<br>`gdb_unoptimized` | you can use `make gdb SEQNUM=10` to causes GDB to immediately begin executing and sets a breakpoint such that GDB will stop executing once it arrives at the `CryptoEvent` with `seqNum=10` |
| `DATA` | `measure`<br>`profile`<br>`gdb` | you can use `make measure DATA=./data/blah` to measure the time to process a specific file. Or you can use `make profile DATA=./data/foo` to profile the time to process a specific file. |

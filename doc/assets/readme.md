# Carabiner architecture


## NOTE

This document is simply a summary of my own research into Carbiner's architecture.

The purpose was to understand it well enough to be reuse as much as possible when modifying it to use OSC.

Documenting here for reference.

**it may be incomplete, partially incorrect, or no longer accurate.**

<br>

## Initialization flow

```mermaid
sequenceDiagram

participant AbletonLink
participant gflags
participant mongoose
participant main

participant linkHandler

participant processMessage
participant eventHandler

main ->> gflags: parse flags
main ->> AbletonLink: set callbacks
main ->> mongoose: init + bind eventHandler
main-->mongoose: Poll (LOOP)

eventHandler ->> processMessage: on (MG_RECV)
eventHandler ->> linkHandler: call relevant fn handleX()
linkHandler ->> linkHandler: validate args

linkHandler ->> AbletonLink: captureAppSessionState()
linkHandler ->> AbletonLink: (sometimes) commitAppSessionState()
linkHandler ->> mongoose: mg_send(formatted)
```

# Network send flow
```mermaid
graph TD

reportStatus --> mg_send
handleVersion --> mg_send
reportBadArgument --> mg_send
handleBeatAtTime --> mg_send
handlePhaseAtTime --> mg_send
handleTimeAtBeat --> mg_send

```


# Modifications
1. `Carabiner.cpp`
    * Replaced `mg_send` calls with `send` function
    * if `SEND_OSC` flag: send an OSC message instead of `mg_send`
2. `osc_handler.cpp` - added function to handle OSc
3. `CMakeLists.txt` - support for new `osc_handler` library / linking
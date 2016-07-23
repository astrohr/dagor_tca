This directory is a template for all things specific to a local implementation.

## Initial setup


### 1. `local` 

Copy this directory to `local`. Directory tree should look like this:

    tca
     |
     ... (other directories)
     +-- local
     +-- local_example
     ... (other directories)
     

Directory `local` is not tracked by Git. Directory `local_example` is, so check
there for new bits of configuration after future code updates.


### 2. Configure USB names
 
Edit all `*_config.py` files and edit settings to match your local setup.

Alternatively (if running Linux), see `90-usb-dagor.rules` for instructions how
to keep using default USB device names (e.g. `/dev/dagor_focus` instead of 
`/dev/ttyUSB1`)

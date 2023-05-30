# HSBG at Odds -- A Hearthstone Battlegrounds Combat Odds Calculator.  
HSBG at Odds is a statistical analysis calculator aiming to "solve" the randomness of Hearthstone Battlegrounds and provide insight into the outcomes of each combat round.
I recognize that other programs such as *Bob's Buddy* and *Hearthstone Deck Tracker* already exist and likely do a better job than this project, 
but I am a student and thought this would be a fun project to work on with some valuable experience to be gained.
If you wish to contribute, feel free to contact me -- I will only be allowing contributions from people I know personally at the time being.

The following readme includes details on how to run the project from a Linux terminal environment.
Please start by cloning the git directory to your personal workspace using `git clone https://github.com/Neslo72/HSBG-at-odds`.

## Linux Setup
### Makefile
A `Makefile` is included within the `/src/` directory to make compilation of the various project targets easy.  Two options are included at the moment:
- Use `make` to compile the project into an executable
- Use `make clean` to remove the object files and executable made from `make`

### Execution
After using `make` to compile the executable, the project can be run by calling `./main`.  There are no command line arguments at the moment, but more customization for the simulation at run time is in the works.  To change the circumstances of the simulation, changes will need to be hardcoded into the `main.cc` file where there exists a brief example of two beast boards ready for combat (`p1_init` and `p2_init`).

## Features to Add
- Inclusion of other minion types
- Easier runtime customization of simulation environment
  - Read in board data from a file
- Golden minion effects
- Minion battlecries - could lead into Buy / Sell phase board tracking
  - *This feature is not very important...*

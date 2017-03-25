# Patrician

A game that intend to be a free clone of patrician III. It's written in C++11 nad uses my [ECS framework](https://gitlab.com/Emeraude/ECS).  
For now, there are two interfaces, a GUI one using SDL, and a CLI one, launched in a separate thread. See `help` in-game command for more informations about what is possible.

## Features

Here is the list of implemented features, depending on the interface:

Feature | CLI | GUI
---|:---:|:---:
Goods buying/selling | ✓ | ✗
City info display | ✓ | Partial
Players info display | ✓ | ✓
World info display | ✓ | Partial
Ships buying | Partial | ✗
Production facility buying | Partial | ✗

## Planned

Here is a list of things that will differ from the original game:

- Removal of sailor mood (useless)
- Addition of AI interactions
- Modification of the captain research
- Addition of shares buying/selling in solo
- Newly created ships should have a default name
- Modification of the auctions system (simultaneous auctions are awful)
- Addition of a manager for the bank
- A lot of UI modifications

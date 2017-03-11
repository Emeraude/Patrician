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

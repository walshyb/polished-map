# Polished Map Web

This is a web implementation of [Rangi42](https://github.com/Rangi42)'s map and tileset editor, [Polished Map](https://github.com/Rangi42/polished-map). This is mostly a rewrite as the original program was written entirely in C++, with rendering and iteraction handling done by a GUI framework, FLTK. This app is written in C++, compiled into Web Assembly via Emscripten, and JavaScript (React) for rendering the editor in the browser and handling interactions. Most of the C++ used is taken from the original project, without the FTLK references, and retrofitted to be called upon via JavaScript as needed.

This app is primarily meant to be for learning experience! Not to be a replacement of the original eeditor, as it is perfectly fine (and late '90s-early 2000's tileset practices are not going to be radically changing in 2024 lol). Doing this allows me to learn more about the inner-workings of the [Polished Crystal](https://github.com/Rangi42/polishedcrystal/tree/9bit), and gives me the chance to work with C++ and Web Assembly in a more "production" setting.

## Differences

With the different runtimes from the original program to this app, there are also some slight changes in operations, particularly in how files are loaded. Since tileset and metatileset data are stored across different files, it will be necessarily to open your game's project folder, as opposed to just the single file, before you're able to edit tilesets. There is no way around this with JS. With a full C++ implementation, you're able to open just the map file (.ablk) and the original program could look for the related .bin & .tileset files. But the browser doesn't have the capability to look for those related files unless you explicitly give it access to the directory where all your files are stored. I'd say this is not ideal, but this is effectively what the original program does, just under the hood.

To make up for the above fact, and to take more advantage of the web environment, this app adds a File Explorer to easily access your map files and tabs to have multiple files open at once.

## Build Instructions

Requirements:

- Node 16.x.0 (I'll update!!)
- Empscripten 3.1.51-git
- make

### Compile the C++ into wasm:

```
make
```

_Note_: What's neat is that you used to have to install zlib and libpng yourself, but with emscripten you can specify that you want to use the emscripten ports of those, and they're included!

### Install the node dependencies:

```
npm install
```

### Start the node app

```
npm run start
```

The app will be accessible at http://localhost:3000.

## File Types and Verbiage

These are the files that the app looks for when you open a project (roughly in ascending order of building blocks):

- `gfx/tilesets/*.png` **Tileset files**: these are where usable tiles come from.
- `gfx/tilesets/*.2bpp` **Tileset files**: these are where usable tiles come from. Compiled from `.png`s
- `gfx/tilsets/roofs/*.png` **Roof files**: Define tilests for roofs
- `data/tilesets/*_metatiles.bin` **Metatileset files**: Define reusable tiles, made up from tiles in a tileset. Each metatile has an ID (it's row) and 16 bits, with every 2bits referring to a tile from the tileset it uses.
- `*.ablk` **Map Files**: Make up the maps that we edit and see in-game. Every 2bits in a map file refer to a metatile ID
- `*.pal` **Palette File**: Define the color palettes that can be used.
- `maps/*.asm` **Event Files**: Define which tiles events (and event type) are supposed to occur
- `constants/map_constants.asm` Map constants
- `constants/tileset_constants.asm` Tileset constants
- `data/tilesets/*_attributes.bin` **Attributes Files**: (Deep)Tile palette definitions (GRAY,RED,GREEN, etc.) and tile flipping (x-flip,y-flip) information
- `data/tilesets/*_collision.asm` **Collision Files**: Player collision information, such as collision against a wall or water
- `data/maps/maps.asm` **Map Headers**: Defines which tilesets that map files (.ablk) use

## App Structure

```
polished-map-web
├─ core/
│   ├─ */**.cpp
│   └─ */**.h
├─ src/
│   ├─ App.tsx
│   ├─ index.tsx
│   ├─ components/
│   │   ├─ editor/
│   │   └─ menus/
│   ├─ store/
│   │   ├─ *Slice.ts
│   │   └─ FileManagerSingleton.ts
│   └─ utils/
│       ├─ helper-funcs.ts
│       └─ wasm-funcs.ts
├─ public/
│   ├─ index.html
│   ├─ favicon.ico
│   ├─ main.js
│   └─ main.wasm
├─ node_modules/
│   └─ (dependencies)
├─ package.json
├─ package-lock.json
└─ README.md
```

`core/*/**.cpp`: C++ Processing files

`core/*/**.h`: C++ Headers

`src/components/*/**.tsx`: Component files

`src/store/*Slice.ts`: Redux slice definitions, reducers, and actions

`src/store/FileManagerSingleton.ts`: Singleton class that stores a JS Map, keeping track of file manager handles. This allows for:

- easily keeping track of open map files
- ability to open files in the project directory
- capability to lazily fetch file contents
- ability to save changes to open files

`src/utils/helper-funcs.ts`: General and file-reading/writing-specific helper functions

`src/utils/wasm-funcs.ts`: Functions that handle calling and response-handling of C++ functions

`public/main.wasm`: Compiled C++ as wasm

`public/main.js`: Linker file to define wasm-callable functions

## Original Description + Instructions

A map and tileset editor for [pokecrystal](https://github.com/pret/pokecrystal), [pokegold](https://github.com/pret/pokegold), [pokeyellow](https://github.com/pret/pokeyellow), [pokered](https://github.com/pret/pokered), and hacks including [Polished Crystal v2](https://github.com/Rangi42/polishedcrystal/tree/v2.2.0), [Red++ v3](https://github.com/TheFakeMateo/rpp-backup), [Orange](https://github.com/PiaCarrot/pokeorange), [Prism](http://www.pokemonprism.com/), and many more.

(For projects that use [256–512 tiles](https://github.com/pret/pokecrystal/wiki/Expand-tilesets-from-192-to-255-tiles) and [per-block attributes](<https://github.com/pret/pokecrystal/wiki/Allow-tiles-to-have-different-attributes-in-different-blocks-(including-X-and-Y-flip)>), including [Polished Crystal v3](https://github.com/Rangi42/polishedcrystal), [Red++ v4](https://github.com/TheFakeMateo/RedPlusPlus), [Coral](https://github.com/pkmncoraldev/polishedcoral), [Black and White 3: Genesis](https://github.com/AzureKeys/BW3G), and [Ancient Ruby](https://github.com/BloodlessNS/ancientruby), use [Polished Map++](https://github.com/Rangi42/polished-map/tree/plusplus)!)

Inspired by [crowdmap](https://github.com/yenatch/crowdmap) (now defunct), but implemented with C++ and [FLTK](http://www.fltk.org/), and with more functions for graphics editing.

Latest release: [**4.7.1**](https://github.com/Rangi42/polished-map/releases/tag/v4.7.1)

Follow the steps in [INSTALL.md](INSTALL.md) to install the release copy of Polished Map, or the longer instructions to build it yourself from source.

The [example/](example/) directory contains a minimal pokecrystal project with two test maps. **Kanto.180x135.kanto.blk** is a stitch of every Kanto overworld map (they all use the `kanto` tileset). **Johto.235x135.johto.blk** is a stitch of every Johto overworld map; Goldenrod and Azalea use the `johto_modern` tileset, so try switching tilesets with **Edit→Change Tileset…** or by pressing Ctrl+H.

Browse the menu items, toolbar buttons, and Help dialog to learn how to use Polished Map. And don't miss the mouse controls:

|                          | Blocks Mode   | Events Mode      | Edit Block          | Edit Tileset |
| ------------------------ | ------------- | ---------------- | ------------------- | ------------ |
| **Click/drag**           | Place block   | Move event       | Place tile          | Place pixel  |
| **Middle drag**          | Scroll        | Scroll           |                     |              |
| **Right-click**          | Select block  | Edit event       | Select tile         | Select hue   |
| **Double-click**         |               | Open .asm file   |                     |              |
| **Ctrl+click**           | Replace block |                  | Place 2x2 tiles     | Replace hue  |
| **Shift+click**          | Flood fill    | Folow warp event | Place 2/2 tiles     | Flood fill   |
| **Ctrl+Shift+click**     |               |                  | Place 4x4 tiles     |              |
| **Alt+click**            | Swap blocks   |                  | Place 2+2 tiles     | Swap hues    |
| **Alt+Shift+click**      |               |                  | Place 4+4+4+4 tiles |              |
| **Ctrl+Alt+click**       |               |                  | Place 2-2 tiles     |              |
| **Ctrl+Alt+Shift+click** |               |                  | Place 4-4-4-4 tiles |              |

(All those "Edit Block" mouse controls are explained further in [doc/edit-block.md](doc/edit-block.md).)

More information is at the [Skeetendo Forums](https://hax.iimarckus.org/topic/7222/) or [PokéCommunity](https://www.pokecommunity.com/showthread.php?t=425994). If you have questions or comments, please go there.

![Screenshot](screenshot.png)

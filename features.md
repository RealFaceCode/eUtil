# Current Features

## App

- **`app.hpp`**
  - `App` class for managing the application lifecycle.
  - Support for event-driven architecture.
  - Scene initialization and management.
  - Configurable update and render intervals.

## Buffer

- **`Array.hpp`**
  - `Array` class for flexible data storage and manipulation.
  - Support for writing and reading various data types, including strings, vectors, and custom types.
  - Customizable read and write rules for unsupported types.
  - Overloaded operators (`<<` and `>>`) for intuitive data handling.
  - Utility functions for resizing, clearing, and resetting the buffer.

- **`FilePermission.hpp`**
  - `FilePermission` structure for managing file read, write, and execute permissions.

- **`FileAccessTimes.hpp`**
  - `FileAccessTimes` structure for tracking file creation, last access, and last write times.

- **`imagedata.hpp`**
  - `ImageData` structure for managing image data.
  - Support for setting and retrieving image dimensions, channels, and raw data.
  - Integration with image loading and saving utilities.

## Event

- **`event.hpp`**
  - Base `Event` class for creating custom event types.
  - Functions to check and set event handling status.
  - Virtual methods for retrieving event type and name.

- **`eventdispatcher.hpp`**
  - `EventDispatcher` class for handling and dispatching events.
  - Support for type-based event callbacks.

## Filesystem

- **`FileIOGeneric.hpp`**
  - Functions for file operations: create, remove, copy, move, and rename files.
  - Directory operations: create, remove, copy, move, and rename directories.
  - Metadata handling: access times, permissions, and size.

- **`FileLoad.hpp`**
  - Functions for loading files and images.
  - Support for optional image flipping during loading.

- **`FileWrite.hpp`**
  - Functions for writing files and images in various formats (PNG, BMP, JPG, TGA, HDR).

- **`saveloadrule.hpp`**
  - Save and load rules for custom data serialization.
  - Functions to add, remove, and use save/load rules.

- **`hotloader.hpp`**
  - `HotLoader` for monitoring file changes.
  - Functions to add, remove, and check modified files.

## Scene

- **`scene.hpp`**
  - `Scene` class for managing individual scenes.
  - Virtual methods for attaching, detaching, updating, rendering, and handling events.
  - Functions to manage scene-specific event queues.

- **`scenemanager.hpp`**
  - `SceneManager` class for managing multiple scenes.
  - Functions to push, pop, and retrieve scenes.
  - Support for pausing and resuming updates, rendering, and events for individual scenes.

## Thread

- **`thread.hpp`**
  - `Thread` class for managing individual threads.
  - Support for custom task functions and thread-safe task queues.
  - Functions to start, stop, and join threads.

- **`threadPool.hpp`**
  - `ThreadPool` class for managing a pool of threads.
  - Support for parallel task execution.
  - Dynamic thread count adjustment and thread-safe result retrieval.

## Time

- **`timer.hpp`**
  - `Timer` class for measuring elapsed time.
  - Functions to start, stop, reset, and retrieve elapsed time.

- **`signaltimer.hpp`**
  - `SignalTimer` class for periodic task execution.
  - Configurable active and inactive intervals.
  - Functions to start, stop, reset, and check timer signals.

- **`time.hpp`**
  - Utility functions for retrieving formatted current time and date.
